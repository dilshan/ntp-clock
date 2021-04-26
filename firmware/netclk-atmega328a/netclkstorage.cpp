//---------------------------------------------------------------------------------------
// Ethernet base NTP clock panel driver.
// EEPROM related functions and data structures.
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
#include <EEPROM.h>

#include "netclkstorage.h"

void configWriteNetworkType(unsigned char netType)
{
  EEPROM.write(0x00, netType);
}

void configWriteDeviceIP(unsigned char *ip)
{
  EEPROM.write(0x01, ip[0]);
  EEPROM.write(0x02, ip[1]);
  EEPROM.write(0x03, ip[2]);
  EEPROM.write(0x04, ip[3]);
}

void configWriteSubnetMask(unsigned char *ip)
{
  EEPROM.write(0x05, ip[0]);
  EEPROM.write(0x06, ip[1]);
  EEPROM.write(0x07, ip[2]);
  EEPROM.write(0x08, ip[3]);  
}

void configWriteGateway(unsigned char *ip)
{
  EEPROM.write(0x09, ip[0]);
  EEPROM.write(0x0A, ip[1]);
  EEPROM.write(0x0B, ip[2]);
  EEPROM.write(0x0C, ip[3]); 
}

void configRead(SystemConfig *config)
{
  // Get network type.
  config->networkType = EEPROM.read(0x00);

  // Load device IP address.
  config->deviceIP[0] = EEPROM.read(0x01);
  config->deviceIP[1] = EEPROM.read(0x02);
  config->deviceIP[2] = EEPROM.read(0x03);
  config->deviceIP[3] = EEPROM.read(0x04);

  // Load subnet mask.
  config->netmask[0] = EEPROM.read(0x05);
  config->netmask[1] = EEPROM.read(0x06);
  config->netmask[2] = EEPROM.read(0x07);
  config->netmask[3] = EEPROM.read(0x08);  

  // Load gateway IP address.
  config->gateway[0] = EEPROM.read(0x09);
  config->gateway[1] = EEPROM.read(0x0A);
  config->gateway[2] = EEPROM.read(0x0B);
  config->gateway[3] = EEPROM.read(0x0C);   
}

void configureNTPInfo(unsigned char *ip, unsigned char port)
{
  // Store NTP server IP address.
  EEPROM.write(0x0D, ip[0]);
  EEPROM.write(0x0E, ip[1]);
  EEPROM.write(0x0F, ip[2]);
  EEPROM.write(0x10, ip[3]); 

  // Store NTP server port.
  EEPROM.write(0x11, port); 
}

void readNTPConfigInfo(unsigned char *ip, unsigned char *port)
{
  // Load NTP server IP address.
  ip[0] = EEPROM.read(0x0D);
  ip[1] = EEPROM.read(0x0E);
  ip[2] = EEPROM.read(0x0F);
  ip[3] = EEPROM.read(0x10);

  // Load NTP server port.
  *port = EEPROM.read(0x11);
}

void writeTimeOffset(long tzOffset)
{
  EEPROM.write(0x12, (tzOffset & 0xFF));
  EEPROM.write(0x13, ((tzOffset >> 8) & 0xFF));
  EEPROM.write(0x14, ((tzOffset >> 16) & 0xFF));
  EEPROM.write(0x15, ((tzOffset >> 24) & 0xFF));
}

long readTimeOffset()
{
  long numComp[4];
  
  numComp[3] = EEPROM.read(0x12);
  numComp[2] = EEPROM.read(0x13);
  numComp[1] = EEPROM.read(0x14);
  numComp[0] = EEPROM.read(0x15);
 
  return ((numComp[3] << 0) & 0xFF) + ((numComp[2] << 8) & 0xFFFF) + 
    ((numComp[1] << 16) & 0xFFFFFF) + ((numComp[0] << 24) & 0xFFFFFFFF);
}

void writeDisplayFormat(bool displayFmt)
{
  EEPROM.write(0x16, displayFmt ? 0xFF : 0x00);
}

bool readDisplayFormat()
{
  return (EEPROM.read(0x16) == 0xFF);
}