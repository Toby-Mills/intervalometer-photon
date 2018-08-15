EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:particle
LIBS:intervalometer-photon-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 4700 4100 3    60   Input ~ 0
GND
Text GLabel 5300 3650 2    60   Input ~ 0
OPTO
$Comp
L LTV-827 U1
U 1 1 5B713F81
P 6950 3250
F 0 "U1" H 6750 3650 50  0000 L CNN
F 1 "LTV-827" H 6950 3650 50  0000 L CNN
F 2 "Housings_DIP:DIP-8_W7.62mm" H 6750 2900 50  0001 L CIN
F 3 "" H 6950 3150 50  0001 L CNN
	1    6950 3250
	1    0    0    -1  
$EndComp
Text GLabel 6100 3150 0    60   Input ~ 0
GND
Text GLabel 6100 2950 0    60   Input ~ 0
OPTO
$Comp
L LED D1
U 1 1 5B7142C4
P 6400 3200
F 0 "D1" H 6400 3300 50  0000 C CNN
F 1 "LED" H 6400 3100 50  0000 C CNN
F 2 "LEDs:LED_D5.0mm" H 6400 3200 50  0001 C CNN
F 3 "" H 6400 3200 50  0001 C CNN
	1    6400 3200
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x02 J3
U 1 1 5B71473E
P 7650 3000
F 0 "J3" H 7650 3100 50  0000 C CNN
F 1 "Conn_01x02" H 7650 2800 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x02_Pitch2.54mm" H 7650 3000 50  0001 C CNN
F 3 "" H 7650 3000 50  0001 C CNN
	1    7650 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 2950 7450 2950
Wire Wire Line
	7450 2950 7450 3000
Wire Wire Line
	7250 3150 7450 3150
Wire Wire Line
	7450 3150 7450 3100
Wire Wire Line
	6100 3150 6250 3150
Wire Wire Line
	6250 3150 6250 3200
Wire Wire Line
	6550 2900 6650 2900
Wire Wire Line
	6650 2900 6650 2950
Wire Wire Line
	6550 3200 6650 3200
Wire Wire Line
	6650 3200 6650 3150
Wire Wire Line
	6100 2950 6250 2950
Wire Wire Line
	6250 2950 6250 2900
$Comp
L R R1
U 1 1 5B71D1EF
P 6400 2900
F 0 "R1" V 6480 2900 50  0000 C CNN
F 1 "R" V 6400 2900 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 6330 2900 50  0001 C CNN
F 3 "" H 6400 2900 50  0001 C CNN
	1    6400 2900
	0    1    1    0   
$EndComp
$Comp
L Photon U2
U 1 1 5B729424
P 4750 3550
F 0 "U2" H 4550 4500 60  0000 C CNN
F 1 "Photon" V 4800 3800 60  0000 C CNN
F 2 "Particle:photon_tht" H 5300 4600 60  0001 C CNN
F 3 "" H 5300 4600 60  0000 C CNN
	1    4750 3550
	1    0    0    -1  
$EndComp
$Comp
L SW_DIP_x01 SW1
U 1 1 5B729AF1
P 5850 4200
F 0 "SW1" H 5850 4350 50  0000 C CNN
F 1 "SW_DIP_x01" H 5850 4050 50  0000 C CNN
F 2 "Connectors_Terminal_Blocks:TerminalBlock_bornier-2_P5.08mm" H 5850 4200 50  0001 C CNN
F 3 "" H 5850 4200 50  0001 C CNN
	1    5850 4200
	1    0    0    -1  
$EndComp
Text GLabel 5550 4200 0    60   Input ~ 0
GND
Text GLabel 5300 3150 2    60   Input ~ 0
START
Text GLabel 6150 4200 2    60   Input ~ 0
START
Text GLabel 4950 2450 1    60   Input ~ 0
3V3
$EndSCHEMATC
