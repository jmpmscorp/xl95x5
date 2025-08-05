# Example for `XL9535/9555` driver

## What it does

Configure device PORT0 GPIOs as outputs and PORT1 GPIOs as inputs. Toggle (blink) each PORT0 GPIO and read inputs every second. It uses 16-bits operations. See docs for individual GPIO API.

## Wiring

Connect LED-resistor to any PORT0 GPIO. Connect any digital switch(i.e. push button) on any PORT1 GPIO.