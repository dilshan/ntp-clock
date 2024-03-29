//---------------------------------------------------------------------------------------
// Ethernet base NTP clock panel driver.
// Main Arduino sketch.
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

#include <Wire.h>
#include <EtherCard.h>
#include <ds3231.h>

#include "netclkdisplay.h"
#include "netclkintf.h"
#include "netclkstorage.h"
#include "netclkutil.h"

// Use NIST Internet Time Server (time-a-g.nist.gov).
#define DEFAULT_NTP_PORT  123
#define DEFAULT_NTP_SERVER  {129, 6, 15, 28}

// Unix timestamp for 1st of January, 1970 (UTC).
#define UNIX_TIMESTAMP  2208988800UL
#define BASE_YEAR       1970

#define LEAP_YEAR(Y)  (((BASE_YEAR+(Y))>0) && !((BASE_YEAR+(Y))%4) && (((BASE_YEAR+(Y))%100) || !((BASE_YEAR+(Y))%400)))
#define NEW_LINE      Serial.println(F(" "))

// Device MAC address generated by  Ian Campbell's MAC address generator
// at https://www.hellion.org.uk/cgi-bin/randmac.pl.
static const unsigned char DEV_MAC[] = {0x3A, 0xF5, 0x42, 0x21, 0x96, 0x33};

// Number of days in each month.
static const unsigned char monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

bool isNetworkAvailable;
bool clock24hrFormat;
bool initalBoot;
static volatile bool updateReq;

unsigned char syncButtonState;
unsigned char dateButtonState;
unsigned char displayTimeout;

unsigned char ntpServer[] = DEFAULT_NTP_SERVER;
unsigned char ntpPort = DEFAULT_NTP_PORT;
unsigned char Ethernet::buffer[450];

static unsigned char displayVal[3];

long timeZoneOffset = 0L;

struct ts systime;
SystemConfig sysconf;
static volatile DISPLAY_MODE displayMode;

void resetSysConfig()
{
  SystemConfig resetConfig;
  const unsigned char resetNTPIP[] = DEFAULT_NTP_SERVER;

  setDefaultConfig(&resetConfig);

  // Network type is set to DHCP and set static IP configuration entries.
  configWriteNetworkType(resetConfig.networkType);
  configWriteDeviceIP(resetConfig.deviceIP);
  configWriteSubnetMask(resetConfig.netmask);
  configWriteGateway(resetConfig.gateway);

  // Set default NTP server configuration.
  configureNTPInfo(resetNTPIP, DEFAULT_NTP_PORT);

  // Reset time zone to 0:00.
  writeTimeOffset(0L);

  // Time display format is set to 24-hour.
  writeDisplayFormat(true);

  delay(100);
}

void initSystem()
{
  setDefaultConfig(&sysconf);

  // Setup seven segment display interface.
  initDisplay();

  // Setup serial communication interface with baud rate of 9600 and 8N1 configuration.
  Serial.begin(9600);
  Serial.println(F("Starting up the system..."));

  // Setup I2C interface.
  Wire.begin();

  // Setup PCF8574 I/O expander and it's I/O ports.
  delay(100);
  initInterface();
  setIndicator(IND_BUSY_CONFIG, true);

  // Check for factory reset input.
  if (!getFactorResetState())
  {
    // Perform configuration reset.
    Serial.println(F("Reseting system to default configuration..."));
    resetSysConfig();

    // Wait until user releases reset jumper.
    while (!getFactorResetState())
    {
      setIndicator(IND_BUSY_CONFIG, false);
      delay(250);
      setIndicator(IND_BUSY_CONFIG, true);
      delay(250);
    }

    // Perform system restart.
    rebootSystem();
  }

  // Load system configuration from EEPROM.
  Serial.println(F("Reading system configuration..."));
  configRead(&sysconf);
  readNTPConfigInfo(ntpServer, &ntpPort);

  // Validate NTP configuration.
  if ((ntpServer[0] == 0xFF) && (ntpServer[1] == 0xFF) && (ntpServer[2] == 0xFF) && (ntpServer[3] == 0xFF))
  {
    // EEPROM default is loaded. Fix NTP IP address and port to default values.
    Serial.println(F("Switching NTP server configuration to defaults..."));
    unsigned char tempDefaultIP[] = DEFAULT_NTP_SERVER;
    memcpy(ntpServer, tempDefaultIP, 4);
    ntpPort = DEFAULT_NTP_PORT;
  }

  // Setup ethernet controller.
  Serial.println(F("Connecting to network..."));
  ether.begin(sizeof Ethernet::buffer, DEV_MAC, SS);
}

void configureDHCP(SystemConfig *config)
{
  config->networkType = NET_TYPE_DHCP;
  configWriteNetworkType(NET_TYPE_DHCP);

  Serial.println(F("Configuration updated with DHCP settings."));
}

bool configureStaticIP(SystemConfig *config)
{
  // Prompt IP address, subnet mask and gatway IP address inputs to user.
  Serial.print(F("Device IP address: "));
  if (!inputIpAddress(config->deviceIP))
  {
    return false;
  }

  Serial.print(F("\n\n\rSubnet mask: "));
  if (!inputIpAddress(config->netmask))
  {
    return false;
  }

  Serial.print(F("\n\n\rGateway IP address: "));
  if (!inputIpAddress(config->gateway))
  {
    return false;
  }

  NEW_LINE;

  // Write configuration changes to EEPROM.
  config->networkType = NET_TYPE_STATIC;
  configWriteNetworkType(NET_TYPE_STATIC);
  configWriteDeviceIP(config->deviceIP);
  configWriteSubnetMask(config->netmask);
  configWriteGateway(config->gateway);

  Serial.println(F("Configuration updated with static IP address."));
  return true;
}

bool configureNTPServer(unsigned char* ntpIpAddr, unsigned char* ntpPort)
{
  unsigned char tempIp[4] = DEFAULT_NTP_SERVER;
  unsigned char tempPort = DEFAULT_NTP_PORT;

  int retryAttempt = 60;
  unsigned long ntpTempVal = 0;

  // Prompt NTP IP address.
  Serial.print(F("NTP server IP address: "));
  if (!inputIpAddress(tempIp))
  {
    return false;
  }

  // Prompt NTP port.
  Serial.print(F("\n\n\rNTP server port: "));
  if (!inputPort(&tempPort, DEFAULT_NTP_PORT))
  {
    return false;
  }

  // Verify specified NTP server.
  Serial.println(F("\n\n\rConnecting with the specified NTP server..."));
  while (retryAttempt > 0)
  {
    ether.ntpRequest(tempIp, tempPort);
    Serial.print(".");
    word length = ether.packetReceive();
    ether.packetLoop(length);
    if (length > 0 && ether.ntpProcessAnswer(&ntpTempVal, tempPort))
    {
      // NTP request is completed.
      Serial.println(F("\n\rReceived data from NTP server\n\r"));
      break;
    }

    delay(500);
    retryAttempt--;
  }

  // Validate response received from the request.
  if (ntpTempVal > 0)
  {
    // NTP request is successful.
    memcpy(ntpIpAddr, tempIp, 4);
    *ntpPort = tempPort;
    return true;
  }
  else
  {
    // NTP request is failed.
    Serial.println(F("\n\rNTP request is failed or invalid!\n\r"));
    return false;
  }
}

bool setDisplayFormat()
{
  bool isContinue = true;
  char inKey;

  while (isContinue)
  {
    inKey = 0;

    Serial.println(F("1. Display time in 24-hour format"));
    Serial.println(F("2. Display time in 12-hour format"));
    Serial.println("");
    Serial.print(F("Selection [1..2]: "));

    inKey = getKeyInput(0x30, 0x33);
    {
      if (inKey == 0x1B)
      {
        // User cancel the current operation.
        return false;
      }
    }

    Serial.println(inKey);

    // Allow key option 1 or 2 only.
    isContinue = !((inKey > 0x30) && (inKey < 0x33));
  }

  clock24hrFormat = (inKey == 0x31);
  return true;
}

void systemMenu()
{
  bool isContinue = true;
  char inKey;
  long tempTimeZone;

  // Initialize menu system and start menu service loop.
  setIndicator(IND_BUSY_CONFIG, true);
  NEW_LINE;

  while (isContinue)
  {
    inKey = 0;

    Serial.println(F("1. Configure for DHCP network"));
    Serial.println(F("2. Configure with static IP address"));
    Serial.println(F("3. Show network configuration"));
    Serial.println(F("4. Synchronize date/time"));
    Serial.println(F("5. Set time offset"));
    Serial.println(F("6. Show current date/time"));
    Serial.println(F("7. Configure NTP server"));
    Serial.println(F("8. Configure clock display format"));
    Serial.println(F("9. Exit"));
    Serial.println(F("0. Exit and restart"));
    Serial.println("");
    Serial.print(F("Selection [0..9]: "));

    inKey = getKeyInput(0x2F, 0x3A);

    if ((inKey > 0x2F) && (inKey < 0x3A))
    {
      Serial.println(inKey);
      NEW_LINE;
    }

    switch (inKey)
    {
      case 0x31:
        // Configure for DHCP network.
        configureDHCP(&sysconf);
        Serial.println(F("Restarting the system to apply the network changes..."));
        rebootSystem();
        break;
      case 0x32:
        // Configure with static IP address.
        if (configureStaticIP(&sysconf))
        {
          // Static IP configuration is completed.
          Serial.println(F("Restarting the system to apply the network changes..."));
          rebootSystem();
        }
        else
        {
          // Static IP configuration is canceled by the user.
          Serial.println(F("\n\n"));
        }
        break;
      case 0x33:
        // Show network configuration.
        showNetworkInfo();
        break;
      case 0x34:
        // Synchronize date/time.
        clearDisplay();
        syncSystemTime();
        NEW_LINE;
        break;
      case 0x35:
        // Set time offset.
        Serial.print(F("Time offset relative to UTC/GMT [e.g: +5:30]: "));
        tempTimeZone = inputTimeZone();
        NEW_LINE;
        if (tempTimeZone != 1)
        {
          // Time zone configuration is completed.
          timeZoneOffset = tempTimeZone;
          writeTimeOffset(timeZoneOffset);
          Serial.println(F("Restarting the system to apply the time offset change..."));
          rebootSystem();
        }
        else
        {
          // Time zone configuration is canceled by the user.
          NEW_LINE;
        }
        break;
      case 0x36:
        // Show current date/time.
        printCurrentTimeConfig();
        NEW_LINE;
        break;
      case 0x37:
        // Configure NTP server.
        if (configureNTPServer(ntpServer, &ntpPort))
        {
          // NTP server configuration is completed.
          configureNTPInfo(ntpServer, ntpPort);
        }
        else
        {
          // NTP server configuration is canceled by the user.
          Serial.println(F("\n\n"));
        }
        break;
      case 0x38:
        // Configure clock display format.
        if (setDisplayFormat())
        {
          writeDisplayFormat(clock24hrFormat);
        }
        NEW_LINE;
        break;
      case 0x39:
      case 0x1B:
        // Exit.
        isContinue = false;
        break;
      case 0x30:
        // Exit and restart.
        Serial.println("");
        rebootSystem();
        break;
    }
  }

  // Leaving from configuration menu system.
  NEW_LINE;
  updateReq = true;
  displayMode = DISP_TIME;
  setIndicator(IND_BUSY_CONFIG, false);
}

unsigned long getTimeFromServer()
{
  unsigned long timeFromNTP;

  int retryAttempt = 60;
  while (retryAttempt > 0)
  {
    ether.ntpRequest(ntpServer, ntpPort);
    Serial.print(".");
    word length = ether.packetReceive();
    ether.packetLoop(length);
    if (length > 0 && ether.ntpProcessAnswer(&timeFromNTP, ntpPort))
    {
      // NTP request is successful.
      Serial.println();
      Serial.println(F("Received time from server"));
      return timeFromNTP - UNIX_TIMESTAMP + timeZoneOffset;
    }

    delay(500);
    retryAttempt--;
  }

  return 0;
}

void epochToTime(unsigned long time, struct ts* outTime)
{
  unsigned char year;
  unsigned char month, monthLength;
  unsigned long days;

  // Extract time components from specified data.
  outTime->sec = time % 60;
  time /= 60;

  outTime->min = time % 60;
  time /= 60;

  outTime->hour = time % 24;
  time /= 24;

  year = 0;
  days = 0;

  // Calculate year from 1970.
  while ((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time)
  {
    year++;
  }

  outTime->year = year + BASE_YEAR;

  days -= LEAP_YEAR(year) ? 366 : 365;
  time -= days;

  days = 0;
  monthLength = 0;

  for (month = 0; month < 12; month++)
  {
    if (month == 1)
    {
      // Determine days in month of february based on leap year.
      monthLength = LEAP_YEAR(year) ? 29 : 28;
    }
    else
    {
      monthLength = monthDays[month];
    }

    if (time >= monthLength)
    {
      time -= monthLength;
    }
    else
    {
      break;
    }
  }

  outTime->mon = month + 1;
  outTime->mday = time + 1;
}

void showNetworkInfo()
{
  // Show status of the network connectivity.
  Serial.print(F("Network connection: "));
  if (isNetworkAvailable)
  {
    // Network connection is active.
    Serial.println(F("UP"));
    ether.printIp("Device IP address: ", ether.myip);
    ether.printIp("Netmask: ", ether.netmask);
    ether.printIp("Gateway IP address: ", ether.gwip);
    ether.printIp("DNS IP address: ", ether.dnsip);
  }
  else
  {
    // Network connection is down!
    Serial.println(F("DOWN"));
  }

  ether.printIp("NTP server: ", ntpServer);
  Serial.print(F("NTP server port: "));
  Serial.println(ntpPort);
  NEW_LINE;
}

void syncSystemTime()
{
  setIndicator(IND_SYNC, true);
  Serial.println(F("Trying to synchronize time with server..."));

  // Create DHCP or static IP based network connection.
  if (!isNetworkAvailable)
  {
    if (!initalBoot)
    {
      // If network is not available we need to restart the ENC28J64 controller.
      Serial.println(F("Connecting to network..."));
      resetEthernet();
      delay(1000);
      ether.begin(sizeof Ethernet::buffer, DEV_MAC, SS);
      delay(200);
    }

    // Try to create network connection.
    if (sysconf.networkType == NET_TYPE_DHCP)
    {
      // Create DHCP network connection.
      Serial.println(F("Requesting DHCP network connection"));
      isNetworkAvailable = ether.dhcpSetup();
      if (!isNetworkAvailable)
      {
        Serial.println(F("** Fail to create network connection! **"));
      }
    }
    else
    {
      // Create network connection with specified static IP.
      Serial.println(F("Configuring static IP connection"));
      isNetworkAvailable = ether.staticSetup(sysconf.deviceIP, sysconf.gateway, NULL, sysconf.netmask);
    }

    ether.printIp("- Device IP address: ", ether.myip);
    ether.printIp("- Netmask: ", ether.netmask);
    ether.printIp("- Gateway IP address: ", ether.gwip);
    ether.printIp("- DNS IP address: ", ether.dnsip);
  }

  // Sync RTC time using NTP server.
  if (isNetworkAvailable)
  {
    Serial.println(F("Establishing connection with time server..."));
    unsigned long tempTime = getTimeFromServer();

    if (tempTime != 0)
    {
      // Time sync is successful.
      epochToTime(tempTime, &systime);

      // Sync. RTC time with NTP time.
      DS3231_set(systime);
      Serial.println(F("Real-time clock is now synchronized with server time"));
    }
    else
    {
      // Time sync is fail due to timeout, network issue or configuration issues.
      Serial.println(F("\n\rUnable to synchronize time with specified server!"));
      isNetworkAvailable = false;
    }
  }
  else
  {
    // Time sync is fail due to network connection issues.
    Serial.println(F("Time server synchronize failed due to network error!"));
  }

  setIndicator(IND_SYNC, false);
}

void printCurrentTimeConfig()
{
  DS3231_get(&systime);

  // Print current system time.
  Serial.print(F("Time: "));
  Serial.print(systime.hour);
  Serial.print(F(":"));
  Serial.print(systime.min);
  Serial.print(F(":"));
  Serial.print(systime.sec);
  NEW_LINE;

  // Print current system date.
  Serial.print(F("Date: "));
  Serial.print(systime.year);
  Serial.print(F("/"));
  Serial.print(systime.mon);
  Serial.print(F("/"));
  Serial.print(systime.mday);
  NEW_LINE;
}

void setup()
{
  // Initialize global variables.
  initalBoot = true;
  isNetworkAvailable = false;
  updateReq = false;
  clock24hrFormat = true;

  displayVal[0] = 0;
  displayVal[1] = 0;
  displayVal[2] = 0;

  syncButtonState = 0;
  dateButtonState = 0;

  displayMode = DISP_TIME;

  // Initializing the system.
  initSystem();

  // Initializing RTC.
  Serial.println(F("Configuring and starting real-time clock"));
  DS3231_init(DS3231_INTCN);

  // Get time offset from configuration.
  timeZoneOffset = readTimeOffset();
  if (timeZoneOffset == -1)
  {
    timeZoneOffset = 0L;
  }

  syncSystemTime();
  delay(100);

  // Get time from RTC.
  Serial.println(F("Receiving time from real-time clock..."));
  DS3231_get(&systime);

  // Check RTC values to detect RTC battery status.
  if (systime.year == 1900)
  {
    // RTC reset detetced. Set default time in RTC.
    systime.mday = 1;
    systime.mon = 1;
    systime.year = 2020;
    systime.hour = 1;
    systime.min = 0;
    systime.sec = 0;

    DS3231_set(systime);
  }

  Serial.print(systime.mday);
  Serial.print(F("-"));
  Serial.print(systime.mon);
  Serial.print(F("-"));
  Serial.print(systime.year);
  Serial.print(F("   "));
  Serial.print(systime.hour);
  Serial.print(F(":"));
  Serial.print(systime.min);
  Serial.print(F(":"));
  Serial.println(systime.sec);

  // Get time display format.
  clock24hrFormat = readDisplayFormat();

  // Show current time in seven segment display.
  displayVal[0] = formatHours(clock24hrFormat, systime.hour);
  displayVal[1] = systime.min;
  displayVal[2] = systime.sec;

  // System initialization is completed.
  setIndicator(IND_BUSY_CONFIG, false);
  delay(100);
  activateRefreshTimer();
  initalBoot = false;
}

void loop()
{
  ether.packetLoop(ether.packetReceive());

  // Check status of the serial input buffer.
  if (Serial.available() > 0)
  {
    // System menu is accessible using <ENTER> or <SPACE> keys.
    unsigned char inKey = Serial.read();
    if ((inKey == 0x0D) || (inKey == 0x20))
    {
      // Open system configuration menu.
      clearDisplay();
      systemMenu();
    }
  }

  // Check for time update flag.
  if (updateReq)
  {
    updateReq = false;
    DS3231_get(&systime);

    switch (displayMode)
    {
      case DISP_TIME:
        // Display time;
        displayVal[0] = formatHours(clock24hrFormat, systime.hour);
        displayVal[1] = systime.min;
        displayVal[2] = systime.sec;
        break;
      case DISP_DATE:
        // Display day and month.
        displayVal[0] = systime.mon;
        displayVal[1] = systime.mday;
        displayVal[2] = 0;
        break;
      case DISP_SECONDS:
        // Display seconds.
        displayVal[0] = 0xFF;
        displayVal[1] = systime.sec;
        displayVal[2] = 0;
        break;
    }
  }

  // Update seven segment display with current time.
  if ((displayMode == DISP_TIME) || (displayMode == DISP_DATE))
  {
    displayTime(displayVal);
  }
  else
  {
    displaySeconds(displayVal);
  }

  // Check for SYNC button press event.
  if (!getSyncButtonState())
  {
    syncButtonState += (syncButtonState < 0xFF) ? 1 : 0;
  }
  else
  {
    if (syncButtonState > 0x10)
    {
      syncButtonState = 0;
      displayTimeout = 0;

      // Start time sync with NTP server.
      clearDisplay();
      syncSystemTime();

      // Refresh and show time in next render cycle.
      updateReq = true;
      displayMode = DISP_TIME;
    }
  }

  // Check for DATE/TIME button press event.
  if (!getDateButtonState())
  {
    dateButtonState++;

    if (dateButtonState >= 0xFE)
    {
      // In long press display seconds.
      dateButtonState = 1;
      displayTimeout = 0;

      displayVal[0] = 0xFF;
      displayVal[1] = systime.sec;
      displayVal[2] = 0;

      displayMode = DISP_SECONDS;
    }
  }
  else
  {
    // DATE/TIME button press event handler.
    if (dateButtonState > 0x10)
    {
      dateButtonState = 0;
      displayTimeout = 0;

      if (displayMode == DISP_TIME)
      {
        // Show current day and month in seven segement display.
        displayVal[0] = systime.mon;
        displayVal[1] = systime.mday;
        displayVal[2] = 0;

        displayMode = DISP_DATE;
      }
      else
      {
        // Return display to default (time) mode.
        displayVal[0] = formatHours(clock24hrFormat, systime.hour);
        displayVal[1] = systime.min;
        displayVal[2] = systime.sec;

        displayMode = DISP_TIME;
      }
    }
  }
}

ISR(TIMER1_COMPA_vect)
{
  // Update time from RTC.
  updateReq = true;

  // Check current display mode.
  if (displayMode == DISP_DATE)
  {
    // Clear date display in 10 seconds.
    displayTimeout++;

    if (displayTimeout >= 10)
    {
      displayTimeout = 0;
      displayMode = DISP_TIME;
    }
  }
}
