EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
Title "NTP clock driver connections"
Date "2021-04-27"
Rev "1.0.0"
Comp "Dilshan R Jayakody"
Comment1 "jayakody2000lk@gmail.com"
Comment2 "jayakody2000lk.blogspot.com"
Comment3 "https://github.com/dilshan/ntp-clock"
Comment4 ""
$EndDescr
$Comp
L Connector_Generic:Conn_02x04_Odd_Even J?
U 1 1 6096421B
P 5700 3500
AR Path="/6096421B" Ref="J?"  Part="1" 
AR Path="/609616AE/6096421B" Ref="J3"  Part="1" 
F 0 "J3" H 5750 3817 50  0000 C CNN
F 1 "CNTRL" H 5750 3726 50  0000 C CNN
F 2 "" H 5700 3500 50  0001 C CNN
F 3 "~" H 5700 3500 50  0001 C CNN
	1    5700 3500
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D?
U 1 1 60964221
P 5750 2850
AR Path="/60964221" Ref="D?"  Part="1" 
AR Path="/609616AE/60964221" Ref="D2"  Part="1" 
F 0 "D2" H 5743 3067 50  0000 C CNN
F 1 "YELLOW - 5mm LED" H 5743 2976 50  0000 C CNN
F 2 "" H 5750 2850 50  0001 C CNN
F 3 "~" H 5750 2850 50  0001 C CNN
	1    5750 2850
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D?
U 1 1 60964227
P 5750 2250
AR Path="/60964227" Ref="D?"  Part="1" 
AR Path="/609616AE/60964227" Ref="D1"  Part="1" 
F 0 "D1" H 5743 2467 50  0000 C CNN
F 1 "GREEN - 5mm LED" H 5743 2376 50  0000 C CNN
F 2 "" H 5750 2250 50  0001 C CNN
F 3 "~" H 5750 2250 50  0001 C CNN
	1    5750 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 3400 5300 3400
Wire Wire Line
	5300 3400 5300 2850
Wire Wire Line
	5300 2850 5600 2850
Wire Wire Line
	5500 3500 5150 3500
Wire Wire Line
	5150 3500 5150 2250
Wire Wire Line
	5150 2250 5600 2250
$Comp
L Switch:SW_Push SW?
U 1 1 60964239
P 5750 4400
AR Path="/60964239" Ref="SW?"  Part="1" 
AR Path="/609616AE/60964239" Ref="SW1"  Part="1" 
F 0 "SW1" H 5750 4685 50  0000 C CNN
F 1 "12.5mm Push Switch" H 5750 4594 50  0000 C CNN
F 2 "" H 5750 4600 50  0001 C CNN
F 3 "~" H 5750 4600 50  0001 C CNN
	1    5750 4400
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 6096423F
P 5750 5050
AR Path="/6096423F" Ref="SW?"  Part="1" 
AR Path="/609616AE/6096423F" Ref="SW2"  Part="1" 
F 0 "SW2" H 5750 5335 50  0000 C CNN
F 1 "12.5mm Push Switch" H 5750 5244 50  0000 C CNN
F 2 "" H 5750 5250 50  0001 C CNN
F 3 "~" H 5750 5250 50  0001 C CNN
	1    5750 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 3700 6200 3700
Wire Wire Line
	6200 3700 6200 4400
Wire Wire Line
	6200 4400 5950 4400
Wire Wire Line
	5500 3700 5300 3700
Wire Wire Line
	5300 3700 5300 4400
Wire Wire Line
	5300 4400 5550 4400
Wire Wire Line
	6000 3600 6350 3600
Wire Wire Line
	6350 3600 6350 5050
Wire Wire Line
	6350 5050 5950 5050
Wire Wire Line
	5500 3600 5150 3600
Wire Wire Line
	5150 3600 5150 5050
Wire Wire Line
	5150 5050 5550 5050
Text Notes 5650 3050 0    50   ~ 0
BUSY
Text Notes 5650 2450 0    50   ~ 0
SYNC
Text Notes 5600 5200 0    50   ~ 0
DATE/TIME
Text Notes 5650 4550 0    50   ~ 0
SYNC
Wire Wire Line
	6200 3400 6200 2850
Wire Wire Line
	6000 3400 6200 3400
Wire Wire Line
	5900 2850 6200 2850
Wire Wire Line
	6350 2250 6350 3500
Wire Wire Line
	6000 3500 6350 3500
Wire Wire Line
	5900 2250 6350 2250
$EndSCHEMATC
