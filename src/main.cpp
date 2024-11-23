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
  targets[MAX_RESULTS],
  lastHit;

bool gameOn, lightOn;

unsigned long nextLightChange, lightTimeStart;

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
bool shouldChangeLight();
void initRandomSeed();
unsigned long calcCurrentLightDuration();
void handleGameButtons();
bool wasLastHitGood();
void sortAllStations();


void setup() {
  Serial.begin(9600);
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
    if (lastHit > -1) {
      if (!wasLastHitGood()) {
        handleEndGame();
        return;
      }
      lastHit = -1;
    }
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
  lastHit = -1;
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
  unsigned long duration = calcCurrentLightDuration();
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
    delay(5);
    lightTimeStart = millis();
  }

}

bool shouldChangeLight() {
  if (lightLap == -1) return true;
  unsigned long now = millis();
  if (now > nextLightChange) {
    return true;
  }
  return false;
}

void initRandomSeed() {
  size_t const address {0};
  unsigned int seed {};
  EEPROM.get(address, seed);
  randomSeed(seed);
  EEPROM.put(address, seed + 1);
}

unsigned long calcCurrentLightDuration() {
  const unsigned long  MAX = 1000;
  const unsigned long MIN = 200;
  if (lightLap < 10) return MAX;
  if (lightLap < 20) return MAX * .95;
  if (lightLap < 30) return MAX * .9;
  if (lightLap < 40) return MAX * .85;
  if (lightLap < 50) return MAX * .8;
  if (lightLap < 60) return MAX * .75;
  if (lightLap < 70) return MAX * .7;
  if (lightLap < 80) return MAX * .65;
  if (lightLap < 90) return MAX * .6;
  if (lightLap < 100) return MAX * .55;
  if (lightLap < 120) return MAX * .5;
  if (lightLap < 140) return MAX * .45;
  if (lightLap < 160) return MAX * .3;
  if (lightLap < 200) return MAX * .35;
  if (lightLap < 250) return MAX * .3;
  if (lightLap < 300) return MAX * .25;
  return MIN;
}

void handleGameButtons() {
  for (int i = 0; i < AMOUNT_OF_STATIONS; i++) {
    gameButtons[i].read();
    if (gameButtons[i].wasPressed()) {
      lastHit = i;
      buttonLap++;
    }
  }
}

bool wasLastHitGood() {
  return (targets[buttonLap] == lastHit);
}

void sortAllStations() {
  for (int i = 0; i < MAX_RESULTS; i++) {
    do {
      targets[i] = random(AMOUNT_OF_STATIONS);
    } while (i > 0 && targets[i - 1] == targets[i]);
  }
}
