Software for arduino uno r3 to control robotic arm sinoning v1.1.
The arm can be moved either with the use of potentiometers or remotely, via bluetooth.

The protocol is very simple and the commands are bound to the numbering of potentiometers on the board.
The command needs to end with a new line character (\n).

r - release remote control

c - get remote control

command to control the motor movement (sent as ASCII characters)

[motor index][absolute position]

motor index:
0
1
2
3

absolute position: value in degrees

example:

1100

will move motor 1 to 100 degrees (numbering of motors on the arduino board)


An example of the client software can be found in that repository:
https://github.com/kasikS/RoboArm-controller
