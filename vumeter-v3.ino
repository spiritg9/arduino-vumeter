#include "FastLED.h"

#define NUM_LEDS 200
#define DATA_PIN 3

int pointer = 0;
int matrix[20];
CRGB led_matrix[NUM_LEDS];

int R = 10;
int G = 10;
int B = 10;

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(led_matrix, NUM_LEDS);

  Serial.begin(9600); // setup serial
  for (int i = 0; i < 20; i++) {
    matrix[i] = 0;
  }

}

void serialEvent() {
  //Serial.print("serial read");
  String s = "";
  while (Serial.available()) {
    s = Serial.readString();
    if (s[0] == '#') {
      char rstr[3];
      rstr[0] = s[1];
      rstr[1] = s[2];
      rstr[2] = s[3];
      R = charToInt(rstr);

      char gstr[3];
      gstr[0] = s[5];
      gstr[1] = s[6];
      gstr[2] = s[7];
      G = charToInt(gstr);

      char bstr[3];
      bstr[0] = s[9];
      bstr[1] = s[10];
      bstr[2] = s[11];
      B = charToInt(bstr);
    }
    Serial.print(String(s));
  }
}

int charToInt(char c[3]) {
  int number = 0;
  number = (c[0]  - '0') * 100;
  number = number + ((c[1] - '0') * 10);
  number = number + (c[2] - '0');
  return number;
}

void loop() {
  //Serial.print("loop");

  int sensorValue;
  int ledValue = 0;

  for (int i = 0; i < 20; i++) {
    sensorValue = analogRead(A0);
    if (sensorValue > ledValue) {
      ledValue = sensorValue;
    }
  }
  ledValue = ledValue / 4; // modify to adjust sensitivity
  matrix[pointer] = ledValue;

  pointer++;
  if (pointer >= 20) {
    pointer = 0;
  }
  turnOnTheLEDs();
  delay(75);
}

void turnOnTheLEDs() {
  // go through columns
  for (int i = 0; i < 20; i++) {

    int leds = matrix[i];
    if (leds > 10) {
      leds = 10;
    }

    // turn on leds
    for (int j = 0; j < leds; j++) {
      int offset;

      if (j % 2 != 0) {
        // every odd row, the signals goes backwards
        offset = abs(i - 19);
        offset = offset - pointer;
        if (offset < 0) {
          offset += 20;
        }
        led = j * 20 + offset;
      } else {
        offset = i + pointer;
        if (offset >= 20) {
          offset -= 20;
        }
        led = j * 20 + offset;
      }
      led_matrix[led].r = R;
      led_matrix[led].g = G;
      led_matrix[led].b = B;
    }

    // turn off above the desired level
    for (int j = leds; j < 10; j++) {

      int offset;

      if (j % 2 != 0) {
        // every odd row, the signals goes backwards
        offset = abs(i - 19);
        offset = offset - pointer;
        if (offset < 0) {
          offset += 20;
        }
        led = j * 20 + offset;
      } else {
        offset = i + pointer;
        if (offset >= 20) {
          offset -= 20;
        }
        led = j * 20 + offset;
      }
      led_matrix[led].r = 0;
      led_matrix[led].g = 0;
      led_matrix[led].b = 0;
    }
  }
  FastLED.show();
}
