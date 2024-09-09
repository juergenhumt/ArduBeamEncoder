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


# download your program to the arduino and wait until downlod completed
# then start this program to read data. If python program is running the
# serial port is blocked 
import serial
import time
import os
import struct
from timeit import default_timer as timer


# modify the line below to match your serial port
ser = serial.Serial('/dev/ttyACM0')
ser.flushInput()
ser.baudrate = 114000


k= -1;

sInt = 0;

outF = open("test_data.csv","w")
outF.write('k  dT         est              rpm\n');

tStrt = timer()

while True:

    if k < 1:
      print('Program Started!')
    else:
      sInt = int.from_bytes(ser.read(4), byteorder='little',signed=False)      
      [sFlt1] = struct.unpack('f',ser.read(4))      
      [sFlt2] = struct.unpack('f',ser.read(4))      
      [sFlt3] = struct.unpack('f',ser.read(4))      
        

    tStop=False
    if sInt != 0:
      outStr = str(sInt) + '  ' + str(sFlt1) + '  ' + str(sFlt2) + '    ' + str(sFlt3) + '\n'
      outF.write(outStr)
      print(outStr)
     
      sInt = 0
      # the code below can be used if you want to stop data transmission 
      # from the arduino. Just send any of the values below
      if sInt==9999:
        tStop=True
        jSt = 0
      if sInt==999:
        tStop=True
        jSt = 2


    if (k > 3) and tStop:
      tEnd = timer()        
      cmd = "aplay ~/Data/MorseCode/txt2morse-master/test/r.wav"
      returned_value = os.system(cmd)  # returns the exit code in unix
      print('returned value:', returned_value)
      print('dltT ' + str(tEnd-tStrt) + '  jSt ' + str(jSt))
      
      break

    k= k+1

    # stop reading after preconfigured number of values received
    if k > 7500:
      tEnd = timer()
      cmd = "aplay ~/Data/MorseCode/txt2morse-master/test/r.wav"
      returned_value = os.system(cmd)  # returns the exit code in unix
      print('Max Count Reached!')
      print('Time dltT ',tEnd-tStrt)
      break
