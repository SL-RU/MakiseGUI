MakiseGUI 

Makise GUI - opensource crossplatform GUI library written on pure C99. It targets on ARM MCU but works fine everywhere - it has no external dependencies.

![logo](https://github.com/SL-RU/MakiseGUI/blob/master/MakiseGUI/docs/logo.png)

STAGE: ACTIVE DEVELOPMENT

## About

Graphics library for embed ARM platforms. It will support variety of displays and controllers. It allows to draw only primitives or use GUI.

### Mission

To simplify process of development GUI interfaces for embed systems. And make possible to create modern GUI for everyone.

## Main features
* free and opensource GUI for everyone
* absolutely crossplatform - written fully on gnu-c99 without any external libraries. It can be used with any MCU, PC or other platform
* Minimal resource usage. Can work fine without external memory. F.e. it requires less then 40kb for color 320x240 display.
* All base controls: buttons, sliders, tabs and etc
* supports keyboard or touch input
* customisable - any color can be configured even in runtime
* EASY TO USE. Low barrier of entry and easy developing. Almost as simple as WindowsForms or etc
* etc

## Usage

Documentation is in /MakiseGUI/docs. But it's in progress. You can see an examples here: https://github.com/SL-RU/MakiseSDLTest

## Elements
* Button
* Lable
* Text field
* Canvas(simple container)
* Slider
* List/Radiobuttons/Checkbox
* Tabs
* Toggle button
* more will be soon

## Drivers

It supports batch drawing(two buffers: one wich contains all pixels with low pixeldepth and second smaller with display's native pixeldepth in which data will be rendered from first using palette and after that sended to display using DMA).

Existing drivers:
- 2.2 inch tft 240x320 on ILI9340C
- SDL2 for PC applications
You can easily write your own driver. Documentation(will be soon).


PULL REQUESTS, QUESTIONS, ISSUES and etc are welcome!

Author: Alexander Lutsai <s.lyra@ya.ru>
