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

#ifndef NETCLK_STORAGE
#define NETCLK_STORAGE

#define NET_TYPE_DHCP   0x00
#define NET_TYPE_STATIC 0x01

typedef struct 
{
  byte networkType;   // Type of the network, such as DHCP or static IP.
  byte deviceIP[4];   // Static IP address of the device.
  byte netmask[4];    // Subnet mask.
  byte gateway[4];    // IP address of the gateway.
} SystemConfig;

void EEPROMWrite(unsigned char devAddr, unsigned char memAddr, unsigned char data);
unsigned char EEPROMRead(unsigned char devAddr, unsigned char memAddr);

void configRead(SystemConfig *config);
void configWriteNetworkType(unsigned char netType);
void configWriteDeviceIP(unsigned char *ip);
void configWriteSubnetMask(unsigned char *ip);
void configWriteGateway(unsigned char *ip);

void configureNTPInfo(unsigned char *ip, unsigned char port);
void readNTPConfigInfo(unsigned char *ip, unsigned char *port);

void writeTimeOffset(long tzOffset);
long readTimeOffset();

void writeDisplayFormat(bool displayFmt);
bool readDisplayFormat();

#endif
