# DuinoTag
Arduino based laser tag game system.
## Overview
Requires IRremote: https://github.com/z3t0/Arduino-IRremote

Modified from http://www.instructables.com/id/Duino-Tagger/


**IR Transmitter System*
- Transistor amplifier to power up the high power IR LED
- Lens to focus IR beam

**Sound**
- Piezo Sounder to provide audio feedback and sound effects

**Receivers**
- Standard IR receiver module and power regulation capacitor to provide signal feedback system

## Material
- Arduino nano
- Light Gun
- Colored LEDs (+30mA)
- IR Sensors
- 2x IR LED's (+100mA)
- Piezo Sounder
- Power Transistor /MOSFET
- Solder, Resistors, Capacitors, etc

# IRremote Installation
- Rename "Arduino-IRremote" to "IRremote"
- Move the "IRremote" folder to Arduino_Root/libraries/
- Remove RobotIRremote from Arduino_Root/libraries/
- Send Pin for Arduino Nano is 3, 9
