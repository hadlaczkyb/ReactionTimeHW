## Reaction time game

Write a reaction time measuring application using the STK3700 board.

Homework teammate: *Bence Szabó*

# Objective of the game

The application displays a character (number or letter) on the alphanumeric display of the board. The player must send the displayed character back using the serial port as fast as possible.

# Game logic

The application measures the time required to provide the correct answer while counting the number of incorrect characters sent in the meantime. The reaction time must be displayed on the 4 digit display in [s].[ms] resolution. Using the serial port, the results must be displayed in the following format: "**REQ:x ERROR:y, RT:z CR+LF**", where **x** is the requested character, **y** is the number of errors, **z** is the reaction time in [ms]. The configuration for the serial port is '115200 baud 8N1'.
