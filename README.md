# ad9833
Simple code to drive AD9833 DDS chip

This repository contains simple code to drive the AD9833 from Arduino (in C++) and Raspberry Pi (in Python).

There is only the most basic functionality: control of one of the
chip’s frequency registers. However if you can read the datasheet 
it should be easy to control the chip as you wish.

If you want a higher-level API, there are better choices.

These include https://github.com/MajicDesigns/MD_AD9833 for
the Arduino, from whence I copied the bit-banging SPI code.
