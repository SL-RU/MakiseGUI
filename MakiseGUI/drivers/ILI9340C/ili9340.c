#include "makise_config.h"
#if MAKISE_ILI9340_USE == 1
#include "ili9340.h"
#include "stm32f4xx_hal_tim.h"
#include "dma.h"

extern inline void _ili9340_delay(uint32_t x) { HAL_Delay(x); }
extern inline void _ili9340_rst(uint8_t st){HAL_GPIO_WritePin(ILI9340_RST, st ? GPIO_PIN_SET : GPIO_PIN_RESET);}
extern inline void _ili9340_cs(uint8_t st){HAL_GPIO_WritePin(ILI9340_CS, st ? GPIO_PIN_SET : GPIO_PIN_RESET);}
extern inline void _ili9340_dc(uint8_t st){HAL_GPIO_WritePin(ILI9340_DC, st ? GPIO_PIN_SET : GPIO_PIN_RESET);}

uint8_t ili9340_backlight = 0;

void ili9340_driver(MakiseDriver * d)
{
    d->lcd_height    = 240;
    d->lcd_width     = 320;
    d->buffer_height = MAKISE_BUF_H;
    d->buffer_width  = MAKISE_BUF_W;
    d->pixeldepth    = 16;
    d->buffer        = 0;
    d->size          = MAKISE_BUF_H * MAKISE_BUF_W * 2;
    d->posx          = 0;
    d->posy          = 321;
    d->init          = &ili9340_init;
    d->start         = &ili9340_start;
    d->sleep         = &ili9340_sleep;
    d->awake         = &ili9340_awake;
    d->set_backlight = &ili9340_set_backlight;
}

uint8_t ili9340_init(MakiseGUI* gui)
{
    _ili9340_rst(1);
    _ili9340_delay(5);
    _ili9340_rst(0);
    _ili9340_delay(20);
    _ili9340_rst(1);
    _ili9340_delay(150);

    ili9340_write_command(0x01); //software reset
    _ili9340_delay(5);
    ili9340_write_command(0x28);
    // display off
    //---------------------------------------------------------
    // magic?
    ili9340_write_command(0xcf);
    ili9340_write_data(0x00);
    ili9340_write_data(0x83);
    ili9340_write_data(0x30);

    ili9340_write_command(0xed);
    ili9340_write_data(0x64);
    ili9340_write_data(0x03);
    ili9340_write_data(0x12);
    ili9340_write_data(0x81);
    ili9340_write_command(0xe8);
    ili9340_write_data(0x85);
    ili9340_write_data(0x01);
    ili9340_write_data(0x79);
    ili9340_write_command(0xcb);
    ili9340_write_data(0x39);
    ili9340_write_data(0x2c);
    ili9340_write_data(0x00);
    ili9340_write_data(0x34);
    ili9340_write_data(0x02);
    ili9340_write_command(0xf7);
    ili9340_write_data(0x20);
    ili9340_write_command(0xea);
    ili9340_write_data(0x00);
    ili9340_write_data(0x00);
    //------------power control------------------------------
    ili9340_write_command(0xc0); //power control
    ili9340_write_data(0x26);
    ili9340_write_command(0xc1); //power control
    ili9340_write_data(0x11);
    //--------------VCOM
    ili9340_write_command(0xc5); //vcom control
    ili9340_write_data(0x35);//35
    ili9340_write_data(0x3e);//3E
    ili9340_write_command(0xc7); //vcom control
    ili9340_write_data(0xbe); // 0x94
    //------------memory access control------------------------
    ili9340_write_command(0x36);
    // memory access control
    ili9340_write_data(0b11101000); //0048 my,mx,mv,ml,BGR,mh,0.0
    ili9340_write_command(0x3a); // pixel format set
    ili9340_write_data(0x55);//16bit /pixel
    //----------------- frame rate------------------------------
    ili9340_write_command(0xb1);
    // frame rate
    ili9340_write_data(0x00);
    ili9340_write_data(0x1B); //70
    //----------------Gamma---------------------------------
    ili9340_write_command(0xf2); // 3Gamma Function Disable
    ili9340_write_data(0x08);
    ili9340_write_command(0x26);
    ili9340_write_data(0x02); // gamma set 4 gamma curve 01/02/04/08

    ili9340_write_command(0xE0); //positive gamma correction
    ili9340_write_data(0x0F); 
    ili9340_write_data(0x31); 
    ili9340_write_data(0x2B); 
    ili9340_write_data(0x0C); 
    ili9340_write_data(0x0E); 
    ili9340_write_data(0x08); 
    ili9340_write_data(0x4E); 
    ili9340_write_data(0xF1); 
    ili9340_write_data(0x37); 
    ili9340_write_data(0x07); 
    ili9340_write_data(0x10); 
    ili9340_write_data(0x03); 
    ili9340_write_data(0x0E); 
    ili9340_write_data(0x09); 
    ili9340_write_data(0x00); 
    ili9340_write_command(0xE1); //negamma correction
    ili9340_write_data(0x00); 
    ili9340_write_data(0x0E); 
    ili9340_write_data(0x14); 
    ili9340_write_data(0x03); 
    ili9340_write_data(0x11); 
    ili9340_write_data(0x07); 
    ili9340_write_data(0x31); 
    ili9340_write_data(0xC1); 
    ili9340_write_data(0x48); 
    ili9340_write_data(0x08); 
    ili9340_write_data(0x0F); 
    ili9340_write_data(0x0C); 
    ili9340_write_data(0x31); 
    ili9340_write_data(0x36); 
    ili9340_write_data(0x0F); 
    //--------------ddram ---------------------
    ili9340_write_command(0x2a);
    // column set
    // size = 239
    ili9340_write_data(0x00);
    ili9340_write_data(0x00);
    ili9340_write_data(0x01);
    ili9340_write_data(0x3F);
    ili9340_write_command(0x2b);
    // page address set
    // size = 319
    ili9340_write_data(0x00);
    ili9340_write_data(0x00);
    ili9340_write_data(0x00);
    ili9340_write_data(0xEF);
    ili9340_write_command(0x34);
    ili9340_write_command(0x35);
    ili9340_write_data(0x01);
    // tearing effect off
    // tearing effect on
    // ili9340_write_command(0xb4); // display inversion
    ili9340_write_command(0xb7); //entry mode set
    ili9340_write_data(0x07);
    //-----------------display---------------------
    ili9340_write_command(0xb6);
    // display function control
    ili9340_write_data(0x0a);
    ili9340_write_data(0x82);
    ili9340_write_data(0x27);
    ili9340_write_data(0x00);
    ili9340_write_command(0x11); //sleep out
    _ili9340_delay(100);
    ili9340_write_command(0x29); // display on
    _ili9340_delay(100);

    
    return HAL_OK;
}

//set window for drawing buffer
void _ili9340_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1,
		   uint16_t y1)
{
    uint8_t _ili9340_addr[2];

    _ili9340_cs(0);
    ili9340_write_command(ILI9340_CASET); // Column addr set
    
    _ili9340_dc(1);
    _ili9340_addr[0] = x0 >> 8;
    _ili9340_addr[1] = x0;
    _ili9340_addr[2] = x1 >> 8;
    _ili9340_addr[3] = x1;
    HAL_SPI_Transmit(&ILI9340_SPI, (uint8_t*)_ili9340_addr, 4, 10);

    ili9340_write_command(ILI9340_PASET); // Row addr set

    _ili9340_dc(1);
    _ili9340_addr[0] = y0 >> 8;
    _ili9340_addr[1] = y0;
    _ili9340_addr[2] = y1 >>8;
    _ili9340_addr[3] = y1;
    HAL_SPI_Transmit(&ILI9340_SPI, (uint8_t*)_ili9340_addr, 4, 10);

    ili9340_write_command(ILI9340_RAMWR); // write to RAM
}

void ili9340_tx(MakiseDriver* d)
{    
    d->posy = 0;   
    
    if(d->gui->draw != 0)
    {
	d->gui->draw(d->gui);
    }
    
    _ili9340_setAddrWindow(0, d->posy, d->lcd_width, d->buffer_height - 1 + d->posy);
    makise_render(d->gui, 0);

    _ili9340_dc(1);
    _ili9340_cs(0);

    HAL_SPI_Transmit_DMA(&ILI9340_SPI, d->buffer, d->size);
}

uint8_t ili9340_start(MakiseGUI* gui)
{
#if ILI9340_LED_USE_PWM
    HAL_TIM_Base_Start(ILI9340_LED_PWM);
    HAL_TIM_PWM_Start(ILI9340_LED_PWM, ILI9340_LED_PWM_CHANNEL);
#endif
//    ili9340_set_backlight(1);
    ili9340_set_backlight(gui, 31);
    ili9340_tx(gui->driver);

    return M_OK;
}
uint8_t ili9340_sleep(MakiseGUI* gui)
{
    return M_OK;
}
uint8_t ili9340_awake(MakiseGUI* gui)
{
    return M_OK;
}
uint8_t ili9340_set_backlight(MakiseGUI* gui, uint8_t val)
{
#if ILI9340_LED_USE_PWM
    __HAL_TIM_SET_COMPARE(ILI9340_LED_PWM, ILI9340_LED_PWM_CHANNEL, val);
#else
    HAL_GPIO_WritePin(ILI9340_LED, val ? GPIO_PIN_SET : GPIO_PIN_RESET);
#endif
    return M_OK;
}
uint8_t ili9340_spi_txhalfcplt(MakiseDriver* d)
{
    if(d->posy < d->lcd_height)
	makise_render(d->gui, 1);
    return M_OK;
}
uint8_t ili9340_spi_txcplt(MakiseDriver* d)
{
//    _ili9340_cs(1);

    uint8_t dr = 0;
    MakiseBuffer *bu = d->gui->buffer;

    if(d->posy >= d->lcd_height)
    {
	dr = 1;
	d->posy = 0;
	
	memset(bu->buffer + 
	       d->lcd_width * (d->lcd_height - d->buffer_height) * bu->pixeldepth / 32,
	       0,
	       d->lcd_width * d->buffer_height * bu->pixeldepth / 8);

	if(d->gui->draw != 0)
	{
	    d->gui->draw(d->gui);
	}
	
    }

    _ili9340_setAddrWindow(0, d->posy, d->lcd_width, d->buffer_height - 1 + d->posy);
    makise_render(d->gui, dr ? 0 : 2);
    
    _ili9340_dc(1);
    _ili9340_cs(0);


    HAL_DMA_Start_IT(ILI9340_SPI.hdmatx, (uint32_t)d->buffer, (uint32_t)&ILI9340_SPI.Instance->DR, d->size);
    /* Enable the SPI Error Interrupt Bit */
    SET_BIT(ILI9340_SPI.Instance->CR2, SPI_CR2_ERRIE);

    /* Enable Tx DMA Request */
    SET_BIT(ILI9340_SPI.Instance->CR2, SPI_CR2_TXDMAEN);
    
    if(dr)
    {
	if(d->gui->predraw != 0)
	{
	    d->gui->predraw(d->gui);
	}
	if(d->gui->update != 0)
	{
	    d->gui->update(d->gui);
	}
    }
    else
    {
	memset(bu->buffer + (d->posy - d->buffer_height * 2) *
	       d->lcd_width * bu->pixeldepth / 32,
	       0,
	       d->lcd_width * d->buffer_height * bu->pixeldepth / 8);
    }
    
    return M_OK;
}

uint8_t ili9340_write_data(uint8_t d)
{
    _ili9340_dc(1);
    HAL_SPI_Transmit(&ILI9340_SPI, &d, 1, 10);
    return d;
}

uint8_t ili9340_write_command(uint8_t c)
{
    _ili9340_dc(0);
    HAL_SPI_Transmit(&ILI9340_SPI, &c, 1, 10);
    return c;
}
#endif
