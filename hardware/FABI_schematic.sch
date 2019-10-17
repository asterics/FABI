EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "FABI schematic for 8 jack plugs"
Date "2019-09-27"
Rev ""
Comp "AsTeRICS Foundation"
Comment1 "www.asterics-foundation.org"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:AudioJack3 J?
U 1 1 5D8DD375
P 5200 2650
F 0 "J?" V 5136 2838 50  0000 L CNN
F 1 "PJ-392" V 5227 2838 50  0000 L CNN
F 2 "" H 5200 2650 50  0001 C CNN
F 3 "~" H 5200 2650 50  0001 C CNN
	1    5200 2650
	0    1    1    0   
$EndComp
$Comp
L Connector:AudioJack3 J?
U 1 1 5D8DD9E0
P 6200 2650
F 0 "J?" V 6136 2838 50  0000 L CNN
F 1 "PJ-392" V 6227 2838 50  0000 L CNN
F 2 "" H 6200 2650 50  0001 C CNN
F 3 "~" H 6200 2650 50  0001 C CNN
	1    6200 2650
	0    1    1    0   
$EndComp
$Comp
L Connector:AudioJack3 J?
U 1 1 5D8DF9FE
P 7200 2650
F 0 "J?" V 7136 2838 50  0000 L CNN
F 1 "PJ-392" V 7227 2838 50  0000 L CNN
F 2 "" H 7200 2650 50  0001 C CNN
F 3 "~" H 7200 2650 50  0001 C CNN
	1    7200 2650
	0    1    1    0   
$EndComp
$Comp
L Connector:AudioJack3 J?
U 1 1 5D8DFA04
P 7150 4750
F 0 "J?" V 7178 4520 50  0000 R CNN
F 1 "PJ-392" V 7087 4520 50  0000 R CNN
F 2 "" H 7150 4750 50  0001 C CNN
F 3 "~" H 7150 4750 50  0001 C CNN
	1    7150 4750
	0    -1   -1   0   
$EndComp
$Comp
L Connector:AudioJack3 J?
U 1 1 5D8E0D42
P 6150 4750
F 0 "J?" V 6178 4520 50  0000 R CNN
F 1 "PJ-392" V 6087 4520 50  0000 R CNN
F 2 "" H 6150 4750 50  0001 C CNN
F 3 "~" H 6150 4750 50  0001 C CNN
	1    6150 4750
	0    -1   -1   0   
$EndComp
$Comp
L Connector:AudioJack3 J?
U 1 1 5D8E0D48
P 5150 4750
F 0 "J?" V 5178 4520 50  0000 R CNN
F 1 "PJ-392" V 5087 4520 50  0000 R CNN
F 2 "" H 5150 4750 50  0001 C CNN
F 3 "~" H 5150 4750 50  0001 C CNN
	1    5150 4750
	0    -1   -1   0   
$EndComp
$Comp
L Connector:AudioJack3 J?
U 1 1 5D8E0D4E
P 4150 4750
F 0 "J?" V 4178 4520 50  0000 R CNN
F 1 "PJ-392" V 4087 4520 50  0000 R CNN
F 2 "" H 4150 4750 50  0001 C CNN
F 3 "~" H 4150 4750 50  0001 C CNN
	1    4150 4750
	0    -1   -1   0   
$EndComp
$Comp
L Connector:AudioJack3 J?
U 1 1 5D8E0D54
P 3150 4750
F 0 "J?" V 3178 4520 50  0000 R CNN
F 1 "PJ-392" V 3087 4520 50  0000 R CNN
F 2 "" H 3150 4750 50  0001 C CNN
F 3 "~" H 3150 4750 50  0001 C CNN
	1    3150 4750
	0    -1   -1   0   
$EndComp
$Comp
L ProMicro:ProMicro U?
U 1 1 5D8EF33C
P 3650 3100
F 0 "U?" H 3675 3875 50  0000 C CNN
F 1 "ProMicro" H 3675 3784 50  0000 C CNN
F 2 "" H 3200 3750 50  0001 C CNN
F 3 "" H 3200 3750 50  0001 C CNN
	1    3650 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 3050 7300 3650
Wire Wire Line
	7300 4350 7050 4350
Wire Wire Line
	3050 4350 3050 4550
Wire Wire Line
	4050 4550 4050 4350
Connection ~ 4050 4350
Wire Wire Line
	4050 4350 3050 4350
Wire Wire Line
	5050 4550 5050 4350
Connection ~ 5050 4350
Wire Wire Line
	5050 4350 4050 4350
Wire Wire Line
	6050 4550 6050 4350
Connection ~ 6050 4350
Wire Wire Line
	6050 4350 5050 4350
Wire Wire Line
	7050 4550 7050 4350
Connection ~ 7050 4350
Wire Wire Line
	7050 4350 6050 4350
Wire Wire Line
	7300 2850 7300 3050
Connection ~ 7300 3050
Wire Wire Line
	6300 2850 6300 3050
Connection ~ 6300 3050
Wire Wire Line
	6300 3050 7300 3050
Wire Wire Line
	5300 2850 5300 3050
Connection ~ 5300 3050
Wire Wire Line
	5300 3050 6300 3050
$Comp
L power:GND #PWR?
U 1 1 5D8F1B47
P 7450 3650
F 0 "#PWR?" H 7450 3400 50  0001 C CNN
F 1 "GND" V 7455 3522 50  0000 R CNN
F 2 "" H 7450 3650 50  0001 C CNN
F 3 "" H 7450 3650 50  0001 C CNN
	1    7450 3650
	0    -1   1    0   
$EndComp
Wire Wire Line
	7450 3650 7300 3650
Connection ~ 7300 3650
Wire Wire Line
	7300 3650 7300 4350
Wire Wire Line
	7100 2850 7100 3850
Wire Wire Line
	7100 3850 2400 3850
Wire Wire Line
	2400 2950 2800 2950
Wire Wire Line
	2400 2950 2400 3850
Wire Wire Line
	2800 3050 2450 3050
Wire Wire Line
	2450 3050 2450 3800
Wire Wire Line
	2450 3800 6100 3800
Wire Wire Line
	6100 3800 6100 2850
Wire Wire Line
	5100 2850 5100 3750
Wire Wire Line
	5100 3750 2500 3750
Wire Wire Line
	2500 3750 2500 3150
Wire Wire Line
	2500 3150 2800 3150
Wire Wire Line
	2800 3650 2800 3950
Wire Wire Line
	2800 3950 7250 3950
Wire Wire Line
	7250 3950 7250 4550
Wire Wire Line
	2800 3550 2750 3550
Wire Wire Line
	2750 3550 2750 4000
Wire Wire Line
	2750 4000 6250 4000
Wire Wire Line
	6250 4000 6250 4550
Wire Wire Line
	5250 4550 5250 4050
Wire Wire Line
	5250 4050 2700 4050
Wire Wire Line
	2700 4050 2700 3450
Wire Wire Line
	2700 3450 2800 3450
Wire Wire Line
	2800 3350 2650 3350
Wire Wire Line
	2650 3350 2650 4100
Wire Wire Line
	2650 4100 4250 4100
Wire Wire Line
	4250 4100 4250 4550
Wire Wire Line
	2800 3250 2600 3250
Wire Wire Line
	2600 3250 2600 4150
Wire Wire Line
	2600 4150 3250 4150
Wire Wire Line
	3250 4150 3250 4550
NoConn ~ 3150 4550
NoConn ~ 4150 4550
NoConn ~ 5150 4550
NoConn ~ 6150 4550
NoConn ~ 7150 4550
NoConn ~ 7200 2850
NoConn ~ 6200 2850
NoConn ~ 5200 2850
NoConn ~ 2800 2550
NoConn ~ 2800 2650
NoConn ~ 2800 2850
NoConn ~ 4550 2750
NoConn ~ 4550 2550
NoConn ~ 4550 2850
NoConn ~ 4550 2950
NoConn ~ 4550 3050
NoConn ~ 4550 3150
NoConn ~ 4550 3250
NoConn ~ 4550 3350
NoConn ~ 4550 3450
NoConn ~ 4550 3550
NoConn ~ 4550 3650
Text Notes 7050 6100 0    50   ~ 0
Easiest way for assembly:\n1.) Mount the ProMicro in its position (with hot glue, only on unused pin header)\n2.) Solder enamelled wire on ProMicro (just one side, take care of sufficient length)\n3.) Mount the JackPlugs, fix with hot glue\n4.) Solder GND connection\n5.) Solder remaining signals\n6.) Stabilize the USB plug with hot glue (attach a cable, otherwise you may have glue IN the plug).
NoConn ~ 4550 2650
Wire Wire Line
	4700 3050 4700 2250
Wire Wire Line
	4700 2250 2650 2250
Wire Wire Line
	2650 2250 2650 2450
Wire Wire Line
	2650 2750 2800 2750
Wire Wire Line
	4700 3050 5300 3050
$Comp
L power:GND #PWR?
U 1 1 5D913F91
P 2500 2450
F 0 "#PWR?" H 2500 2200 50  0001 C CNN
F 1 "GND" V 2505 2322 50  0000 R CNN
F 2 "" H 2500 2450 50  0001 C CNN
F 3 "" H 2500 2450 50  0001 C CNN
	1    2500 2450
	0    1    -1   0   
$EndComp
Wire Wire Line
	2500 2450 2650 2450
Connection ~ 2650 2450
Wire Wire Line
	2650 2450 2650 2750
Text Notes 5150 2150 0    79   ~ 16
3
Text Notes 7150 2150 0    79   ~ 16
1
Text Notes 6150 2150 0    79   ~ 16
2
Text Notes 3100 5350 0    79   ~ 16
4
Text Notes 4100 5350 0    79   ~ 16
5
Text Notes 6100 5350 0    79   ~ 16
7
Text Notes 7100 5350 0    79   ~ 16
8
Text Notes 5100 5350 0    79   ~ 16
6
Text Notes 5100 2300 0    39   ~ 8
(D4)
Text Notes 6100 2300 0    39   ~ 8
(D3)
Text Notes 7150 2300 0    39   ~ 8
(D2)
Text Notes 5100 5100 0    39   ~ 8
(D7)
Text Notes 3100 5100 0    39   ~ 8
(D5)
Text Notes 4100 5100 0    39   ~ 8
(D6)
Text Notes 6100 5100 0    39   ~ 8
(D8)
Text Notes 7100 5100 0    39   ~ 8
(D9)
$EndSCHEMATC
