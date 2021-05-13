# NTP based digital clock panel

![Prototype version of NTP clock driver](https://raw.githubusercontent.com/dilshan/ntp-clock/main/resources/netclk-main.jpg)

This project introduces an open-source, *ATmega328* based, configurable [NTP](https://en.wikipedia.org/wiki/Network_Time_Protocol) clock with a 2.3-inch, 7-segment display driver. This clock automatically obtains time from the configured NTP server and updates the date and time of the built-in RTC (*real-time clock*). If the connection to the NTP server is lost, the clock continues to run using its built-in RTC.

All the parameters of this clock can configure using its USB base serial terminal. The firmware of this clock supports both *static* and *DHCP* addressing modes. Apart from that, parameters such as NTP server address, time-offset, and clock display formats can change through the menu-driven configuration terminal.

This clock is designed to drive large common-anode, 7-segment displays. The prototype version is assembled using four individual segments of 7.2V, 2.3-inch, red color displays (*FJS23101BH*). To archive, the necessary high output voltage and the current, the 7-segment display output stage of this clock is formed using *ULN2803* Darlington transistor array and *BC858* transistors.

The network component of this clock is built around the *ENC28J60* 10BASE-T stand-alone Ethernet controller.

Highly accurate *DS3231M* RTC is using in this clock for time-keeping operations. To backup the date and time, the *CR1220* type button cell is included in the PCB. The date and time are syncs with the NTP server during the system startup. Users can manually initiate synchronization by pressing the "*SYNC*" button in the system.

Depending on the NTP server, the user needs to specify the time offset to the system to get the correct local time. Time offset can configure using the system configuration menu, and it must be input in a standard format like *+5:30*.

Firmware of this clock is developed using the [Arduino framework](https://www.arduino.cc/en/software) and libraries.

[**PCBWay**](https://www.pcbway.com) provides sponsorship for this project. The [PCBWay online ordering service](https://www.pcbway.com/orderonline.aspx) provides excellent help to fabricate this PCB. We got finished PCBs within three days, and the quality of the service is at a superior level. PCB of this project can directly [order from PCBWay through this link](https://www.pcbway.com/project/shareproject/NTP_based_digital_clock_panel.html).

This project is a [certified](https://certification.oshwa.org/lk000009.html) open-source hardware project. All the content of this project are distributed under the terms of the following license:

- Hardware License: [CC BY 4.0](https://creativecommons.org/licenses/by/4.0/)
- Software and firmware License: [MIT](https://github.com/dilshan/ntp-clock/blob/main/LICENSE)
- [Documentation](https://github.com/dilshan/ntp-clock/wiki) License: [CC BY 4.0](https://creativecommons.org/licenses/by/4.0/)

[![LK000009](https://raw.githubusercontent.com/wiki/dilshan/ntp-clock/images/OSHW_mark_LK000009.png)](https://certification.oshwa.org/lk000009.html)
