General Notes regarding Robockey code.

Pin Assignments
V+   Motor Driver, PWM1 , Motor Driver, PWM2, Motor Driver, +5V
B4 : ADC, Phototransistor
B5 : OCR4B'
B6 : OCR4B
C6 : OCR4A'
C7 : OCR4A
E6 : unused - red onboard LED
AR : unused
F0 : 
F1 : ADC, Phototransistor
F4 : ADC, Phototransistor
F5 : ADC, Phototransistor
F6 : ADC, Phototransistor
F7 : ADC, Phototransistor

GND
B0 : GPIO, Yellow Status LED
B1 : Red LED
B2 : Blue LED
B3 : 
B7 : 
D0 : off limits - mWii & mRF
D1 : off limits - mWii & mRF
D2 : off limits - mRF
D3 : Motor Driver, ENABLE (A0)
D4 : ADC, Phototransistor
D5 : 
D6 :
D7 : ADC, Phototransistor

STATUS LEDS:
onboard green - mWii not connected
onboard red - only 0-2 points seen for localization
yellow LED - sees puck

ROBOT ADDRESSES:
0x18 - Offensive Bot 1
0x19 - Offensive Bot 2
0x1A - Goalie Bot