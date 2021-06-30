# Port-Manipulation_AT328p
It's not very often that I program an Arduino (or Chinese knock-off) through port manipulation (directly controlling the registers) so I usually have to look things back up whenever I do. This is just a reference I created for myself when there's a need to do so, but I put it here thinking others may find it useful as well.


This repository contains a datasheet for the ATmega328p microcontroller chip along with two .ino files. The two Arduino programs do exactly the same thing, one is written with the Arduino library and the other written with Port-Manipulation for comparative reference material when needed. Both programs demonstrate an example of I/O pin control, digital read and write, analog read and and PWM write, hardware interrupts, and delay/timer-interrupts.

