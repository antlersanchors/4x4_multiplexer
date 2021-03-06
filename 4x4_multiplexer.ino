/*
  Row-Column Scanning an 8x8 LED matrix with X-Y input
 
 This example controls an 8x8 LED matrix using two analog inputs
 
 created 27 May 2009
 modified 30 Aug 2011
 by Tom Igoe
 
 This example works for the Lumex  LDM-24488NI Matrix. See
 http://sigma.octopart.com/140413/datasheet/Lumex-LDM-24488NI.pdf
 for the pin connections
 
 For other LED cathode column matrixes, you should only need to change
 the pin numbers in the row[] and column[] arrays
 
 rows are the anodes
 cols are the cathodes
 ---------
 
 Pin numbers:
 Matrix:
 * Digital pins 2 through 13,
 * analog pins 2 through 5 used as digital 16 through 19
 Potentiometers:
 * center pins are attached to analog pins 0 and 1, respectively
 * side pins attached to +5V and ground, respectively.
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/RowColumnScanning
 
 see also http://www.tigoe.net/pcomp/code/category/arduinowiring/514 for more
 */


// 2-dimensional array of row pin numbers:
const int row[4] = {
	8,4,3,6 };
	// 8,4,3,6 };

// 2-dimensional array of column pin numbers:
const int col[4] = {
	2,7,5,12};
	// 2,7,5,12};

// 2-dimensional array of pixels:
int pixels[4][4];          

// cursor position:
int x = 2;
int y = 2;

// define sensors
#define sensor1 A1
#define sensor2 A2

// calibration variables
int sensorHigh1 = 0;
int sensorLow1 = 1023;
int sensorHigh2 = 0;
int sensorLow2 = 1023;
int sensorValue1;
int sensorValue2;

void setup() {

  Serial.begin(9600);

  // calibrate our sensors
  while (millis() < 6000) {
  	sensorValue1 = analogRead(sensor1);

  	if (sensorValue1 > sensorHigh1) {
  		sensorHigh1 = sensorValue1;
  	}
  	if (sensorValue1 < sensorLow1) {
  		sensorLow1 = sensorValue1;
  	}

  	sensorValue2 = analogRead(sensor2);

  	if (sensorValue2 > sensorHigh2) {
  		sensorHigh2 = sensorValue2;
  	}
  	if (sensorValue2 < sensorLow2) {
  		sensorLow2 = sensorValue2;
  	}
  }

  // initialize the I/O pins as outputs
  // iterate over the pins:

  for (int thisPin = 0; thisPin < 4; thisPin++) {
    // initialize the output pins:
    pinMode(col[thisPin], OUTPUT);
    pinMode(row[thisPin], OUTPUT);  
    // take the col pins (i.e. the cathodes) high to ensure that
    // the LEDS are off:
    digitalWrite(col[thisPin], HIGH);    
  }

  // initialize the pixel matrix:
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      pixels[x][y] = HIGH;
    }
  }
}

void loop() {
  // read input:
  readSensors();

  // draw the screen:
  refreshScreen();
}

void readSensors() {
  // turn off the last position:
  pixels[x][y] = HIGH;
  // read the sensors for X and Y values:
  // shave a bit of the top to help with mapping to the full 4 pixel range
  x = 3 - map(analogRead(sensor1), sensorLow1, sensorHigh1 * 0.9, 0, 3);
  y = map(analogRead(sensor2), sensorLow2, sensorHigh2 * 0.9, 0, 3);


  // Serial.print("Sensor A2: ");
  // Serial.println(analogRead(A2));
  // Serial.print("Sensor A1: ");
  // Serial.println(analogRead(A1));

  // set the new pixel position low so that the LED will turn on
  // in the next screen refresh:
  pixels[x][y] = LOW;

}

void refreshScreen() {
  // iterate over the rows (anodes):
  for (int thisRow = 0; thisRow < 4; thisRow++) {
    // take the row pin (anode) high:
    digitalWrite(row[thisRow], HIGH);
    // iterate over the cols (cathodes):
    for (int thisCol = 0; thisCol < 4; thisCol++) {
      // get the state of the current pixel;
      int thisPixel = pixels[thisRow][thisCol];

      // Serial.print("pixels :");
      // Serial.println(pixels[thisRow][thisCol]);

      // when the row is HIGH and the col is LOW,
      // the LED where they meet turns on:
      digitalWrite(col[thisCol], thisPixel);
      // turn the pixel off:
      if (thisPixel == LOW) {
        digitalWrite(col[thisCol], HIGH);
      }
    }
    // take the row pin low to turn off the whole row:
    digitalWrite(row[thisRow], LOW);
  }
}