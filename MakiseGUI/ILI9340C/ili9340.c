//#ifdef MAKISE_ILI9340_USE
#include "ili9340.h"
#include "stm32f4xx_hal_tim.h"

extern inline void _ili9340_delay(uint32_t x) { HAL_Delay(x); }
extern inline void _ili9340_rst(uint8_t st){HAL_GPIO_WritePin(ILI9340_RST, st ? GPIO_PIN_SET : GPIO_PIN_RESET);}
extern inline void _ili9340_cs(uint8_t st){HAL_GPIO_WritePin(ILI9340_CS, st ? GPIO_PIN_SET : GPIO_PIN_RESET);}
extern inline void _ili9340_dc(uint8_t st){HAL_GPIO_WritePin(ILI9340_DC, st ? GPIO_PIN_SET : GPIO_PIN_RESET);}

uint8_t ili9340_backlight = 0;

void ili9340_driver(MakiseDriver * d)
{
    d->lcd_height    = 320;
    d->lcd_width     = 240;
    d->buffer_height = MAKISE_BUF_H;
    d->buffer_width  = MAKISE_BUF_W;
    d->pixeldepth    = 16;
    d->buffer        = 0;
    d->size          = MAKISE_BUF_H * MAKISE_BUF_W * 2;
    d->posx          = 0;
    d->posy          = 0;
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
    ili9340_write_data(0x48); //0048 my,mx,mv,ml,BGR,mh,0.0
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
    ili9340_write_data(0x00);
    ili9340_write_data(0xEF);
    ili9340_write_command(0x2b);
    // page address set
    // size = 319
    ili9340_write_data(0x00);
    ili9340_write_data(0x00);
    ili9340_write_data(0x01);
    ili9340_write_data(0x3F);
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
uint8_t _ili9340_addr_kek[2];
void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1,
		   uint16_t y1) {

    _ili9340_cs(0);
    ili9340_write_command(ILI9340_CASET); // Column addr set
    
    _ili9340_dc(1);
    _ili9340_addr_kek[0] = x0 >> 8;
    _ili9340_addr_kek[1] = x0;
    _ili9340_addr_kek[2] = x1 >> 8;
    _ili9340_addr_kek[3] = x1;
    HAL_SPI_Transmit(&ILI9340_SPI, (uint8_t*)_ili9340_addr_kek, 4, 10);

    ili9340_write_command(ILI9340_PASET); // Row addr set

    _ili9340_dc(1);
    _ili9340_addr_kek[0] = y0 >> 8;
    _ili9340_addr_kek[1] = y0;
    _ili9340_addr_kek[2] = y1 >>8;
    _ili9340_addr_kek[3] = y1;
    HAL_SPI_Transmit(&ILI9340_SPI, (uint8_t*)_ili9340_addr_kek, 4, 10);

    ili9340_write_command(ILI9340_RAMWR); // write to RAM
}

uint32_t t, ss, se, w=0;
uint8_t ili_f = 0;
extern uint16_t taaak = 0;
void strt_tx(MakiseDriver* d)
{
//    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
    uint8_t dr = 0;
    if(d->posy >= d->lcd_height - taaak)
    {
	ili_f = 0;
	dr = 1;
	d->posy = taaak;
	
	//se = HAL_GetTick();
	//printf("delay %d\n", se - ss);
	//ss = HAL_GetTick();
	
//	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
	if(d->gui->draw != 0)
	{
	    d->gui->draw(d->gui);
	}
//	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
    }


    setAddrWindow(0, d->posy, d->lcd_width, d->buffer_height - 1 + d->posy);
    ili9340_render(d->gui);


    _ili9340_dc(1);
    _ili9340_cs(0);

    if(w==0)
    {
	HAL_SPI_Transmit_DMA(&ILI9340_SPI, d->buffer, d->size);
	w = 1;
    }
    else
    {
	HAL_DMA_Start_IT(ILI9340_SPI.hdmatx, (uint32_t)d->buffer, (uint32_t)&ILI9340_SPI.Instance->DR, d->size);
	  /* Enable the SPI Error Interrupt Bit */
	SET_BIT(ILI9340_SPI.Instance->CR2, SPI_CR2_ERRIE);

	/* Enable Tx DMA Request */
	SET_BIT(ILI9340_SPI.Instance->CR2, SPI_CR2_TXDMAEN);

    }
//    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
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
}

uint8_t ili9340_start(MakiseGUI* gui)
{
    if(ILI9340_LED_USE_PWM)
    {
	HAL_TIM_Base_Start(ILI9340_LED_PWM);
	HAL_TIM_PWM_Start(ILI9340_LED_PWM, ILI9340_LED_PWM_CHANNEL);
    }
//    ili9340_set_backlight(1);
    ili9340_set_backlight(gui, 31);
    strt_tx(gui->driver);
}
uint8_t ili9340_sleep(MakiseGUI* gui)
{
    
}
uint8_t ili9340_awake(MakiseGUI* gui)
{
    
}
uint8_t ili9340_set_backlight(MakiseGUI* gui, uint8_t val)
{
    if(ILI9340_LED_USE_PWM)
	__HAL_TIM_SET_COMPARE(ILI9340_LED_PWM, ILI9340_LED_PWM_CHANNEL, val);
    else
	HAL_GPIO_WritePin(ILI9340_LED, val ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
uint8_t ili9340_spi_txhalfcplt(MakiseDriver* d)
{
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
    ili_f = 1;
    if(d->posy < d->lcd_height)
	ili9340_render(d->gui);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
}
uint8_t ili9340_spi_txcplt(MakiseDriver* driver)
{
    ili_f = 2;
    _ili9340_cs(1);
    strt_tx(driver);
}

uint16_t clrs[] = {ILI9340_BLACK, ILI9340_WHITE, ILI9340_RED,
		   ILI9340_GREEN, ILI9340_BLUE, ILI9340_CYAN,
		   ILI9340_YELLOW, ILI9340_MAGENTA};
void    ili9340_render(MakiseGUI* gui)
{
    MakiseDriver * d = gui->driver;
    uint16_t c;

    uint32_t y = d->posy, x = 0, i = 0,
	cu, bu = 0, m;

    if(ili_f == 0) //render full buffer
    {
	m = d->posy + d->buffer_height;
	d->posy += d->buffer_height;
	ili_f = 1;
    }
    else if(ili_f == 1) //render first half (called by halfcplt callback)
    {
	m = d->posy + (d->buffer_height / 2);
	y = d->posy;
    }
    else if(ili_f == 2) //render second half (called by cplt callback)
    {
	m = d->posy + d->buffer_height;
	y = d->posy + (d->buffer_height / 2);
	i = d->size / 4;
	d->posy += d->buffer_height;
    }
    
    cu = (y * gui->buffer->width)*gui->buffer->pixeldepth/32;
    for (; y < m; y++) {	
	for (x = 0; x < d->buffer_width; x+=1)
	{
	    c = clrs[((((uint32_t*)gui->buffer->buffer)[cu]) >> bu) & gui->buffer->depthmask];
	    bu += gui->buffer->pixeldepth;
	    if(bu>=32)
	    {
		bu = 0;
		cu ++;
	    }
	    //c+=bc;
	    ((uint16_t*)d->buffer)[i] = c;
	    i+=1;
	}
    }
}


uint8_t ili9340_write_data(uint8_t d)
{
    _ili9340_dc(1);

    HAL_SPI_Transmit(&ILI9340_SPI, &d, 1, 10);
}

uint8_t ili9340_write_command(uint8_t c)
{
    _ili9340_dc(0);

    HAL_SPI_Transmit(&ILI9340_SPI, &c, 1, 10);
    
}
//#endif
