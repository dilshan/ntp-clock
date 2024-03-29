//---------------------------------------------------------------------------------------
// Ethernet base NTP clock panel driver.
// Support routines for Seven-segment display.
// 
// Copyright (c) 2021 Dilshan R Jayakody. [jayakody2000lk@gmail.com]
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//---------------------------------------------------------------------------------------

#ifndef NETCLK_DISPLAY
#define NETCLK_DISPLAY

typedef enum DISPLAY_MODE 
{
    DISP_TIME,
    DISP_DATE,
    DISP_SECONDS
} DISPLAY_MODE;

void setSegmentValue(unsigned char val, bool dot);
void selectSegment(unsigned char segment);

void displayTime(unsigned char *segVal);
void displaySeconds(unsigned char *segVal);
unsigned char formatHours(bool hr24, unsigned char hr);

void clearDisplay();
void initDisplay();
void activateRefreshTimer();

#endif
