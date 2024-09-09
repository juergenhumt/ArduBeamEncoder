/*
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
*/

#include <SKalmanF.h>

volatile unsigned long counter, k = 0;
volatile long m, curTm, prvTm, dT; 
volatile int pn1, pN, nSeg, kY, i;
volatile float rpm, rps, pi, dFhi, dFhiSec, estVal, prvEstVal;

typedef union {
  unsigned long intNum;
  uint8_t bytes[4];  
} INTUNION_t;

INTUNION_t myInt;
 
typedef union {
  float fltNum;
  uint8_t bytes[4];
} FLOATUNION_t;

FLOATUNION_t myFlt;

SimpleKalmanFilter skf(2, 2, 0.012);
    
void setup() {
  
  Serial.begin (114000);
  Serial.flush();
  nSeg = 64;
  pi= asin(1.0);
  dFhi = 2*pi/nSeg;
  prvTm = 0;

  prvEstVal= 0;

  counter  = 0;
  pn1 = 2;
  kY = -1;
  m = 0;
  pinMode(pn1, INPUT_PULLUP); // internal pullup input pin 
  
  //Setting up interrupt
  //A rising pulse from encoder activates ai0(). 
  // AttachInterrupt to DigitalPin No. 2 on Arduino.
  noInterrupts();
  // the digitalPinToInterrupt function has been
  // added hoping that the code will run on differen
  // arduinos without requiering a change
  pN = digitalPinToInterrupt(pn1);
  attachInterrupt(pN, ai0, RISING); 
  interrupts();
}
   
void loop() {
  // Send the value of counter
  k = k+1;
  if (k > 20000) {
    //Serial.write(kOut + m);
    // Serial.println(pN);
    k=0;
    m = m+1;
  }
}


void wrtFlt(float xFlt) {
  myFlt.fltNum = xFlt;  
  for ( i=0; i<4; i++)
     Serial.write(myFlt.bytes[i]);
}
     
void ai0() {
  noInterrupts();  
  // ai0 is activated if DigitalPin pn1 is going from LOW to HIGH
  counter++;
  curTm = millis(); // micros();
  dT = curTm - prvTm;

  dFhiSec = 0;
  
  if (dT > 0) {
    dFhiSec = dFhi/dT;
    estVal = skf.updateEstimate(dT);
    prvEstVal = estVal;  } else
    estVal = prvEstVal;
  // rotational speed per sec and per min is
  rps = 1.0e3/(estVal*nSeg);
  rpm = rps*60;
  if(counter > 4294967290) {
    counter = 0;
  }
  // the loop below sends one integer via serial link
  // arduino uses littel endian byte order
  myInt.intNum = counter;
  for ( i=0; i<4; i++)
     Serial.write(myInt.bytes[i]);
     
  wrtFlt(dT);
  wrtFlt(estVal);
  wrtFlt(rpm);
  prvTm = curTm;

  kY = -kY;
  interrupts();
}
