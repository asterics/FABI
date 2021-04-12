EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "FABI PCB for 8 jack plugs (ESP32 addon board conn)"
Date "2021-02-11"
Rev ""
Comp "AsTeRICS Foundation"
Comment1 "www.asterics-foundation.org"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
NoConn ~ 3610 2870
NoConn ~ 3610 2770
Wire Wire Line
	1710 2870 1860 2870
$Comp
L power:GND #PWR0102
U 1 1 5D913F91
P 1710 2870
F 0 "#PWR0102" H 1710 2620 50  0001 C CNN
F 1 "GND" V 1715 2742 50  0000 R CNN
F 2 "" H 1710 2870 50  0001 C CNN
F 3 "" H 1710 2870 50  0001 C CNN
	1    1710 2870
	0    1    -1   0   
$EndComp
$Comp
L power:VCC #PWR013
U 1 1 5F3C6FB2
P 3800 2900
F 0 "#PWR013" H 3800 2750 50  0001 C CNN
F 1 "VCC" H 3815 3073 50  0000 C CNN
F 2 "" H 3800 2900 50  0001 C CNN
F 3 "" H 3800 2900 50  0001 C CNN
	1    3800 2900
	1    0    0    -1  
$EndComp
$Comp
L Power_Protection:SP0504BAHT D12
U 1 1 5F3C5FAA
P 4370 4850
F 0 "D12" H 4675 4896 50  0000 L CNN
F 1 "SP0504BAHT" H 4420 4650 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 4670 4800 50  0001 L CNN
F 3 "http://www.littelfuse.com/~/media/files/littelfuse/technical%20resources/documents/data%20sheets/sp05xxba.pdf" H 4495 4975 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/suppressordioden-tvs/7931960/" H 4370 4850 50  0001 C CNN "RS"
F 5 "793-1960" H 4370 4850 50  0001 C CNN "RS-Nr"
	1    4370 4850
	1    0    0    -1  
$EndComp
$Comp
L Power_Protection:SP0504BAHT D11
U 1 1 5F3C6CE7
P 3720 4850
F 0 "D11" H 4025 4896 50  0000 L CNN
F 1 "SP0504BAHT" H 3770 4650 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 4020 4800 50  0001 L CNN
F 3 "http://www.littelfuse.com/~/media/files/littelfuse/technical%20resources/documents/data%20sheets/sp05xxba.pdf" H 3845 4975 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/suppressordioden-tvs/7931960/" H 3720 4850 50  0001 C CNN "RS"
F 5 "793-1960" H 3720 4850 50  0001 C CNN "RS-Nr"
	1    3720 4850
	1    0    0    -1  
$EndComp
Text GLabel 3920 4650 1    39   Input ~ 0
AUX_1
Text GLabel 3620 4650 1    39   Input ~ 0
AUX_2
Text GLabel 3720 4650 1    39   Input ~ 0
AUX_3
Text GLabel 3820 4650 1    39   Input ~ 0
AUX_4
Text GLabel 4370 4650 1    39   Input ~ 0
AUX_5
Text GLabel 4570 4650 1    39   Input ~ 0
AUX_6
Text GLabel 4270 4650 1    39   Input ~ 0
AUX_7
Text GLabel 4470 4650 1    39   Input ~ 0
AUX_8
$Comp
L power:GND #PWR026
U 1 1 5F3C927A
P 3720 5050
F 0 "#PWR026" H 3720 4800 50  0001 C CNN
F 1 "GND" V 3725 4922 50  0000 R CNN
F 2 "" H 3720 5050 50  0001 C CNN
F 3 "" H 3720 5050 50  0001 C CNN
	1    3720 5050
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR027
U 1 1 5F3C9749
P 4370 5050
F 0 "#PWR027" H 4370 4800 50  0001 C CNN
F 1 "GND" V 4375 4922 50  0000 R CNN
F 2 "" H 4370 5050 50  0001 C CNN
F 3 "" H 4370 5050 50  0001 C CNN
	1    4370 5050
	-1   0    0    -1  
$EndComp
Text Notes 3670 4300 0    39   ~ 0
ESD protection (SMD variant)
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J9
U 1 1 5F49020A
P 7410 5010
F 0 "J9" H 7460 5427 50  0000 C CNN
F 1 "Conn_02x05_Odd_Even" H 7460 5336 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x05_P2.54mm_Vertical" H 7410 5010 50  0001 C CNN
F 3 "~" H 7410 5010 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/leiterplatten-header/7457059/" H 7410 5010 50  0001 C CNN "RS"
F 5 "745-7059" H 7410 5010 50  0001 C CNN "RS-Nr"
	1    7410 5010
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 5F4935A1
P 7710 5010
F 0 "#PWR010" H 7710 4760 50  0001 C CNN
F 1 "GND" V 7715 4882 50  0000 R CNN
F 2 "" H 7710 5010 50  0001 C CNN
F 3 "" H 7710 5010 50  0001 C CNN
	1    7710 5010
	0    -1   1    0   
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5F493918
P 7710 5210
F 0 "#PWR012" H 7710 4960 50  0001 C CNN
F 1 "GND" V 7715 5082 50  0000 R CNN
F 2 "" H 7710 5210 50  0001 C CNN
F 3 "" H 7710 5210 50  0001 C CNN
	1    7710 5210
	0    -1   1    0   
$EndComp
Text Notes 1150 4300 0    39   ~ 0
Jack plugs with ESD protection (THT variant)
Wire Notes Line
	700  4350 3200 4350
Wire Notes Line
	3200 4350 3200 7650
Wire Notes Line
	3200 7650 700  7650
Wire Notes Line
	4970 5500 3420 5500
Wire Notes Line
	3420 5500 3420 4350
Text Notes 5460 4300 0    39   ~ 0
Neopixel (WS2812B)
Wire Notes Line
	5170 4350 5170 5500
Wire Notes Line
	5170 5500 6320 5500
Wire Notes Line
	6320 5500 6320 4350
Wire Notes Line
	6320 4350 5170 4350
Text Notes 7020 4300 0    39   ~ 0
ESP32 miniBT addon board
Wire Notes Line
	6520 4350 8320 4350
Wire Notes Line
	8320 4350 8320 5500
Wire Notes Line
	8320 5500 6520 5500
Wire Notes Line
	6520 5500 6520 4350
Wire Notes Line
	610  3910 610  1960
Wire Notes Line
	610  1970 4860 1970
Wire Notes Line
	4860 1970 4860 3920
Text Notes 2310 1920 0    39   ~ 0
Arduino Pro Micro
Wire Notes Line
	4970 4350 4970 5500
Wire Notes Line
	3420 4350 4970 4350
Text GLabel 9185 4565 0    39   Input ~ 0
Buzzer_Pin
$Comp
L power:GND #PWR0101
U 1 1 602224F3
P 8860 5190
F 0 "#PWR0101" H 8860 4940 50  0001 C CNN
F 1 "GND" H 8865 5017 50  0000 C CNN
F 2 "" H 8860 5190 50  0001 C CNN
F 3 "" H 8860 5190 50  0001 C CNN
	1    8860 5190
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9185 4565 9285 4565
Wire Notes Line
	8490 5500 8490 4350
NoConn ~ 1340 4600
$Comp
L power:GND #PWR01
U 1 1 5F391609
P 1560 5100
F 0 "#PWR01" H 1560 4850 50  0001 C CNN
F 1 "GND" V 1565 4972 50  0000 R CNN
F 2 "" H 1560 5100 50  0001 C CNN
F 3 "" H 1560 5100 50  0001 C CNN
	1    1560 5100
	0    -1   1    0   
$EndComp
NoConn ~ 1340 4700
Text Notes 8705 4300 0    39   ~ 0
Buzzer
$Comp
L Device:R R8
U 1 1 601AB4BF
P 9060 4965
F 0 "R8" V 9267 4965 50  0000 C CNN
F 1 "200" V 9176 4965 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 8990 4965 50  0001 C CNN
F 3 "~" H 9060 4965 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/widerstande-durchsteckmontage/6833281/" V 9060 4965 50  0001 C CNN "RS"
F 5 "683-3281" V 9060 4965 50  0001 C CNN "RS-Nr"
	1    9060 4965
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8860 4965 8910 4965
Wire Wire Line
	9210 4965 9285 4965
$Comp
L Diode:1.5KExxA D1
U 1 1 5F3D9B1D
P 1390 4950
F 0 "D1" V 1344 5030 50  0000 L CNN
F 1 "P6KE6.8A" V 1435 5030 50  0000 L CNN
F 2 "Diode_THT:D_DO-201AE_P12.70mm_Horizontal" H 1390 4750 50  0001 C CNN
F 3 "https://www.vishay.com/docs/88301/15ke.pdf" H 1340 4950 50  0001 C CNN
F 4 "https://at.farnell.com/littelfuse/p6ke6-8a/tvs-diode/dp/1703028" V 1390 4950 50  0001 C CNN "Farnell"
F 5 "1703028" V 1390 4950 50  0001 C CNN "Farnell-Nr"
F 6 "https://at.rs-online.com/web/p/suppressordioden-tvs/1719669/" V 1390 4950 50  0001 C CNN "RS"
F 7 "171-9669" V 1390 4950 50  0001 C CNN "RS-Nr"
	1    1390 4950
	0    1    1    0   
$EndComp
Wire Wire Line
	1340 4800 1390 4800
Text GLabel 7590 3340 0    39   Input ~ 0
SCL
Text GLabel 7595 3690 0    39   Input ~ 0
SDA
$Comp
L power:GND #PWR020
U 1 1 60275866
P 7620 2940
F 0 "#PWR020" H 7620 2690 50  0001 C CNN
F 1 "GND" V 7625 2812 50  0000 R CNN
F 2 "" H 7620 2940 50  0001 C CNN
F 3 "" H 7620 2940 50  0001 C CNN
	1    7620 2940
	0    1    -1   0   
$EndComp
$Comp
L Device:R R_8
U 1 1 6037812A
P 9060 5115
F 0 "R_8" V 8945 5115 50  0000 C CNN
F 1 "200" V 8854 5115 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 8990 5115 50  0001 C CNN
F 3 "~" H 9060 5115 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/smd-widerstande/8101741/" V 9060 5115 50  0001 C CNN "RS"
F 5 "810-1741" V 9060 5115 50  0001 C CNN "RS-Nr"
	1    9060 5115
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9285 5115 9210 5115
Wire Wire Line
	8910 5115 8860 5115
Wire Wire Line
	8860 4965 8860 5115
Wire Wire Line
	8860 5115 8860 5190
Connection ~ 8860 5115
Text GLabel 1860 3070 0    39   Input ~ 0
SDA
Text GLabel 1860 3170 0    39   Input ~ 0
SCL
NoConn ~ 7710 4910
NoConn ~ 7210 5010
Text Notes 5260 2705 0    39   ~ 0
SMD
Wire Notes Line
	5120 2420 6670 2420
Text Notes 5270 2370 0    39   ~ 0
Arduino->ESP Connector - Reversed Pin
Wire Notes Line
	5120 2420 5120 3970
Wire Notes Line
	5120 3970 6670 3970
Wire Notes Line
	6670 2420 6670 3970
Wire Wire Line
	5320 2845 5290 2845
Wire Wire Line
	5290 2845 5290 2875
$Comp
L power:VDD #PWR09
U 1 1 60226D59
P 3610 2570
F 0 "#PWR09" H 3610 2420 50  0001 C CNN
F 1 "VDD" H 3625 2743 50  0000 C CNN
F 2 "" H 3610 2570 50  0001 C CNN
F 3 "" H 3610 2570 50  0001 C CNN
	1    3610 2570
	1    0    0    -1  
$EndComp
$Comp
L power:VDD #PWR023
U 1 1 60227507
P 7210 5210
F 0 "#PWR023" H 7210 5060 50  0001 C CNN
F 1 "VDD" V 7225 5337 50  0000 L CNN
F 2 "" H 7210 5210 50  0001 C CNN
F 3 "" H 7210 5210 50  0001 C CNN
	1    7210 5210
	0    -1   -1   0   
$EndComp
Text Notes 3240 2150 0    39   ~ 0
RAW:   unregulated voltage input for the Pro Micro\nVCC:   regulated voltage, 5V at 500mA\n
Connection ~ 7970 3040
Text GLabel 1860 3470 0    39   Input ~ 0
AUX_4
Text GLabel 1860 3370 0    39   Input ~ 0
AUX_5
Text GLabel 1860 3670 0    39   Input ~ 0
AUX_2
Text GLabel 1860 3770 0    39   Input ~ 0
AUX_1
Text GLabel 1860 3570 0    39   Input ~ 0
AUX_3
Text GLabel 1860 3270 0    39   Input ~ 0
Buzzer_Pin
Text GLabel 3610 3270 2    39   Input ~ 0
AUX_6
Text GLabel 3610 3370 2    39   Input ~ 0
MPX
Text GLabel 3610 3470 2    39   Input ~ 0
NEOPIXEL
Text GLabel 5690 4800 0    39   Input ~ 0
NEOPIXEL
$Comp
L power:GND #PWR016
U 1 1 5F4DD6BE
P 5690 4700
F 0 "#PWR016" H 5690 4450 50  0001 C CNN
F 1 "GND" V 5695 4572 50  0000 R CNN
F 2 "" H 5690 4700 50  0001 C CNN
F 3 "" H 5690 4700 50  0001 C CNN
	1    5690 4700
	0    1    -1   0   
$EndComp
$Comp
L Connector:Conn_01x03_Male J10
U 1 1 5F4DCE7D
P 5890 4800
F 0 "J10" H 5998 4490 50  0000 C CNN
F 1 "Conn_01x03_Male" H 5998 4590 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 5890 4800 50  0001 C CNN
F 3 "~" H 5890 4800 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/leiterplatten-header/2518092/" H 5890 4800 50  0001 C CNN "RS"
F 5 "251-8092" H 5890 4800 50  0001 C CNN "RS-Nr"
	1    5890 4800
	-1   0    0    1   
$EndComp
Connection ~ 1860 2870
Wire Wire Line
	3610 2570 3610 2670
Wire Wire Line
	1860 2970 1860 2870
$Comp
L ProMicro:ProMicro U2
U 1 1 601F96F3
P 2710 3220
F 0 "U2" H 2735 3995 50  0000 C CNN
F 1 "ProMicro" H 2735 3904 50  0000 C CNN
F 2 "FABI_customLibs:ProMicro" H 2260 3870 50  0001 C CNN
F 3 "" H 2260 3870 50  0001 C CNN
	1    2710 3220
	1    0    0    -1  
$EndComp
Text GLabel 1860 2670 0    39   Input ~ 0
A_TX
Text GLabel 1860 2770 0    39   Input ~ 0
A_RX
Text GLabel 7210 5110 0    39   Input ~ 0
A_RX
Text GLabel 7710 5110 2    39   Input ~ 0
ESP_RX
Text Notes 6250 7670 0    39   ~ 0
R_x ... SMD Resistor\nRx  ... THT Resistor\n
$Comp
L power:VDD #PWR0103
U 1 1 6031B043
P 5690 4900
F 0 "#PWR0103" H 5690 4750 50  0001 C CNN
F 1 "VDD" V 5705 5028 50  0000 L CNN
F 2 "" H 5690 4900 50  0001 C CNN
F 3 "" H 5690 4900 50  0001 C CNN
	1    5690 4900
	0    -1   -1   0   
$EndComp
Text GLabel 5320 2845 2    39   Input ~ 0
AddOn_Pin1
Text GLabel 3610 3570 2    39   Input ~ 0
AddOn_Pin3
Text GLabel 7210 4910 0    39   Input ~ 0
AddOn_Pin3
Text GLabel 3610 3170 2    39   Input ~ 0
AddOn_Pin2
Text GLabel 3610 3070 2    39   Input ~ 0
AddOn_Pin1
Text GLabel 7710 4810 2    39   Input ~ 0
AddOn_Pin2
Text GLabel 7210 4810 0    39   Input ~ 0
ESP_AddOn_Pin1
Text GLabel 3610 3670 2    39   Input ~ 0
AUX_7
Text GLabel 3610 3770 2    39   Input ~ 0
AUX_8
Wire Notes Line
	8490 5500 9840 5500
Wire Notes Line
	9840 5500 9840 4350
Wire Notes Line
	9840 4350 8490 4350
$Comp
L power:VCC #PWR019
U 1 1 6027585C
P 7620 3040
F 0 "#PWR019" H 7620 2890 50  0001 C CNN
F 1 "VCC" V 7635 3213 50  0000 C CNN
F 2 "" H 7620 3040 50  0001 C CNN
F 3 "" H 7620 3040 50  0001 C CNN
	1    7620 3040
	0    -1   1    0   
$EndComp
Connection ~ 7870 3040
Wire Wire Line
	7870 3040 7970 3040
Wire Wire Line
	7620 3040 7870 3040
Wire Wire Line
	8640 3690 8640 3240
Wire Wire Line
	8500 3340 8500 3140
Connection ~ 7870 3340
Wire Wire Line
	7590 3340 7870 3340
Wire Wire Line
	8370 3390 8370 3040
Wire Wire Line
	7970 3390 7970 3040
Wire Wire Line
	7970 3690 7595 3690
Connection ~ 7970 3690
Text GLabel 5570 5320 0    39   Input ~ 0
NEOPIXEL
$Comp
L power:GND #PWR015
U 1 1 60329107
P 5570 5220
F 0 "#PWR015" H 5570 4970 50  0001 C CNN
F 1 "GND" V 5575 5092 50  0000 R CNN
F 2 "" H 5570 5220 50  0001 C CNN
F 3 "" H 5570 5220 50  0001 C CNN
	1    5570 5220
	0    1    -1   0   
$EndComp
$Comp
L power:VDD #PWR028
U 1 1 6032910D
P 5570 5420
F 0 "#PWR028" H 5570 5270 50  0001 C CNN
F 1 "VDD" V 5585 5548 50  0000 L CNN
F 2 "" H 5570 5420 50  0001 C CNN
F 3 "" H 5570 5420 50  0001 C CNN
	1    5570 5420
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x01_Male J11_data1
U 1 1 6032D010
P 5770 5320
F 0 "J11_data1" H 5742 5298 50  0000 R CNN
F 1 "Conn_01x01_Male" H 5742 5343 50  0001 R CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_1x2mm" H 5770 5320 50  0001 C CNN
F 3 "~" H 5770 5320 50  0001 C CNN
	1    5770 5320
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x01_Male J11_GND1
U 1 1 60327A50
P 5770 5220
F 0 "J11_GND1" H 5742 5198 50  0000 R CNN
F 1 "Conn_01x01_Male" H 5742 5243 50  0001 R CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_1x2mm" H 5770 5220 50  0001 C CNN
F 3 "~" H 5770 5220 50  0001 C CNN
	1    5770 5220
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x01_Male J11_VDD1
U 1 1 603306CD
P 5770 5420
F 0 "J11_VDD1" H 5742 5398 50  0000 R CNN
F 1 "Conn_01x01_Male" H 5742 5443 50  0001 R CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_1x2mm" H 5770 5420 50  0001 C CNN
F 3 "~" H 5770 5420 50  0001 C CNN
	1    5770 5420
	-1   0    0    1   
$EndComp
Wire Wire Line
	5290 3225 5290 3275
Connection ~ 5290 3225
Wire Wire Line
	5310 3225 5290 3225
Text GLabel 5310 3225 2    39   Input ~ 0
ESP_AddOn_Pin1
$Comp
L power:GND #PWR021
U 1 1 6021ABB2
P 5290 3575
F 0 "#PWR021" H 5290 3325 50  0001 C CNN
F 1 "GND" V 5295 3447 50  0000 R CNN
F 2 "" H 5290 3575 50  0001 C CNN
F 3 "" H 5290 3575 50  0001 C CNN
	1    5290 3575
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5290 3175 5290 3225
$Comp
L Device:R R_3
U 1 1 6021ABA9
P 5290 3025
F 0 "R_3" H 5360 3071 50  0000 L CNN
F 1 "10k" H 5360 2980 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 5220 3025 50  0001 C CNN
F 3 "~" H 5290 3025 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/smd-widerstande/8123375/" H 5290 3025 50  0001 C CNN "RS"
F 5 "812-3375" H 5290 3025 50  0001 C CNN "RS-Nr"
	1    5290 3025
	1    0    0    -1  
$EndComp
$Comp
L Device:R R_4
U 1 1 6021ABA3
P 5290 3425
F 0 "R_4" H 5360 3471 50  0000 L CNN
F 1 "18k" H 5360 3380 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 5220 3425 50  0001 C CNN
F 3 "~" H 5290 3425 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/smd-widerstande/9059694/" H 5290 3425 50  0001 C CNN "RS"
F 5 "905-9694" H 5290 3425 50  0001 C CNN "RS-Nr"
	1    5290 3425
	1    0    0    -1  
$EndComp
$Comp
L FABI_customLib:AudioJack3_Ground J1
U 1 1 603F5A97
P 1140 4700
F 0 "J1" H 1122 4933 50  0000 C CNN
F 1 "AudioJack3_Ground" H 1122 4934 50  0001 C CNN
F 2 "FABI_customLibs:FC68125" H 1140 4700 50  0001 C CNN
F 3 "https://docs.rs-online.com/db52/0900766b812d152e.pdf" H 1140 4700 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/klinken-steckerbuchsen/8051655/" H 1140 4700 50  0001 C CNN "RS"
F 5 "805-1655" H 1140 4700 50  0001 C CNN "RS-Nr"
	1    1140 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1140 5000 1140 5100
Wire Wire Line
	1140 5100 1390 5100
Connection ~ 1390 5100
Wire Wire Line
	1390 5100 1560 5100
Text GLabel 1500 4800 2    39   Input ~ 0
AUX_1
Wire Wire Line
	1500 4800 1390 4800
Connection ~ 1390 4800
Wire Notes Line
	610  3920 4860 3920
Wire Notes Line
	700  7650 700  4350
NoConn ~ 1340 5400
$Comp
L power:GND #PWR02
U 1 1 6045DE5C
P 1560 5900
F 0 "#PWR02" H 1560 5650 50  0001 C CNN
F 1 "GND" V 1565 5772 50  0000 R CNN
F 2 "" H 1560 5900 50  0001 C CNN
F 3 "" H 1560 5900 50  0001 C CNN
	1    1560 5900
	0    -1   1    0   
$EndComp
NoConn ~ 1340 5500
$Comp
L Diode:1.5KExxA D2
U 1 1 6045DE65
P 1390 5750
F 0 "D2" V 1344 5830 50  0000 L CNN
F 1 "P6KE6.8A" V 1435 5830 50  0000 L CNN
F 2 "Diode_THT:D_DO-201AE_P12.70mm_Horizontal" H 1390 5550 50  0001 C CNN
F 3 "https://www.vishay.com/docs/88301/15ke.pdf" H 1340 5750 50  0001 C CNN
F 4 "https://at.farnell.com/littelfuse/p6ke6-8a/tvs-diode/dp/1703028" V 1390 5750 50  0001 C CNN "Farnell"
F 5 "1703028" V 1390 5750 50  0001 C CNN "Farnell-Nr"
F 6 "https://at.rs-online.com/web/p/suppressordioden-tvs/1719669/" V 1390 5750 50  0001 C CNN "RS"
F 7 "171-9669" V 1390 5750 50  0001 C CNN "RS-Nr"
	1    1390 5750
	0    1    1    0   
$EndComp
Wire Wire Line
	1340 5600 1390 5600
$Comp
L FABI_customLib:AudioJack3_Ground J2
U 1 1 6045DE6D
P 1140 5500
F 0 "J2" H 1122 5733 50  0000 C CNN
F 1 "AudioJack3_Ground" H 1122 5734 50  0001 C CNN
F 2 "FABI_customLibs:FC68125" H 1140 5500 50  0001 C CNN
F 3 "https://docs.rs-online.com/db52/0900766b812d152e.pdf" H 1140 5500 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/klinken-steckerbuchsen/8051655/" H 1140 5500 50  0001 C CNN "RS"
F 5 "805-1655" H 1140 5500 50  0001 C CNN "RS-Nr"
	1    1140 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1140 5800 1140 5900
Wire Wire Line
	1140 5900 1390 5900
Connection ~ 1390 5900
Wire Wire Line
	1390 5900 1560 5900
Text GLabel 1500 5600 2    39   Input ~ 0
AUX_2
Wire Wire Line
	1500 5600 1390 5600
Connection ~ 1390 5600
NoConn ~ 1340 6200
$Comp
L power:GND #PWR03
U 1 1 60469ABF
P 1560 6700
F 0 "#PWR03" H 1560 6450 50  0001 C CNN
F 1 "GND" V 1565 6572 50  0000 R CNN
F 2 "" H 1560 6700 50  0001 C CNN
F 3 "" H 1560 6700 50  0001 C CNN
	1    1560 6700
	0    -1   1    0   
$EndComp
NoConn ~ 1340 6300
$Comp
L Diode:1.5KExxA D3
U 1 1 60469AC8
P 1390 6550
F 0 "D3" V 1344 6630 50  0000 L CNN
F 1 "P6KE6.8A" V 1435 6630 50  0000 L CNN
F 2 "Diode_THT:D_DO-201AE_P12.70mm_Horizontal" H 1390 6350 50  0001 C CNN
F 3 "https://www.vishay.com/docs/88301/15ke.pdf" H 1340 6550 50  0001 C CNN
F 4 "https://at.farnell.com/littelfuse/p6ke6-8a/tvs-diode/dp/1703028" V 1390 6550 50  0001 C CNN "Farnell"
F 5 "1703028" V 1390 6550 50  0001 C CNN "Farnell-Nr"
F 6 "https://at.rs-online.com/web/p/suppressordioden-tvs/1719669/" V 1390 6550 50  0001 C CNN "RS"
F 7 "171-9669" V 1390 6550 50  0001 C CNN "RS-Nr"
	1    1390 6550
	0    1    1    0   
$EndComp
Wire Wire Line
	1340 6400 1390 6400
$Comp
L FABI_customLib:AudioJack3_Ground J3
U 1 1 60469AD0
P 1140 6300
F 0 "J3" H 1122 6533 50  0000 C CNN
F 1 "AudioJack3_Ground" H 1122 6534 50  0001 C CNN
F 2 "FABI_customLibs:FC68125" H 1140 6300 50  0001 C CNN
F 3 "https://docs.rs-online.com/db52/0900766b812d152e.pdf" H 1140 6300 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/klinken-steckerbuchsen/8051655/" H 1140 6300 50  0001 C CNN "RS"
F 5 "805-1655" H 1140 6300 50  0001 C CNN "RS-Nr"
	1    1140 6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1140 6600 1140 6700
Wire Wire Line
	1140 6700 1390 6700
Connection ~ 1390 6700
Wire Wire Line
	1390 6700 1560 6700
Text GLabel 1500 6400 2    39   Input ~ 0
AUX_3
Wire Wire Line
	1500 6400 1390 6400
Connection ~ 1390 6400
NoConn ~ 1340 7000
$Comp
L power:GND #PWR04
U 1 1 6046D82A
P 1560 7500
F 0 "#PWR04" H 1560 7250 50  0001 C CNN
F 1 "GND" V 1565 7372 50  0000 R CNN
F 2 "" H 1560 7500 50  0001 C CNN
F 3 "" H 1560 7500 50  0001 C CNN
	1    1560 7500
	0    -1   1    0   
$EndComp
NoConn ~ 1340 7100
$Comp
L Diode:1.5KExxA D4
U 1 1 6046D833
P 1390 7350
F 0 "D4" V 1344 7430 50  0000 L CNN
F 1 "P6KE6.8A" V 1435 7430 50  0000 L CNN
F 2 "Diode_THT:D_DO-201AE_P12.70mm_Horizontal" H 1390 7150 50  0001 C CNN
F 3 "https://www.vishay.com/docs/88301/15ke.pdf" H 1340 7350 50  0001 C CNN
F 4 "https://at.farnell.com/littelfuse/p6ke6-8a/tvs-diode/dp/1703028" V 1390 7350 50  0001 C CNN "Farnell"
F 5 "1703028" V 1390 7350 50  0001 C CNN "Farnell-Nr"
F 6 "https://at.rs-online.com/web/p/suppressordioden-tvs/1719669/" V 1390 7350 50  0001 C CNN "RS"
F 7 "171-9669" V 1390 7350 50  0001 C CNN "RS-Nr"
	1    1390 7350
	0    1    1    0   
$EndComp
Wire Wire Line
	1340 7200 1390 7200
$Comp
L FABI_customLib:AudioJack3_Ground J4
U 1 1 6046D83B
P 1140 7100
F 0 "J4" H 1122 7333 50  0000 C CNN
F 1 "AudioJack3_Ground" H 1122 7334 50  0001 C CNN
F 2 "FABI_customLibs:FC68125" H 1140 7100 50  0001 C CNN
F 3 "https://docs.rs-online.com/db52/0900766b812d152e.pdf" H 1140 7100 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/klinken-steckerbuchsen/8051655/" H 1140 7100 50  0001 C CNN "RS"
F 5 "805-1655" H 1140 7100 50  0001 C CNN "RS-Nr"
	1    1140 7100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1140 7400 1140 7500
Wire Wire Line
	1140 7500 1390 7500
Connection ~ 1390 7500
Wire Wire Line
	1390 7500 1560 7500
Text GLabel 1500 7200 2    39   Input ~ 0
AUX_4
Wire Wire Line
	1500 7200 1390 7200
Connection ~ 1390 7200
NoConn ~ 2540 4600
$Comp
L power:GND #PWR05
U 1 1 604A6C4C
P 2760 5100
F 0 "#PWR05" H 2760 4850 50  0001 C CNN
F 1 "GND" V 2765 4972 50  0000 R CNN
F 2 "" H 2760 5100 50  0001 C CNN
F 3 "" H 2760 5100 50  0001 C CNN
	1    2760 5100
	0    -1   1    0   
$EndComp
NoConn ~ 2540 4700
$Comp
L Diode:1.5KExxA D5
U 1 1 604A6C55
P 2590 4950
F 0 "D5" V 2544 5030 50  0000 L CNN
F 1 "P6KE6.8A" V 2635 5030 50  0000 L CNN
F 2 "Diode_THT:D_DO-201AE_P12.70mm_Horizontal" H 2590 4750 50  0001 C CNN
F 3 "https://www.vishay.com/docs/88301/15ke.pdf" H 2540 4950 50  0001 C CNN
F 4 "https://at.farnell.com/littelfuse/p6ke6-8a/tvs-diode/dp/1703028" V 2590 4950 50  0001 C CNN "Farnell"
F 5 "1703028" V 2590 4950 50  0001 C CNN "Farnell-Nr"
F 6 "https://at.rs-online.com/web/p/suppressordioden-tvs/1719669/" V 2590 4950 50  0001 C CNN "RS"
F 7 "171-9669" V 2590 4950 50  0001 C CNN "RS-Nr"
	1    2590 4950
	0    1    1    0   
$EndComp
Wire Wire Line
	2540 4800 2590 4800
$Comp
L FABI_customLib:AudioJack3_Ground J5
U 1 1 604A6C5D
P 2340 4700
F 0 "J5" H 2322 4933 50  0000 C CNN
F 1 "AudioJack3_Ground" H 2322 4934 50  0001 C CNN
F 2 "FABI_customLibs:FC68125" H 2340 4700 50  0001 C CNN
F 3 "https://docs.rs-online.com/db52/0900766b812d152e.pdf" H 2340 4700 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/klinken-steckerbuchsen/8051655/" H 2340 4700 50  0001 C CNN "RS"
F 5 "805-1655" H 2340 4700 50  0001 C CNN "RS-Nr"
	1    2340 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2340 5000 2340 5100
Wire Wire Line
	2340 5100 2590 5100
Connection ~ 2590 5100
Wire Wire Line
	2590 5100 2760 5100
Text GLabel 2700 4800 2    39   Input ~ 0
AUX_5
Wire Wire Line
	2700 4800 2590 4800
Connection ~ 2590 4800
NoConn ~ 2540 5400
$Comp
L power:GND #PWR06
U 1 1 604A6C6B
P 2760 5900
F 0 "#PWR06" H 2760 5650 50  0001 C CNN
F 1 "GND" V 2765 5772 50  0000 R CNN
F 2 "" H 2760 5900 50  0001 C CNN
F 3 "" H 2760 5900 50  0001 C CNN
	1    2760 5900
	0    -1   1    0   
$EndComp
NoConn ~ 2540 5500
$Comp
L Diode:1.5KExxA D6
U 1 1 604A6C74
P 2590 5750
F 0 "D6" V 2544 5830 50  0000 L CNN
F 1 "P6KE6.8A" V 2635 5830 50  0000 L CNN
F 2 "Diode_THT:D_DO-201AE_P12.70mm_Horizontal" H 2590 5550 50  0001 C CNN
F 3 "https://www.vishay.com/docs/88301/15ke.pdf" H 2540 5750 50  0001 C CNN
F 4 "https://at.farnell.com/littelfuse/p6ke6-8a/tvs-diode/dp/1703028" V 2590 5750 50  0001 C CNN "Farnell"
F 5 "1703028" V 2590 5750 50  0001 C CNN "Farnell-Nr"
F 6 "https://at.rs-online.com/web/p/suppressordioden-tvs/1719669/" V 2590 5750 50  0001 C CNN "RS"
F 7 "171-9669" V 2590 5750 50  0001 C CNN "RS-Nr"
	1    2590 5750
	0    1    1    0   
$EndComp
Wire Wire Line
	2540 5600 2590 5600
$Comp
L FABI_customLib:AudioJack3_Ground J6
U 1 1 604A6C7C
P 2340 5500
F 0 "J6" H 2322 5733 50  0000 C CNN
F 1 "AudioJack3_Ground" H 2322 5734 50  0001 C CNN
F 2 "FABI_customLibs:FC68125" H 2340 5500 50  0001 C CNN
F 3 "https://docs.rs-online.com/db52/0900766b812d152e.pdf" H 2340 5500 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/klinken-steckerbuchsen/8051655/" H 2340 5500 50  0001 C CNN "RS"
F 5 "805-1655" H 2340 5500 50  0001 C CNN "RS-Nr"
	1    2340 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2340 5800 2340 5900
Wire Wire Line
	2340 5900 2590 5900
Connection ~ 2590 5900
Wire Wire Line
	2590 5900 2760 5900
Text GLabel 2700 5600 2    39   Input ~ 0
AUX_6
Wire Wire Line
	2700 5600 2590 5600
Connection ~ 2590 5600
NoConn ~ 2540 6200
$Comp
L power:GND #PWR07
U 1 1 604A6C8A
P 2760 6700
F 0 "#PWR07" H 2760 6450 50  0001 C CNN
F 1 "GND" V 2765 6572 50  0000 R CNN
F 2 "" H 2760 6700 50  0001 C CNN
F 3 "" H 2760 6700 50  0001 C CNN
	1    2760 6700
	0    -1   1    0   
$EndComp
NoConn ~ 2540 6300
$Comp
L Diode:1.5KExxA D7
U 1 1 604A6C93
P 2590 6550
F 0 "D7" V 2544 6630 50  0000 L CNN
F 1 "P6KE6.8A" V 2635 6630 50  0000 L CNN
F 2 "Diode_THT:D_DO-201AE_P12.70mm_Horizontal" H 2590 6350 50  0001 C CNN
F 3 "https://www.vishay.com/docs/88301/15ke.pdf" H 2540 6550 50  0001 C CNN
F 4 "https://at.farnell.com/littelfuse/p6ke6-8a/tvs-diode/dp/1703028" V 2590 6550 50  0001 C CNN "Farnell"
F 5 "1703028" V 2590 6550 50  0001 C CNN "Farnell-Nr"
F 6 "https://at.rs-online.com/web/p/suppressordioden-tvs/1719669/" V 2590 6550 50  0001 C CNN "RS"
F 7 "171-9669" V 2590 6550 50  0001 C CNN "RS-Nr"
	1    2590 6550
	0    1    1    0   
$EndComp
Wire Wire Line
	2540 6400 2590 6400
$Comp
L FABI_customLib:AudioJack3_Ground J7
U 1 1 604A6C9B
P 2340 6300
F 0 "J7" H 2322 6533 50  0000 C CNN
F 1 "AudioJack3_Ground" H 2322 6534 50  0001 C CNN
F 2 "FABI_customLibs:FC68125" H 2340 6300 50  0001 C CNN
F 3 "https://docs.rs-online.com/db52/0900766b812d152e.pdf" H 2340 6300 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/klinken-steckerbuchsen/8051655/" H 2340 6300 50  0001 C CNN "RS"
F 5 "805-1655" H 2340 6300 50  0001 C CNN "RS-Nr"
	1    2340 6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2340 6600 2340 6700
Wire Wire Line
	2340 6700 2590 6700
Connection ~ 2590 6700
Wire Wire Line
	2590 6700 2760 6700
Text GLabel 2700 6400 2    39   Input ~ 0
AUX_7
Wire Wire Line
	2700 6400 2590 6400
Connection ~ 2590 6400
NoConn ~ 2540 7000
$Comp
L power:GND #PWR08
U 1 1 604A6CA9
P 2760 7500
F 0 "#PWR08" H 2760 7250 50  0001 C CNN
F 1 "GND" V 2765 7372 50  0000 R CNN
F 2 "" H 2760 7500 50  0001 C CNN
F 3 "" H 2760 7500 50  0001 C CNN
	1    2760 7500
	0    -1   1    0   
$EndComp
NoConn ~ 2540 7100
$Comp
L Diode:1.5KExxA D8
U 1 1 604A6CB2
P 2590 7350
F 0 "D8" V 2544 7430 50  0000 L CNN
F 1 "P6KE6.8A" V 2635 7430 50  0000 L CNN
F 2 "Diode_THT:D_DO-201AE_P12.70mm_Horizontal" H 2590 7150 50  0001 C CNN
F 3 "https://www.vishay.com/docs/88301/15ke.pdf" H 2540 7350 50  0001 C CNN
F 4 "https://at.farnell.com/littelfuse/p6ke6-8a/tvs-diode/dp/1703028" V 2590 7350 50  0001 C CNN "Farnell"
F 5 "1703028" V 2590 7350 50  0001 C CNN "Farnell-Nr"
F 6 "https://at.rs-online.com/web/p/suppressordioden-tvs/1719669/" V 2590 7350 50  0001 C CNN "RS"
F 7 "171-9669" V 2590 7350 50  0001 C CNN "RS-Nr"
	1    2590 7350
	0    1    1    0   
$EndComp
Wire Wire Line
	2540 7200 2590 7200
$Comp
L FABI_customLib:AudioJack3_Ground J8
U 1 1 604A6CBA
P 2340 7100
F 0 "J8" H 2322 7333 50  0000 C CNN
F 1 "AudioJack3_Ground" H 2322 7334 50  0001 C CNN
F 2 "FABI_customLibs:FC68125" H 2340 7100 50  0001 C CNN
F 3 "https://docs.rs-online.com/db52/0900766b812d152e.pdf" H 2340 7100 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/klinken-steckerbuchsen/8051655/" H 2340 7100 50  0001 C CNN "RS"
F 5 "805-1655" H 2340 7100 50  0001 C CNN "RS-Nr"
	1    2340 7100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2340 7400 2340 7500
Wire Wire Line
	2340 7500 2590 7500
Connection ~ 2590 7500
Wire Wire Line
	2590 7500 2760 7500
Text GLabel 2700 7200 2    39   Input ~ 0
AUX_8
Wire Wire Line
	2700 7200 2590 7200
Connection ~ 2590 7200
Text Notes 5320 5100 0    39   ~ 0
SMD Pads:
$Comp
L Device:Buzzer bzr1
U 1 1 6039FDDB
P 9385 4665
F 0 "bzr1" H 9537 4694 50  0000 L CNN
F 1 "Buzzer" H 9537 4603 50  0000 L CNN
F 2 "Buzzer_Beeper:Buzzer_12x9.5RM7.6" V 9360 4765 50  0001 C CNN
F 3 "https://docs.rs-online.com/f4c8/0900766b811685fc.pdf" V 9360 4765 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/elektro-magnetische-buzzer/7716957/" H 9385 4665 50  0001 C CNN "RS"
F 5 "771-6957" H 9385 4665 50  0001 C CNN "RS-Nr"
	1    9385 4665
	1    0    0    -1  
$EndComp
Wire Wire Line
	9285 4765 9285 4965
Connection ~ 9285 4965
Wire Wire Line
	9285 4965 9285 5115
$Comp
L FABI_customLib:dispaly_connector_91601-304LF J11
U 1 1 603EB941
P 8640 2940
F 0 "J11" H 9040 3205 50  0000 C CNN
F 1 "dispaly_connector_91601-304LF" H 9040 3114 50  0000 C CNN
F 2 "FABI_customLibs:91601304LF" H 9840 2990 50  0001 L CNN
F 3 "http://www.mouser.com/datasheet/2/18/91601-1289404.pdf" H 9840 2890 50  0001 L CNN
F 4 "Dubox 2.54mm, Board to Board Connector, PCB Mounted Receptacle, Vertical , Surface Mount, Dual Entry ,Single row , 4 Positions, 2.54mm (0.100in) Pitch" H 9840 2790 50  0001 L CNN "Description"
F 5 "8" H 9840 2690 50  0001 L CNN "Height"
F 6 "649-91601-304LF" H 9840 2590 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.com/Search/Refine.aspx?Keyword=649-91601-304LF" H 9840 2490 50  0001 L CNN "Mouser Price/Stock"
F 8 "Amphenol" H 8840 2390 50  0001 L CNN "Manufacturer_Name"
F 9 "91601-304LF" H 8840 2290 50  0001 L CNN "Manufacturer_Part_Number"
F 10 "https://at.rs-online.com/web/p/leiterplatten-buchsen/6737405/" H 8640 2940 50  0001 C CNN "RS"
F 11 "673-7405" H 8640 2940 50  0001 C CNN "RS-Nr"
	1    8640 2940
	1    0    0    -1  
$EndComp
Wire Wire Line
	8370 3040 8640 3040
Connection ~ 8370 3040
Wire Wire Line
	8500 3140 8640 3140
Wire Wire Line
	8640 2940 7620 2940
Wire Notes Line
	7200 2420 9720 2420
Wire Notes Line
	9720 2420 9720 3970
Wire Notes Line
	9720 3970 7190 3970
Text Notes 7280 2370 0    39   ~ 0
Display connector incl. Pull-UP (SMD & THT)
$Comp
L Device:R R4
U 1 1 6021AB8E
P 6030 3425
F 0 "R4" H 6100 3471 50  0000 L CNN
F 1 "18k" H 6100 3380 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5960 3425 50  0001 C CNN
F 3 "~" H 6030 3425 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/widerstande-durchsteckmontage/1997554/" H 6030 3425 50  0001 C CNN "RS"
F 5 "199-7554" H 6030 3425 50  0001 C CNN "RS-Nr"
	1    6030 3425
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 6021AB94
P 6030 3025
F 0 "R3" H 6100 3071 50  0000 L CNN
F 1 "10k" H 6100 2980 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5960 3025 50  0001 C CNN
F 3 "~" H 6030 3025 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/widerstande-durchsteckmontage/1650369/" H 6030 3025 50  0001 C CNN "RS"
F 5 "165-0369" H 6030 3025 50  0001 C CNN "RS-Nr"
	1    6030 3025
	1    0    0    -1  
$EndComp
Wire Wire Line
	6030 3175 6030 3225
$Comp
L power:GND #PWR022
U 1 1 6021AB9D
P 6030 3575
F 0 "#PWR022" H 6030 3325 50  0001 C CNN
F 1 "GND" V 6035 3447 50  0000 R CNN
F 2 "" H 6030 3575 50  0001 C CNN
F 3 "" H 6030 3575 50  0001 C CNN
	1    6030 3575
	-1   0    0    -1  
$EndComp
Text GLabel 6050 3225 2    39   Input ~ 0
ESP_AddOn_Pin1
Wire Wire Line
	6050 3225 6030 3225
Connection ~ 6030 3225
Wire Wire Line
	6030 3225 6030 3275
Text GLabel 6060 2845 2    39   Input ~ 0
AddOn_Pin1
Wire Wire Line
	6030 2845 6030 2875
Wire Wire Line
	6060 2845 6030 2845
Text Notes 6130 2705 0    39   ~ 0
THT
Wire Notes Line
	3420 7340 4780 7340
Wire Notes Line
	4780 5790 4780 7340
Wire Notes Line
	3420 5790 3420 7340
Wire Notes Line
	4780 5790 3420 5790
Wire Notes Line
	4905 5790 6780 5790
Wire Wire Line
	3680 6540 3680 6590
Connection ~ 3680 6540
Wire Wire Line
	3730 6540 3680 6540
Wire Wire Line
	4290 6540 4290 6590
Connection ~ 4290 6540
Wire Wire Line
	4340 6540 4290 6540
$Comp
L Device:R R1
U 1 1 5F3C36EA
P 4290 6340
F 0 "R1" H 4360 6386 50  0000 L CNN
F 1 "10k" H 4360 6295 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 4220 6340 50  0001 C CNN
F 3 "~" H 4290 6340 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/widerstande-durchsteckmontage/1650369/" H 4290 6340 50  0001 C CNN "RS"
F 5 "165-0369" H 4290 6340 50  0001 C CNN "RS-Nr"
	1    4290 6340
	1    0    0    -1  
$EndComp
Text GLabel 4340 6540 2    39   Input ~ 0
ESP_RX
Text GLabel 3730 6540 2    39   Input ~ 0
ESP_RX
Text GLabel 4290 6190 1    39   Input ~ 0
A_TX
Text GLabel 3680 6190 1    39   Input ~ 0
A_TX
Text GLabel 6680 6540 3    39   Input ~ 0
MPX
Wire Wire Line
	5055 6390 5055 6240
Wire Wire Line
	5055 6690 5055 6840
$Comp
L Device:C C_1
U 1 1 6021926D
P 5055 6540
F 0 "C_1" H 5170 6586 50  0000 L CNN
F 1 "100n" H 5170 6495 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 5093 6390 50  0001 C CNN
F 3 "~" H 5055 6540 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/keramik-vielschichtkondensatoren/4646688/" H 5055 6540 50  0001 C CNN "RS"
F 5 "464-6688" H 5055 6540 50  0001 C CNN "RS-Nr"
	1    5055 6540
	1    0    0    -1  
$EndComp
Wire Wire Line
	5055 6240 5455 6240
Wire Wire Line
	5055 6840 5455 6840
Text Notes 5235 6454 2    39   ~ 0
SMD
Connection ~ 5455 6240
Connection ~ 5455 6840
Wire Notes Line
	4905 7340 6780 7340
Wire Notes Line
	6780 5790 6780 7340
Connection ~ 6280 6840
Wire Wire Line
	5455 6840 6280 6840
Connection ~ 6280 6240
Wire Wire Line
	5455 6240 6280 6240
$Comp
L Sensor_Pressure:MPXA6115A U1
U 1 1 5F4DAB0A
P 6280 6540
F 0 "U1" H 5850 6586 50  0000 R CNN
F 1 "MPXV7007GP" H 6180 6140 50  0000 R CNN
F 2 "MPXV7007GP:MPXV7007GP" H 5780 6190 50  0001 C CNN
F 3 "http://www.nxp.com/files/sensors/doc/data_sheet/MPXA6115A.pdf" H 6280 7140 50  0001 C CNN
F 4 "https://at.farnell.com/nxp/mpxv7007gp/drucksensor-7-7kpa-sop-8/dp/2776055" H 6280 6540 50  0001 C CNN "Farnell"
F 5 "2776055" H 6280 6540 50  0001 C CNN "Farnell-Nr"
	1    6280 6540
	1    0    0    -1  
$EndComp
Wire Wire Line
	5455 6690 5455 6840
Wire Wire Line
	5455 6390 5455 6240
$Comp
L Device:C C1
U 1 1 601FACC9
P 5455 6540
F 0 "C1" H 5570 6586 50  0000 L CNN
F 1 "100n" H 5570 6495 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P5.00mm" H 5493 6390 50  0001 C CNN
F 3 "~" H 5455 6540 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/keramik-vielschichtkondensatoren/8523277/" H 5455 6540 50  0001 C CNN "RS"
F 5 "852-3277" H 5455 6540 50  0001 C CNN "RS-Nr"
	1    5455 6540
	1    0    0    -1  
$EndComp
Wire Notes Line
	4905 7340 4905 5790
Text Notes 5230 5740 0    39   ~ 0
MPX pressure sensor
Text Notes 3380 5740 0    39   ~ 0
Arduino->ESP voltage divider (SMD & THT)
$Comp
L power:GND #PWR018
U 1 1 5F4DBD2F
P 6280 6840
F 0 "#PWR018" H 6280 6590 50  0001 C CNN
F 1 "GND" V 6285 6712 50  0000 R CNN
F 2 "" H 6280 6840 50  0001 C CNN
F 3 "" H 6280 6840 50  0001 C CNN
	1    6280 6840
	-1   0    0    -1  
$EndComp
$Comp
L power:VCC #PWR017
U 1 1 5F4DB56F
P 6280 6240
F 0 "#PWR017" H 6280 6090 50  0001 C CNN
F 1 "VCC" H 6295 6413 50  0000 C CNN
F 2 "" H 6280 6240 50  0001 C CNN
F 3 "" H 6280 6240 50  0001 C CNN
	1    6280 6240
	1    0    0    -1  
$EndComp
Text Notes 3630 5990 0    39   ~ 0
SMD
Text Notes 4240 5990 0    39   ~ 0
THT
$Comp
L power:GND #PWR014
U 1 1 5F39FBD7
P 3680 6890
F 0 "#PWR014" H 3680 6640 50  0001 C CNN
F 1 "GND" V 3685 6762 50  0000 R CNN
F 2 "" H 3680 6890 50  0001 C CNN
F 3 "" H 3680 6890 50  0001 C CNN
	1    3680 6890
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3680 6490 3680 6540
$Comp
L Device:R R_1
U 1 1 5F39FBCE
P 3680 6340
F 0 "R_1" H 3750 6386 50  0000 L CNN
F 1 "10k" H 3750 6295 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 3610 6340 50  0001 C CNN
F 3 "~" H 3680 6340 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/smd-widerstande/8123375/" H 3680 6340 50  0001 C CNN "RS"
F 5 "812-3375" H 3680 6340 50  0001 C CNN "RS-Nr"
	1    3680 6340
	1    0    0    -1  
$EndComp
$Comp
L Device:R R_2
U 1 1 5F39FBC8
P 3680 6740
F 0 "R_2" H 3750 6786 50  0000 L CNN
F 1 "18k" H 3750 6695 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 3610 6740 50  0001 C CNN
F 3 "~" H 3680 6740 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/smd-widerstande/9059694/" H 3680 6740 50  0001 C CNN "RS"
F 5 "905-9694" H 3680 6740 50  0001 C CNN "RS-Nr"
	1    3680 6740
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 5F3C51FE
P 4290 6890
F 0 "#PWR011" H 4290 6640 50  0001 C CNN
F 1 "GND" V 4295 6762 50  0000 R CNN
F 2 "" H 4290 6890 50  0001 C CNN
F 3 "" H 4290 6890 50  0001 C CNN
	1    4290 6890
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4290 6490 4290 6540
$Comp
L Device:R R2
U 1 1 5F3C349E
P 4290 6740
F 0 "R2" H 4360 6786 50  0000 L CNN
F 1 "18k" H 4360 6695 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 4220 6740 50  0001 C CNN
F 3 "~" H 4290 6740 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/widerstande-durchsteckmontage/1997554/" H 4290 6740 50  0001 C CNN "RS"
F 5 "199-7554" H 4290 6740 50  0001 C CNN "RS-Nr"
	1    4290 6740
	1    0    0    -1  
$EndComp
Wire Wire Line
	3610 2970 3800 2970
Wire Wire Line
	3800 2970 3800 2900
Wire Notes Line
	7190 3970 7190 2420
Text GLabel 7590 5850 0    39   Input ~ 0
AddOn_Pin3
$Comp
L power:GND #PWR024
U 1 1 6031A014
P 7770 6150
F 0 "#PWR024" H 7770 5900 50  0001 C CNN
F 1 "GND" H 7775 5977 50  0000 C CNN
F 2 "" H 7770 6150 50  0001 C CNN
F 3 "" H 7770 6150 50  0001 C CNN
	1    7770 6150
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R_5
U 1 1 6032A458
P 7770 6000
F 0 "R_5" H 7840 6046 50  0000 L CNN
F 1 "10k" H 7840 5955 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 7700 6000 50  0001 C CNN
F 3 "~" H 7770 6000 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/smd-widerstande/8123375/" H 7770 6000 50  0001 C CNN "RS"
F 5 "812-3375" H 7770 6000 50  0001 C CNN "RS-Nr"
	1    7770 6000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR025
U 1 1 603348A2
P 8060 6150
F 0 "#PWR025" H 8060 5900 50  0001 C CNN
F 1 "GND" H 8065 5977 50  0000 C CNN
F 2 "" H 8060 6150 50  0001 C CNN
F 3 "" H 8060 6150 50  0001 C CNN
	1    8060 6150
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 603348A8
P 8060 6000
F 0 "R5" H 8130 6046 50  0000 L CNN
F 1 "10k" H 8130 5955 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 7990 6000 50  0001 C CNN
F 3 "~" H 8060 6000 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/widerstande-durchsteckmontage/1650369/" H 8060 6000 50  0001 C CNN "RS"
F 5 "165-0369" H 8060 6000 50  0001 C CNN "RS-Nr"
	1    8060 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7770 5850 8060 5850
Wire Wire Line
	7590 5850 7770 5850
Connection ~ 7770 5850
Wire Notes Line
	6980 6450 8550 6450
Wire Notes Line
	8550 5720 8550 6450
Wire Notes Line
	6980 5720 8550 5720
Wire Notes Line
	6980 6450 6980 5720
Text Notes 7040 5680 0    39   ~ 0
Pin 3 Pulldown (for PCB detection in SW)
$Comp
L Device:R R_7
U 1 1 6074582F
P 8220 3190
F 0 "R_7" H 8150 3236 50  0000 R CNN
F 1 "10k" H 8150 3145 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 8150 3190 50  0001 C CNN
F 3 "~" H 8220 3190 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/smd-widerstande/8123375/" H 8220 3190 50  0001 C CNN "RS"
F 5 "812-3375" H 8220 3190 50  0001 C CNN "RS-Nr"
	1    8220 3190
	1    0    0    -1  
$EndComp
Wire Wire Line
	7870 3340 8220 3340
Wire Wire Line
	7970 3040 8220 3040
Connection ~ 8220 3040
Wire Wire Line
	8220 3040 8370 3040
Connection ~ 8220 3340
Wire Wire Line
	8220 3340 8500 3340
$Comp
L Device:R R_6
U 1 1 6074FD21
P 8370 3540
F 0 "R_6" H 8440 3586 50  0000 L CNN
F 1 "10k" H 8440 3495 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 8300 3540 50  0001 C CNN
F 3 "~" H 8370 3540 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/smd-widerstande/8123375/" H 8370 3540 50  0001 C CNN "RS"
F 5 "812-3375" H 8370 3540 50  0001 C CNN "RS-Nr"
	1    8370 3540
	1    0    0    -1  
$EndComp
Wire Wire Line
	7970 3690 8370 3690
Connection ~ 8370 3690
Wire Wire Line
	8370 3690 8640 3690
$Comp
L Device:R R7
U 1 1 6076ABE6
P 7870 3190
F 0 "R7" H 7800 3236 50  0000 R CNN
F 1 "10k" H 7800 3145 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 7800 3190 50  0001 C CNN
F 3 "~" H 7870 3190 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/widerstande-durchsteckmontage/1650369/" H 7870 3190 50  0001 C CNN "RS"
F 5 "165-0369" H 7870 3190 50  0001 C CNN "RS-Nr"
	1    7870 3190
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 6076FBC9
P 7970 3540
F 0 "R6" H 8040 3586 50  0000 L CNN
F 1 "10k" H 8040 3495 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 7900 3540 50  0001 C CNN
F 3 "~" H 7970 3540 50  0001 C CNN
F 4 "https://at.rs-online.com/web/p/widerstande-durchsteckmontage/1650369/" H 7970 3540 50  0001 C CNN "RS"
F 5 "165-0369" H 7970 3540 50  0001 C CNN "RS-Nr"
	1    7970 3540
	1    0    0    -1  
$EndComp
$EndSCHEMATC
