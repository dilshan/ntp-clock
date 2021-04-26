//---------------------------------------------------------------------------------------
// Ethernet base NTP clock panel driver.
// Generic support routines.
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

#include <avr/wdt.h>

#include "Arduino.h"

#include "netclkutil.h"
#include "netclkintf.h"

unsigned char getKeyInput(unsigned char keyLo, unsigned char keyHi)
{
  unsigned char inKey = 0;
  
  while(!((inKey > keyLo) && (inKey < keyHi)))
  {
    if(Serial.available() > 0)
    {
      inKey = Serial.read();
    }

    // Accept <ESC> key to cancel the current operation.
    if(inKey == 0x1B)
    {
      break;
    }
  }

  return inKey;
}

void setDefaultConfig(SystemConfig *config)
{
  // Set default network type to DHCP.
  config->networkType = NET_TYPE_DHCP;

  // Configure default device static IP as 192.168.1.112
  config->deviceIP[0] = 192;
  config->deviceIP[1]=  168;
  config->deviceIP[2] = 1;
  config->deviceIP[3] = 112;

  // Configure default netmask as 255.255.255.0
  config->netmask[0] = 255;
  config->netmask[1]=  255;
  config->netmask[2] = 255;
  config->netmask[3] = 0;

  // Configure default gateway as 192.168.1.1
  config->gateway[0] = 192;
  config->gateway[1] = 168;
  config->gateway[2] = 1;
  config->gateway[3] = 1;
}

bool inputPort(unsigned char *port, unsigned char defaultVal)
{
  bool isPortCont = true;
  char tmp;
  unsigned char inBuffer[3];
  unsigned char bufPos = 0;

  while(isPortCont)
  {
    while (Serial.available() > 0)
    {
      tmp = Serial.read();
      if(isDigit(tmp))
      {
        inBuffer[bufPos++] = (tmp - 0x30);
        Serial.print(tmp);
        if(bufPos == 3)
        {
          // All 3 digits are enter by the user.
          isPortCont = false;
          break;
        }
      }
      else if((bufPos > 0) && (tmp == 0x0D))
      {
        // User enter <RETURN> key to close the current input session.
        isPortCont = false;
        break;
      }
      else if(tmp == 0x1B)
      {
        // User presss <ESC> key to cancel the input.
        return false; 
      }
    }
  }

  if(bufPos > 0)
  {
    // Convert specified string to byte.
    word val = 0;

    for(tmp = 0; tmp < bufPos; tmp++)
    {
      val = (val * 10) + inBuffer[tmp];
    }

    if(val <= 0xFF)
    {
      // Specified input is valid byte.
      *port = val;
    }
    else
    {
      // Specified input is larger than 255.
      *port = defaultVal;
    }
  }

  return true;
}

bool inputIpAddress(unsigned char *ipData)
{
  char segData[] = {0, 0, 0};
  unsigned char segLength = 0;
  bool isSegCont = true;
  unsigned char segPos = 0;
  unsigned char tempIp[] = {0, 0, 0, 0};
  char tmp;

  while(segPos < 4)
  {
    // Continue to fill next available byte of the IP address.
    while(isSegCont)
    {
      while (Serial.available() > 0)
      {
        tmp = Serial.read();
        if(isDigit(tmp))
        {          
          segData[segLength++] = (tmp - 0x30);
          Serial.print(tmp);
          if(segLength == 3)
          {
            // All 3 digits are entered by the user.
            if(segPos != 3)
            {
              Serial.print('.');
            }

            isSegCont = false;
            break;
          }
        }
        else if((segLength > 0) && (tmp == 0x2E))
        {
          // User enter . to close the current byte.
          Serial.print(tmp);
          isSegCont = false;
          break;
        }
        else if((segLength > 0) && (tmp == 0x0D))
        {
          // User enter <RETURN> key to close the current byte.
          if(segPos != 3)
          {
            Serial.print('.');
          }

          isSegCont = false;
          break;
        }
        else if(tmp == 0x1B)
        {
          // User presss <ESC> key to cancel the input.
          return false; 
        }
      }
    }

    if(segLength > 0)
    {
      // Convert specified string to byte.
      word val = 0;

      for(tmp = 0; tmp < segLength; tmp++)
      {
        val = (val * 10) + segData[tmp];
      }

      if(val <= 0xFF)
      {
        // Valid byte.
        tempIp[segPos] = (unsigned char)val;
      }
      else
      {
        // Invalid number and fill current byte with 0XFF.
        tempIp[segPos] = 0xFF;
      }
      
      // Move to next byte of the IP address.
      segPos++;

      segData[0] = 0;
      segData[1] = 0;
      segData[2] = 0;

      segLength = 0;
      isSegCont = true;
    }
  }

  // End of user input. Copy specified address into variable.
  memcpy(ipData, tempIp, 4);
  return true;
}

long inputTimeZone()
{
  unsigned char segPos = 0;
  unsigned char bufPos = 0;
  unsigned char inBuffer[2];
  unsigned char offsetComp[3];
  unsigned char val;
  char tmp;
  bool isZoneCont = true;
  bool isSegmentCont = true;
  long returnVal = 0;

  while(isZoneCont)
  {
    // Loop to handle each component of the time zone. [SYMBOL][HH][MM]
    while(isSegmentCont)
    {
      while (Serial.available() > 0)
      {
        tmp = Serial.read();

        if(tmp == 0x1B)
        {
          // User presss <ESC> key to cancel the input.
          return 1;
        }
  
        if(segPos == 0)
        {
          // For first digit accept only + or - symbols.
          if((tmp == 0x2B) || (tmp == 0x2D))
          {
            offsetComp[0] = tmp;
            segPos++;
            Serial.print(tmp);
          }
        }
        else
        {
          // Handle time value components.
          if(isDigit(tmp))
          {
            inBuffer[bufPos] = tmp;
            Serial.print(tmp);
            bufPos++;
            
            if(bufPos == 2)
            {
              // End of buffer is reached.
              isSegmentCont = false;
              break;
            }
          }
          else if((bufPos > 0) && (tmp == 0x0D))
          {
            // User hit <RETURN> key.
            isSegmentCont = false;
            break;
          }
          else if((bufPos > 0) && (tmp == 0x3A) && (segPos == 1))
          {
            // User hit : key to insert time seperator.
            isSegmentCont = false;
            break;
          }
        }
      }
    }

    // Process captured content.
    if(bufPos > 0)
    {
      val = 0;
      
      for(tmp = 0; tmp < bufPos; tmp++)
      {
        val = (val * 10) + (inBuffer[tmp] - 0x30);
      }

      if((segPos == 1) && (val > 15))
      {
        // Validate and limit hour component.
        val = 12;
      }
      else if((segPos == 2) && (val > 59))
      {
        // Validate and limit minutes component.
        val = 59;
      }

      offsetComp[segPos] = val;

      // Move to the next component of the time zone.
      if(segPos < 2)
      {
        segPos++;
        bufPos = 0;
        isSegmentCont = true;
        Serial.print(F(":"));
      }      
      else
      {
        // End of time zone data.
        isZoneCont = false;
        break;
      }      
    }
  }

  // Convert specified time zone offset into seconds.
  returnVal = (offsetComp[2] + (offsetComp[1] * 60L)) * 60L;
  if(offsetComp[0] == 0x2D)
  {
    returnVal *= -1L;
  }

  return returnVal;
}

void rebootSystem()
{
  delay(500);
  
  // Reset ENC28J60 ethernet controller.  
  resetEthernet();

  // Reset ATMEGA328 MCU.
  wdt_disable();
  wdt_enable(WDTO_15MS);
  while(1);
}
