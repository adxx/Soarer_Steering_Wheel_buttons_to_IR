# Soarer_Steering_Wheel_buttons_to_IR
Z20 steering wheel buttons convert to IR

Some of Z20 Soarers have steering wheel with control buttons - Radio and A/C. Output from these buttons is serial data output - 1 frame contains 2 or more repetiteve 10-bit data fields for each button pressed. 

Radio buttons codes are captured and converted to Alpine's NEC IR remote control standard. 

BOM: 
- Attiny13a
- 78L05 voltage regulator
- DTC144 digital transistor
- IR diod - noname
- 160 Ohm resistor for IR  diod
- 10 kOhm resistor for reset pull-up

Wiring: 
- input from DTC144 to PINB1
- Output from PINB0 through 160Ohm resistor to IR led
