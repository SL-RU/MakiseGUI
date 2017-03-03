#if MAKISE_ILI9340_USE == 1
#ifndef ILI9340_H
#define ILI9340_H
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include <stdio.h>

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "ili9340_registers.h"

#include "makise.h"



#define ILI9340_SPI   hspi1
#define ILI9340_CS    GPIOB, GPIO_PIN_0
#define ILI9340_DC    GPIOC, GPIO_PIN_4
#define ILI9340_LED   GPIOB, GPIO_PIN_1
#define ILI9340_RST   GPIOC, GPIO_PIN_5
#define ILI9340_LED_PWM &htim3
#define ILI9340_LED_PWM_CHANNEL TIM_CHANNEL_4
#define ILI9340_LED_USE_PWM 1


void ili9340_driver(MakiseDriver*);

uint8_t ili9340_init (MakiseGUI* gui);
uint8_t ili9340_start(MakiseGUI* gui);
uint8_t ili9340_sleep(MakiseGUI* gui);
uint8_t ili9340_awake(MakiseGUI* gui);
uint8_t ili9340_set_backlight(MakiseGUI* gui, uint8_t);

uint8_t ili9340_spi_txhalfcplt(MakiseDriver* driver);
uint8_t ili9340_spi_txcplt(MakiseDriver* driver);

//void    ili9340_render(MakiseGUI* gui);

uint8_t ili9340_write_data(uint8_t d);
uint8_t ili9340_write_command(uint8_t c);
#endif
#endif
