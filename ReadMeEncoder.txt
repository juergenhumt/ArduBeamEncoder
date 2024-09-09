# Copyright 2024 Juergen Humt
# 
# This file is part of ArduBeamEncoder.
# 
#
#     ArduBeamEncoder, is free  software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by the 
#     Free Software Foundation, either version 3 of the License or any later 
#     version.
# 
#     ArduBeamEncoder is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.
# 
#     You should have received a copy of the GNU General Public License along 
#     with ArduBeamEncoder.  If not, see <http://www.gnu.org/licenses/>.



ArduBeamEncoder

The project uses a beam speed sensor to measure rotational speed. The files included are:
- encoder_test2.ino  c/c++ file for the arduino. It uses an interrupt to detect
           if a segment of the encoder wheel obstructs the sensor beam. The time
           in milli seconds between two consecutive impulses is measured and the
           rotational speed calculated. The values are sent to the PC via serial
           link
- rdSerEnc1.py       python file to receive the data sent by the arduino on a PC
- SKalmanF           a simple Kalman filter implemented by Denys Sene
- two svg files      these contain two different encoder wheels. If you print them
           use 600dpi for the laser printer. You can use gimp to generate printable
           files from the svg
- two jpg files      encoder_test_rig1/2.jpg show the test rig with a controllable
            motor...;-). In #1 the yellow data cable is connected to pin 11 which 
            does not work since 11 is not an input port which can be used with an
            interrupt. In the program pin 2 is used.

Hardware:
- arduino (program is tested with mega 2560 and micro)
- beam sensor
- three cables to connect sensor and arduino
- serial cable from PC that fits the arduino
- hand crafted encoder wheel
- base on which to mount the sensor and the
  encoder wheel.


Apart from an arduino, some cables and the sensor you need an encoder wheel, which rotates in the slot between the two small pedestals, that house the light source and photoelectric cell to detect when the beam is interrupted by a segment of the encoder wheel. First I tried a wheel, laser printed on clear plastic, but this did not work. The black segments did let too much light pass through, so the sensor was not triggered. Currently I am using a 64 segment wheel cut from cardboard. I will try a printed wheel where I stack several prints one above the other. You might need four prints to effectively block the light beam.
For test purposes some data is transmitted to the PC via a serial connection. Since an arduino Serial.write statement transmits only one byte, variables have to be split into a byte sequence which can then be transmitted and must be converted to the variable in the receiving program on the PC. Make sure that the python program is not running when you want to download the program from the development environment to the arduino, since then the serial link is blocked.
My sequence is a) download program from IDE to arduino b) start rdSerEnc2.py c) start turning encoder wheel

There are a few odd things which I do not understand so far. 
- Every other dT value is zero but this is not consistent, as now and then two consecutive non zero values are recorded.
- Using the millis() function and then smoothing the data with a Kalman filter the measured rotational speeds seem consitent and vary with the motor speed. However when the micros() function is used to measure the time the values wildly jump back and forth so that even the Kalman filter is not able to give a useful estimate 


The program was developed on an arduino mega 2560 where it uses about 3% of the main memory. It was also tested on an arduino micro. It worked flawlessly but takes up 23% of the memory of the micro. Note that the cables need male/female terminals for the mega and female/female for the micro. You also need different types of connector cables from PC to controler.

The arduino integrated development environment (=IDE) usually creates a folder sketchbook for your porjects. Place the SKalmanF folder in the sketchbook folder,so that it can be found. If you prefer a global folder read the instructions on where the IDE searches for included modules and files.

On the site below you can generate encoder wheels
https://www.dimensionengineering.com/calculators/codewheel?style=&diameter=5.5&units=in&diameter_s=0.6&lines=96&linetype=&encdist=0.1&edt=&sw=&gen=Generate#

Have fun with your arduino!

Jürgen
