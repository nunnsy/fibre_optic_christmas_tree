# 🎄🎅 Fibre Optic Christmas Tree 🎅🎄

Code running on ATTiny85 to make our Christmas tree shine bright.

The circuit is simple as to allow the PWM outputs of the ATTiny85 to drive transistors in supplying current to a set of LEDs. These four-legged (B-GR) LEDs are controlled through three PWM channels are used to control the RGB values.

An ADC is used to read the voltage across LED pins at their respective maximum to ensure it is not under a certain threshold. If it where, this would indicate one or more LEDs has failed and thus would heat the transistors. If this occurs, it is sensed and the RGB values are written to zero, with a warning LED powered to show a fault has occurred.
