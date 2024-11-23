#include <Arduino.h>
#include <EEPROM.h>
#include "ButtonClass.h"

const int AMOUNT_OF_STATIONS = 4,
  START_BTN_PIN = 12,
  GAME_LED_PIN = 13,
  MAX_RESULTS = 500,
  STATIONS[4][2] = {
    {4, 5},
    {6, 7},
    {8, 9},
    {10, 11}
  };

int lightLap,
  buttonLap,
  targets[MAX_RESULTS];

bool gameOn, lightOn;

unsigned long lightTimeStart;

Button startButton(START_BTN_PIN);

Button gameButtons[AMOUNT_OF_STATIONS] = {
  Button(STATIONS[0][0]),
  Button(STATIONS[1][0]),
  Button(STATIONS[2][0]),
  Button(STATIONS[3][0])
};

void initPins();
void testButtons();
void blinkAllLights();
void resetGameProps();
void handleStartButtonPress();
void handleStartGame();
void handleEndGame();
void handleGameLights();
void initRandomSeed();
unsigned long calcCurrentLightDuration(int lap);
void handleGameButtons();
void sortAllStations();


void setup() {
  initPins();
  testButtons();
  initRandomSeed();
  resetGameProps();
  blinkAllLights();
}

void loop() {
  handleStartButtonPress();
  if (gameOn) {
    handleGameLights();
    handleGameButtons();
  }
}

void initPins() {
  for (int i = 0; i < AMOUNT_OF_STATIONS; i++) {
    pinMode(STATIONS[i][0], INPUT_PULLUP);
    pinMode(STATIONS[i][1], OUTPUT);
  }
  pinMode(START_BTN_PIN, INPUT_PULLUP);
  pinMode(GAME_LED_PIN, OUTPUT);
}

void testButtons() {
  digitalWrite(GAME_LED_PIN, HIGH);
  do {
    startButton.read();
  } while (!startButton.wasPressed());
  digitalWrite(GAME_LED_PIN, LOW);
  for (int i = 0; i < AMOUNT_OF_STATIONS; i++) {
    digitalWrite(STATIONS[i][1], HIGH);
    do {
      gameButtons[i].read();
    } while (!gameButtons[i].wasPressed());
    digitalWrite(STATIONS[i][1], LOW);
  }
}

void blinkAllLights() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(GAME_LED_PIN, HIGH);
    for (int x = 0; x < AMOUNT_OF_STATIONS; x++) {
      digitalWrite(STATIONS[x][1], HIGH);
    }
    delay(500);
    digitalWrite(GAME_LED_PIN, LOW);
    for (int x = 0; x < AMOUNT_OF_STATIONS; x++) {
      digitalWrite(STATIONS[x][1], LOW);
    }
    delay(200);
  }
}

void resetGameProps() {
  gameOn = false;
  lightLap = -1;
  buttonLap = -1;
  lightOn = false;
  sortAllStations();
}

void handleStartButtonPress() {
  startButton.read();
  if (startButton.wasPressed()) {
    if (gameOn) {
      handleEndGame();
      return;
    }
    handleStartGame();
  }
}

void handleStartGame() {
  gameOn = true;
  digitalWrite(GAME_LED_PIN, HIGH);
}

void handleEndGame() {
  blinkAllLights();
  resetGameProps();
}

void handleGameLights() {
  unsigned long now =  millis();
  unsigned long duration = calcCurrentLightDuration(lightLap);
  if (lightLap == -1) {
    lightTimeStart = now;
  }
  if (!lightOn && now >= lightTimeStart && now < lightTimeStart + duration) {
    lightLap++;
    digitalWrite(STATIONS[targets[lightLap]][1], HIGH);
    lightOn = true;
    return;
  }
  if (lightOn && now > lightTimeStart + duration) {
    digitalWrite(STATIONS[targets[lightLap]][1], LOW);
    lightOn = false;
    delay(1);
    lightTimeStart = millis() + 5;
  }

}

void initRandomSeed() {
  size_t const address {0};
  unsigned int seed {};
  EEPROM.get(address, seed);
  randomSeed(seed);
  EEPROM.put(address, seed + 1);
}

unsigned long calcCurrentLightDuration(int lap) {
  if (lap < 10) return 1000;
  if (lap < 20) return 950;
  if (lap < 30) return 900;
  if (lap < 40) return 850;
  if (lap < 50) return 800;
  if (lap < 60) return 750;
  if (lap < 70) return 700;
  if (lap < 80) return 650;
  if (lap < 90) return 600;
  if (lap < 100) return 550;
  if (lap < 110) return 525;
  if (lap < 120) return 500;
  if (lap < 130) return 475;
  if (lap < 140) return 450;
  if (lap < 140) return 425;
  if (lap < 160) return 400;
  if (lap < 170) return 375;
  if (lap < 180) return 350;
  if (lap < 190) return 325;
  if (lap < 200) return 300;
  if (lap < 225) return 275;
  if (lap < 250) return 250;
  if (lap < 275) return 225;
  return 200;
}

void handleGameButtons() {
  for (int i = 0; i < AMOUNT_OF_STATIONS; i++) {
    gameButtons[i].read();
    if (gameButtons[i].wasPressed()) {
      buttonLap++;
      if (targets[buttonLap] == i) {
        if (buttonLap == lightLap) digitalWrite(STATIONS[targets[lightLap]][1], LOW);
        return;
      }
      handleEndGame();
    }
  }
}

void sortAllStations() {
  for (int i = 0; i < MAX_RESULTS; i++) {
    do {
      targets[i] = random(AMOUNT_OF_STATIONS);
    } while (i > 0 && targets[i - 1] == targets[i]);
  }
}
