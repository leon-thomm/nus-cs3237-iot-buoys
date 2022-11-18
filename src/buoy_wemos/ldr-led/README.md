# LED/LDR turbidity sensor

An LED will be placed next to a Light Dependent Resistor (LDR). When murky water gets between the two modules, the LDR should register a lower light level corresponding to the murkiness/turbidity of the water.

Currently, this program increases the brightness of LED until the LDR registers max brightness. This gives us a good idea on what brightness level we should use for the LED, so that the LDR will register some drop in brightness when murky water is introduced.

# Wiring tips
## Wiring up the LDR sensor:

- Connect signal to `S` pin
- Connect GND to the middle pin
- Connect 3.3v to `-` pin

## Wiring up the LED:

- Connect LED to pin `D4` (or whatever is specified as the `ledPin` in the program)
- Connect the other end to GND
- Optinal: use a resistor to reduce the brightness