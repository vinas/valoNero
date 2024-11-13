#include <Arduino.h>
#include "ButtonClass.h"

const int AMOUNT_OF_STATIONS = 4,
  STATIONS[4][2] = {
    {4, 5},
    {6, 7},
    {8, 9},
    {10, 11}
  };

Button buttons[AMOUNT_OF_STATIONS] = {
  Button(STATIONS[0][0]),
  Button(STATIONS[1][0]),
  Button(STATIONS[2][0]),
  Button(STATIONS[3][0])
};

void initPins();
void testLeds();
void testButtons();
void blinkAllLights();

void setup() {
  initPins();
  testLeds();
  testButtons();
  blinkAllLights();
}

void loop() {
}

void initPins() {
  for (int i = 0; i < AMOUNT_OF_STATIONS; i++) {
    pinMode(STATIONS[i][0], INPUT_PULLUP);
    pinMode(STATIONS[i][1], OUTPUT);
  }
}

void testLeds() {
  for (int i = 0; i < AMOUNT_OF_STATIONS; i++) {
    digitalWrite(STATIONS[i][1], HIGH);
    delay(500);
    digitalWrite(STATIONS[i][1], LOW);
    delay(200);
  }
}

void testButtons() {
  for (int i = 0; i < AMOUNT_OF_STATIONS; i++) {
    digitalWrite(STATIONS[i][1], HIGH);
    do {
      buttons[i].read();
    } while (!buttons[i].wasPressed());
    digitalWrite(STATIONS[i][1], LOW);
  }
}

void blinkAllLights() {
  digitalWrite(STATIONS[0][1], HIGH);
  digitalWrite(STATIONS[1][1], HIGH);
  digitalWrite(STATIONS[2][1], HIGH);
  digitalWrite(STATIONS[3][1], HIGH);
  delay(500);
  digitalWrite(STATIONS[0][1], LOW);
  digitalWrite(STATIONS[1][1], LOW);
  digitalWrite(STATIONS[2][1], LOW);
  digitalWrite(STATIONS[3][1], LOW);
  delay(200);
  digitalWrite(STATIONS[0][1], HIGH);
  digitalWrite(STATIONS[1][1], HIGH);
  digitalWrite(STATIONS[2][1], HIGH);
  digitalWrite(STATIONS[3][1], HIGH);
  delay(500);
  digitalWrite(STATIONS[0][1], LOW);
  digitalWrite(STATIONS[1][1], LOW);
  digitalWrite(STATIONS[2][1], LOW);
  digitalWrite(STATIONS[3][1], LOW);
  delay(200);
  digitalWrite(STATIONS[0][1], HIGH);
  digitalWrite(STATIONS[1][1], HIGH);
  digitalWrite(STATIONS[2][1], HIGH);
  digitalWrite(STATIONS[3][1], HIGH);
  delay(500);
  digitalWrite(STATIONS[0][1], LOW);
  digitalWrite(STATIONS[1][1], LOW);
  digitalWrite(STATIONS[2][1], LOW);
  digitalWrite(STATIONS[3][1], LOW);
  delay(200);
}
