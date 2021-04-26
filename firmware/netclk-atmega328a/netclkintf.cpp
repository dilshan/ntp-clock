//---------------------------------------------------------------------------------------
// Ethernet base NTP clock panel driver.
// PCF8574 based utility functions.
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

#include "Arduino.h"
#include <jm_PCF8574.h>

#include "netclkintf.h"

// I2C address of the PCF8574 I/O expander.
#define PCF8574_ADDRESS   0x20

// I/O ports exposed to user.
#define EXPIO_CONFIG_LED  IND_BUSY_CONFIG
#define EXPIO_SYNC_LED    IND_SYNC
#define EXPIO_BTN_DATE    0x02
#define EXPIO_BTN_SYNC    0x03

// I/O ports used for internal functions.
#define EXPIO_NET_RESET   0x04
#define EXPIO_SYS_RESET   0x05

jm_PCF8574 pcf8574;

void initInterface()
{
  pcf8574.begin(PCF8574_ADDRESS);

  // Configure output interfaces of the I/O expander.
  pcf8574.pinMode(EXPIO_CONFIG_LED, OUTPUT);
  pcf8574.digitalWrite(EXPIO_CONFIG_LED, HIGH); 

  pcf8574.pinMode(EXPIO_SYNC_LED, OUTPUT);
  pcf8574.digitalWrite(EXPIO_SYNC_LED, HIGH);   

  // Configure input interfaces of the I/O expander.
  pcf8574.pinMode(EXPIO_BTN_DATE, INPUT);
  pcf8574.digitalWrite(EXPIO_BTN_DATE, HIGH);

  pcf8574.pinMode(EXPIO_BTN_SYNC, INPUT);
  pcf8574.digitalWrite(EXPIO_BTN_SYNC, HIGH);

  // Enable ethernet interface.
  pcf8574.pinMode(EXPIO_NET_RESET, OUTPUT);
  pcf8574.digitalWrite(EXPIO_NET_RESET, HIGH); 

  // Factory reset interface.
  pcf8574.pinMode(EXPIO_SYS_RESET, INPUT);
  pcf8574.digitalWrite(EXPIO_SYS_RESET, HIGH);
}

void setIndicator(unsigned char indicator, bool isActive)
{
  pcf8574.digitalWrite(indicator, (isActive ? LOW : HIGH));
}

void resetEthernet()
{
  pcf8574.digitalWrite(EXPIO_NET_RESET, LOW); 
  delay(5);
  pcf8574.digitalWrite(EXPIO_NET_RESET, HIGH); 
  delay(5);
}

bool getDateButtonState()
{
  return (pcf8574.digitalRead(EXPIO_BTN_DATE) == HIGH);
}

bool getSyncButtonState()
{
  return (pcf8574.digitalRead(EXPIO_BTN_SYNC) == HIGH);
}

bool getFactorResetState()
{
  return (pcf8574.digitalRead(EXPIO_SYS_RESET) == HIGH);
}