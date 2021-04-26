//---------------------------------------------------------------------------------------
// Ethernet base NTP clock panel driver.
// Support routines for Seven-segment display.
// 
// Copyright (c) 2021 Dilshan R Jayakody. [jayakody2000lk@gmail.com]
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//---------------------------------------------------------------------------------------

#include "Arduino.h"
#include "netclkdisplay.h"

void setSegmentValue(unsigned char val, bool dot)
{
  switch(val)
  {
    case 0:
      // Digit "0".
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      break;
    case 1:
      // Digit "1".
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      break;
    case 2:
      // Digit "2".
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      break;
    case 3:
      // Digit "3".
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);   
      break;
    case 4:
      // Digit "4".
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);    
      break;
    case 5:
      // Digit "5".
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);    
      break;
    case 6:
      // Digit "6".
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);
      break;
    case 7:
      // Digit "7".
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);    
      break;
    case 8:
      // Digit "8".
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);     
      break;
    case 9:
      // Digit "9".
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);
      break;
    default:
      // Clear (turn off) the current segment.
      digitalWrite(2, LOW);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);      
  } 

  digitalWrite(9, dot ? HIGH : LOW);
}

void selectSegment(unsigned char segment)
{
  switch(segment)
  {
    case 1:
      digitalWrite(A0, HIGH);
      digitalWrite(A1, LOW);
      digitalWrite(A2, LOW);
      digitalWrite(A3, LOW);
      break;
    case 2:
      digitalWrite(A0, LOW);
      digitalWrite(A1, HIGH);
      digitalWrite(A2, LOW);
      digitalWrite(A3, LOW);
      break;
   case 3:
      digitalWrite(A0, LOW);
      digitalWrite(A1, LOW);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, LOW);
      break;
   case 4:
      digitalWrite(A0, LOW);
      digitalWrite(A1, LOW);
      digitalWrite(A2, LOW);
      digitalWrite(A3, HIGH);
      break;
   default:
      // Shutdown all segments.
      digitalWrite(A0, LOW);
      digitalWrite(A1, LOW);
      digitalWrite(A2, LOW);
      digitalWrite(A3, LOW);
  }
}

void clearDisplay()
{
  selectSegment(0);
  setSegmentValue(0xFF, false);
}

void displayTime(unsigned char *segVal)
{
  unsigned char tempVal = segVal[0] / 10;
  
  selectSegment(0);
  setSegmentValue((tempVal == 0) ? 0xFF : tempVal, false);
  selectSegment(1);
  delay(1);

  selectSegment(0);
  setSegmentValue(segVal[0] % 10, (segVal[2] % 2));
  selectSegment(2);
  delay(1);

  selectSegment(0);
  setSegmentValue(segVal[1] / 10, false);
  selectSegment(3);
  delay(1);

  selectSegment(0);
  setSegmentValue(segVal[1] % 10, false);
  selectSegment(4);
  delay(1);
}

void displaySeconds(unsigned char *segVal)
{
  selectSegment(0);
  setSegmentValue(0xFF, false);
  selectSegment(1);
  delay(1);

  selectSegment(0);
  setSegmentValue(0xFF, false);
  selectSegment(2);
  delay(1);

  selectSegment(0);
  setSegmentValue(segVal[1] / 10, false);
  selectSegment(3);
  delay(1);

  selectSegment(0);
  setSegmentValue(segVal[1] % 10, false);
  selectSegment(4);
  delay(1);
}

void initDisplay()
{
  // Configure display segment lines as output.
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);

  // Configure display data lines as output.
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  // Clear all the segments of the seven segment display.
  clearDisplay();
}

void activateRefreshTimer()
{
  noInterrupts();
  
  // Enable timer1.
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  // Set timer 1 to 1Hz.
  OCR1A = 15624;

  // Enable timer 1 compare interrupt and set prescaler to 256.
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10); 
  TIMSK1 |= (1 << OCIE1A);

  interrupts();
}

unsigned char formatHours(bool hr24, unsigned char hr)
{
  if(hr24)
  {
    // 24-hour clock format.
    return hr;
  }
  else
  {
    // 12-hour clock format.
    return (hr > 12) ? (hr - 12) : hr;
  }
}