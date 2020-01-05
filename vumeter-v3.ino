#include "FastLED.h"

#define NUM_LEDS 200
#define DATA_PIN 3

#define ROWS 10
#define COLUMNS 20



int matrix[COLUMNS];
CRGB led_matrix[NUM_LEDS];

int timeout = 75;
int mode = 0;
int R = 10;
int G = 10;
int B = 10;

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(led_matrix, NUM_LEDS);

  Serial.begin(9600); // setup serial
  for (int i = 0; i < COLUMNS; i++) {
    matrix[i] = 0;
  }

}

void serialEvent() {
  //Serial.print("serial read");
  String s = "";
  while (Serial.available()) {
    s = Serial.readString();
    switch (s[0]) {
      case '#':
        char rstr[4];
        rstr[0] = '0';
        rstr[1] = s[1];
        rstr[2] = s[2];
        rstr[3] = s[3];
        R = charToInt(rstr);

        char gstr[4];
        gstr[0] = '0';
        gstr[1] = s[5];
        gstr[2] = s[6];
        gstr[3] = s[7];
        G = charToInt(gstr);

        char bstr[4];
        bstr[0] = '0';
        bstr[1] = s[9];
        bstr[2] = s[10];
        bstr[3] = s[11];
        B = charToInt(bstr);
        break;
      case 'm':
        setMode(s[1]);
        break;
      case 'd':
        Serial.println("setting delay");
        char dstr[4];
        dstr[0] = s[1];
        dstr[1] = s[2];
        dstr[2] = s[3];
        dstr[3] = s[4];
        timeout = charToInt(dstr);
        Serial.print("delay: ");
        Serial.println(timeout);
        break;
      default:
        Serial.print("unknown command");
        break;
    }
    Serial.print(String(s));
  }
}

void setMode(char c) {
  switch (c) {
    case '0':
      mode = 0;
      Serial.print("mode0");
      break;
    case '1':
      mode = 1;
      Serial.print("mode1");
      break;
    default:
      mode = 0;
      break;
  }
}

int charToInt(char c[4]) {
  int number = 0;
  number = (c[0]  - '0') * 1000;
  number = number + ((c[1]  - '0') * 100);
  number = number + ((c[2] - '0') * 10);
  number = number + (c[3] - '0');
  return number;
}



void loop() {
  //Serial.print("loop");

  int sensorValue;
  int ledValue = 0;

  for (int i = 0; i < COLUMNS; i++) {
    sensorValue = analogRead(A0);
    if (sensorValue > ledValue) {
      ledValue = sensorValue;
    }
  }
  ledValue = ledValue / 4; // modify to adjust sensitivity

  if (ledValue > ROWS) {
    ledValue = ROWS;
  }

  for (int k = COLUMNS; k > 0; k--) {
    matrix[k] = matrix[k - 1];
  }
  matrix[0] = ledValue;

  switch (mode) {
    case 0:
      mode0();
      break;
    case 1:
      mode1();
      break;
    default:
      mode0();
      break;
  }
}

void mode0() {
  turnOnTheLEDs();
  delay(timeout);
}

int counter = 0;
void mode1() {
  counter++;
  if (counter > COLUMNS) {
    counter = 0;
    turnOnTheLEDs();
    delay(timeout);
  }
}

void turnOnTheLEDs() {
  // go through columns
  //Serial.println();
  for (int i = 0; i < COLUMNS; i++) {
    //Serial.print(matrix[i]);
    //Serial.print(" ");
    int leds = matrix[i];

    for (int j = 0; j < ROWS; j++) {
      int offset;
      int led;


      offset = i;
      if (offset >= COLUMNS) {
        offset -= COLUMNS;
      }

      if (j % 2 != 0) {
        // every odd row, the signals goes backwards
        offset = abs(offset - (COLUMNS - 1));
      }

      led = j * COLUMNS + offset;

      if (j < leds) {
        led_matrix[led].r = R;
        led_matrix[led].g = G;
        led_matrix[led].b = B;
      } else {
        led_matrix[led].r = 0;
        led_matrix[led].g = 0;
        led_matrix[led].b = 0;
      }

    }
  }
  FastLED.show();
}
