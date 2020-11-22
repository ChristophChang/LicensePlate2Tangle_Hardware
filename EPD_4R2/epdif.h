/**
 *  @filename   :   epdif.h
 *  @brief      :   Header file of epdif.cpp providing EPD interface functions
 *                  Users have to implement all the functions in epdif.cpp
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 *  Modified by Kenji Arai / JH1PJL
 *
 *  http://www.page.sannet.ne.jp/kenjia/index.html
 *  http://mbed.org/users/kenjiArai/
 *      Created:    April     27th, 2019
 *      Revised:    May       26th, 2019
 *
 *  Refrence software
 *  https://github.com/waveshare/e-Paper
 *  https://os.mbed.com/users/imachooon/code/epd1in54/
 *
 *  Technical documents
 *  https://www.waveshare.com/wiki/2.13inch_e-Paper_HAT_(B)
 *
 *  Product
 *  http://akizukidenshi.com/catalog/g/gP-13757/
 *  https://www.waveshare.com/2.13inch-e-paper-hat-b.htm
 *
 */

#ifndef EPDIF_H
#define EPDIF_H

#include "mbed.h"

class EpdIf
{
public:
    EpdIf(void);
    EpdIf(PinName mosi,
          PinName miso,
          PinName sclk,
          PinName cs,
          PinName dc,
          PinName rst,
          PinName busy,
          PinName pwr);
    ~EpdIf(void)
    {
        ;
    }

    int  IfInit(void);
    void DigitalWrite(DigitalOut* pin, int value);
    int  DigitalRead(DigitalIn* pin);
    void DelayMs(unsigned int delaytime);
    void SpiTransfer(unsigned char data);
    void all_off(void);

    SPI* m_spi;
    DigitalOut* m_cs;
    DigitalOut* m_dc;
    DigitalOut* m_rst;
    DigitalIn*  m_busy;
    DigitalOut* m_pwr;
    PinName p_mosi;
    PinName p_sclk;
    DigitalOut* m_mosi;
    DigitalOut* m_sclk;
};

#endif
