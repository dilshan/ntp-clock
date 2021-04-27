EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 3
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
L Connector_Generic:Conn_01x08 J?
U 1 1 6095C345
P 9250 4450
AR Path="/6095C345" Ref="J?"  Part="1" 
AR Path="/60957C2B/6095C345" Ref="J2"  Part="1" 
F 0 "J2" H 9330 4442 50  0000 L CNN
F 1 "SSD-DATA" H 9330 4351 50  0000 L CNN
F 2 "" H 9250 4450 50  0001 C CNN
F 3 "~" H 9250 4450 50  0001 C CNN
	1    9250 4450
	1    0    0    -1  
$EndComp
$Comp
L Display_Character:SA15-11GWA U?
U 1 1 6095C34B
P 3150 3550
AR Path="/6095C34B" Ref="U?"  Part="1" 
AR Path="/60957C2B/6095C34B" Ref="U1"  Part="1" 
F 0 "U1" H 3150 4217 50  0000 C CNN
F 1 "FJS23101BH" H 3150 4126 50  0000 C CNN
F 2 "Display_7Segment:SA15-11xxx" H 3150 3500 50  0001 C CNN
F 3 "http://www.kingbrightusa.com/images/catalog/SPEC/SA15-11GWA.pdf" H 3150 3500 50  0001 C CNN
	1    3150 3550
	1    0    0    -1  
$EndComp
$Comp
L Display_Character:SA15-11GWA U?
U 1 1 6095C351
P 4750 3550
AR Path="/6095C351" Ref="U?"  Part="1" 
AR Path="/60957C2B/6095C351" Ref="U2"  Part="1" 
F 0 "U2" H 4750 4217 50  0000 C CNN
F 1 "FJS23101BH" H 4750 4126 50  0000 C CNN
F 2 "Display_7Segment:SA15-11xxx" H 4750 3500 50  0001 C CNN
F 3 "http://www.kingbrightusa.com/images/catalog/SPEC/SA15-11GWA.pdf" H 4750 3500 50  0001 C CNN
	1    4750 3550
	1    0    0    -1  
$EndComp
$Comp
L Display_Character:SA15-11GWA U?
U 1 1 6095C357
P 6350 3550
AR Path="/6095C357" Ref="U?"  Part="1" 
AR Path="/60957C2B/6095C357" Ref="U3"  Part="1" 
F 0 "U3" H 6350 4217 50  0000 C CNN
F 1 "FJS23101BH" H 6350 4126 50  0000 C CNN
F 2 "Display_7Segment:SA15-11xxx" H 6350 3500 50  0001 C CNN
F 3 "http://www.kingbrightusa.com/images/catalog/SPEC/SA15-11GWA.pdf" H 6350 3500 50  0001 C CNN
	1    6350 3550
	1    0    0    -1  
$EndComp
$Comp
L Display_Character:SA15-11GWA U?
U 1 1 6095C35D
P 7950 3550
AR Path="/6095C35D" Ref="U?"  Part="1" 
AR Path="/60957C2B/6095C35D" Ref="U4"  Part="1" 
F 0 "U4" H 7950 4217 50  0000 C CNN
F 1 "FJS23101BH" H 7950 4126 50  0000 C CNN
F 2 "Display_7Segment:SA15-11xxx" H 7950 3500 50  0001 C CNN
F 3 "http://www.kingbrightusa.com/images/catalog/SPEC/SA15-11GWA.pdf" H 7950 3500 50  0001 C CNN
	1    7950 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	9050 4150 7550 4150
Wire Wire Line
	7550 4150 7550 3250
Wire Wire Line
	7550 3250 7650 3250
Wire Wire Line
	7550 4150 5950 4150
Wire Wire Line
	5950 4150 5950 3250
Wire Wire Line
	5950 3250 6050 3250
Connection ~ 7550 4150
Wire Wire Line
	5950 4150 4350 4150
Wire Wire Line
	4350 4150 4350 3250
Wire Wire Line
	4350 3250 4450 3250
Connection ~ 5950 4150
Wire Wire Line
	4350 4150 2750 4150
Wire Wire Line
	2750 4150 2750 3250
Wire Wire Line
	2750 3250 2850 3250
Connection ~ 4350 4150
Wire Wire Line
	9050 4250 7450 4250
Wire Wire Line
	7450 4250 7450 3350
Wire Wire Line
	7450 3350 7650 3350
Wire Wire Line
	7450 4250 5850 4250
Wire Wire Line
	5850 4250 5850 3350
Wire Wire Line
	5850 3350 6050 3350
Connection ~ 7450 4250
Wire Wire Line
	5850 4250 4250 4250
Wire Wire Line
	4250 4250 4250 3350
Wire Wire Line
	4250 3350 4450 3350
Connection ~ 5850 4250
Wire Wire Line
	4250 4250 2650 4250
Wire Wire Line
	2650 4250 2650 3350
Wire Wire Line
	2650 3350 2850 3350
Connection ~ 4250 4250
Wire Wire Line
	9050 4350 7350 4350
Wire Wire Line
	7350 4350 7350 3450
Wire Wire Line
	7350 3450 7650 3450
Wire Wire Line
	7350 4350 5750 4350
Wire Wire Line
	5750 4350 5750 3450
Wire Wire Line
	5750 3450 6050 3450
Connection ~ 7350 4350
Wire Wire Line
	5750 4350 4150 4350
Wire Wire Line
	4150 4350 4150 3450
Wire Wire Line
	4150 3450 4450 3450
Connection ~ 5750 4350
Wire Wire Line
	4150 4350 2550 4350
Wire Wire Line
	2550 4350 2550 3450
Wire Wire Line
	2550 3450 2850 3450
Connection ~ 4150 4350
Wire Wire Line
	9050 4450 7250 4450
Wire Wire Line
	7250 4450 7250 3550
Wire Wire Line
	7250 3550 7650 3550
Wire Wire Line
	7250 4450 5650 4450
Wire Wire Line
	5650 4450 5650 3550
Wire Wire Line
	5650 3550 6050 3550
Connection ~ 7250 4450
Wire Wire Line
	5650 4450 4050 4450
Wire Wire Line
	4050 4450 4050 3550
Wire Wire Line
	4050 3550 4450 3550
Connection ~ 5650 4450
Wire Wire Line
	4050 4450 2450 4450
Wire Wire Line
	2450 4450 2450 3550
Wire Wire Line
	2450 3550 2850 3550
Connection ~ 4050 4450
Wire Wire Line
	9050 4550 7150 4550
Wire Wire Line
	7150 4550 7150 3650
Wire Wire Line
	7150 3650 7650 3650
Wire Wire Line
	7150 4550 5550 4550
Wire Wire Line
	5550 4550 5550 3650
Wire Wire Line
	5550 3650 6050 3650
Connection ~ 7150 4550
Wire Wire Line
	5550 4550 3950 4550
Wire Wire Line
	3950 4550 3950 3650
Wire Wire Line
	3950 3650 4450 3650
Connection ~ 5550 4550
Wire Wire Line
	3950 4550 2350 4550
Wire Wire Line
	2350 4550 2350 3650
Wire Wire Line
	2350 3650 2850 3650
Connection ~ 3950 4550
Wire Wire Line
	9050 4650 7050 4650
Wire Wire Line
	7050 4650 7050 3750
Wire Wire Line
	7050 3750 7650 3750
Wire Wire Line
	7050 4650 5450 4650
Wire Wire Line
	5450 4650 5450 3750
Wire Wire Line
	5450 3750 6050 3750
Connection ~ 7050 4650
Wire Wire Line
	5450 4650 3850 4650
Wire Wire Line
	3850 4650 3850 3750
Wire Wire Line
	3850 3750 4450 3750
Connection ~ 5450 4650
Wire Wire Line
	3850 4650 2250 4650
Wire Wire Line
	2250 4650 2250 3750
Wire Wire Line
	2250 3750 2850 3750
Connection ~ 3850 4650
Wire Wire Line
	9050 4750 6950 4750
Wire Wire Line
	6950 4750 6950 3850
Wire Wire Line
	6950 3850 7650 3850
Wire Wire Line
	6950 4750 5350 4750
Wire Wire Line
	5350 4750 5350 3850
Wire Wire Line
	5350 3850 6050 3850
Connection ~ 6950 4750
Wire Wire Line
	5350 4750 3750 4750
Wire Wire Line
	3750 4750 3750 3850
Wire Wire Line
	3750 3850 4450 3850
Connection ~ 5350 4750
Wire Wire Line
	3750 4750 2150 4750
Wire Wire Line
	2150 4750 2150 3850
Wire Wire Line
	2150 3850 2850 3850
Connection ~ 3750 4750
Wire Wire Line
	9050 4850 6850 4850
Wire Wire Line
	6850 4850 6850 3950
Wire Wire Line
	6850 3950 7650 3950
Wire Wire Line
	6850 4850 5250 4850
Wire Wire Line
	5250 4850 5250 3950
Wire Wire Line
	5250 3950 6050 3950
Connection ~ 6850 4850
Wire Wire Line
	5250 4850 3650 4850
Wire Wire Line
	3650 4850 3650 3950
Wire Wire Line
	3650 3950 4450 3950
Connection ~ 5250 4850
Wire Wire Line
	3650 4850 2050 4850
Wire Wire Line
	2050 4850 2050 3950
Wire Wire Line
	2050 3950 2850 3950
Connection ~ 3650 4850
$Comp
L Connector_Generic:Conn_01x04 J?
U 1 1 6095C3DB
P 9250 2500
AR Path="/6095C3DB" Ref="J?"  Part="1" 
AR Path="/60957C2B/6095C3DB" Ref="J1"  Part="1" 
F 0 "J1" H 9330 2492 50  0000 L CNN
F 1 "SSD-SEG" H 9330 2401 50  0000 L CNN
F 2 "" H 9250 2500 50  0001 C CNN
F 3 "~" H 9250 2500 50  0001 C CNN
	1    9250 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 3950 3550 3950
Wire Wire Line
	3550 3950 3550 3850
Wire Wire Line
	3550 3850 3450 3850
Wire Wire Line
	5050 3850 5150 3850
Wire Wire Line
	5150 3850 5150 3950
Wire Wire Line
	5150 3950 5050 3950
Wire Wire Line
	6650 3850 6750 3850
Wire Wire Line
	6750 3850 6750 3950
Wire Wire Line
	6750 3950 6650 3950
Wire Wire Line
	8250 3850 8350 3850
Wire Wire Line
	8350 3850 8350 3950
Wire Wire Line
	8350 3950 8250 3950
Wire Wire Line
	3550 3850 3550 2400
Wire Wire Line
	3550 2400 9050 2400
Connection ~ 3550 3850
Wire Wire Line
	5150 3850 5150 2500
Wire Wire Line
	5150 2500 9050 2500
Connection ~ 5150 3850
Wire Wire Line
	6750 3850 6750 2600
Wire Wire Line
	6750 2600 9050 2600
Connection ~ 6750 3850
Wire Wire Line
	8350 3850 8350 2700
Wire Wire Line
	8350 2700 9050 2700
Connection ~ 8350 3850
Text Label 8550 4150 0    50   ~ 0
A
Text Label 8550 4250 0    50   ~ 0
B
Text Label 8550 4350 0    50   ~ 0
C
Text Label 8550 4450 0    50   ~ 0
D
Text Label 8550 4550 0    50   ~ 0
E
Text Label 8550 4650 0    50   ~ 0
F
Text Label 8550 4750 0    50   ~ 0
G
Text Label 8550 4850 0    50   ~ 0
dt
Text Label 8550 2400 0    50   ~ 0
A0
Text Label 8550 2500 0    50   ~ 0
A1
Text Label 8550 2600 0    50   ~ 0
A2
Text Label 8550 2700 0    50   ~ 0
A3
$EndSCHEMATC
