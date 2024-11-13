#include <Arduino.h>
#include <EEPROM.h>
#include "ButtonClass.h"

const int AMOUNT_OF_STATIONS = 4,
  AMOUNT_OF_PHASES = 6,
  PHASE_BLOCK = 15,
  STATIONS[4][2] = {
    {4, 5},
    {6, 7},
    {8, 9},
    {10, 11}
  };

unsigned const int LIGHT_TIME_RANGE[2] = {200, 1000};

int currentTarget,
  currentLap = 0,
  targets[999],
  hits[999],
  variabilityRate = (LIGHT_TIME_RANGE[1] - LIGHT_TIME_RANGE[0]) / AMOUNT_OF_PHASES,
  phase = 1;

bool gameOn = false,
  lightOn = false,
  isVariableLightTimeSet = false;

unsigned int variableLightTime = 0;

unsigned long lightStartingTime = 0;

Button startButton(12);

Button gameButtons[AMOUNT_OF_STATIONS] = {
  Button(STATIONS[0][0]),
  Button(STATIONS[1][0]),
  Button(STATIONS[2][0]),
  Button(STATIONS[3][0])
};

void initPins();
void testLeds();
void testButtons();
void blinkAllLights();
void handleStartButton();
void sortTarget();
void initRandomSeed();

void setup() {
  initPins();
  testLeds();
  testButtons();
  initRandomSeed();
  blinkAllLights();
  Serial.begin(9600);
}

void loop() {
  handleStartButton();
  if (gameOn) {
    if (!lightOn) {
      sortTarget();
      targets[currentLap] = currentTarget;
      digitalWrite(STATIONS[currentTarget][1], HIGH);
      lightStartingTime = millis();
      lightOn = true;
    } else {
      if (!isVariableLightTimeSet) {
        if (currentLap == 0) {
          variableLightTime = LIGHT_TIME_RANGE[1];
        } else {
          if (currentLap > PHASE_BLOCK * AMOUNT_OF_PHASES) {
            variableLightTime = LIGHT_TIME_RANGE[0];
          } else if ((currentLap * phase) % PHASE_BLOCK == 0) {
            Serial.println();
            Serial.println(currentLap);
            variableLightTime = variableLightTime - variabilityRate;
          }
        }
        isVariableLightTimeSet = true;
      }
      if (millis() - lightStartingTime > variableLightTime) {
        digitalWrite(STATIONS[currentTarget][1], LOW);
        lightOn = false;
        lightStartingTime = 0;
        currentLap++;
        isVariableLightTimeSet = false;
      }
    }
  }
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
      gameButtons[i].read();
    } while (!gameButtons[i].wasPressed());
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

void handleStartButton() {
  startButton.read();
  if (startButton.wasPressed()) {
    gameOn = !gameOn;
  }
}

void sortTarget() {
  int lastTarget = currentTarget;
  do {
    currentTarget = random(AMOUNT_OF_STATIONS);
  } while (lastTarget == currentTarget);
}

void initRandomSeed() {
  size_t const address {0};
  unsigned int seed {};
  EEPROM.get(address, seed);
  randomSeed(seed);
  EEPROM.put(address, seed + 1);
}