#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define TFT_CS    10
#define TFT_RST   9
#define TFT_DC    8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

int switch_1 = 0;
int switch_2 = 0;
int switch_3 = 0;
int switch_4 = 0;

// set up initial position of snake head
int x = 15;
int y = 8;
int width = 8;
int height = 8;
int color = ST77XX_GREEN;

// set current direction
int current_direction = 0;

// stores the time since the last move
int previous_move_time = 0;
int movement_interval = 300;

void setup() {
// code runs once

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  // set up push button as input
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);

  tft.fillRect(x * 8, y * 8, width, height, color);

}

void loop() {
// loops through code

  // read electric output of switches
  switch_1 = digitalRead(5);
  switch_2 = digitalRead(4);
  switch_3 = digitalRead(3);
  switch_4 = digitalRead(2);

  // change current_direction value if switch is pressed
  if (switch_1 == HIGH) {
    current_direction = 0;
  } else if (switch_2 == HIGH) {
    current_direction = 1;
  } else if (switch_3 == HIGH) {
    current_direction = 2;
  } else if (switch_4 == HIGH) {
    current_direction = 3;
  }

  // move snake dependending on the current direction
  int current_move_time = millis();
  if (current_move_time - previous_move_time >= movement_interval) {
    if (current_direction == 0) {
      moveLeft();
      previous_move_time = current_move_time;
    } else if (current_direction == 1) {
      moveRight();
      previous_move_time = current_move_time;
    } else if (current_direction == 2) {
      moveUp();
      previous_move_time = current_move_time;
    } else if (current_direction == 3) {
      moveDown();
      previous_move_time = current_move_time;
    }
  }

}


void moveLeft() {
  if (x > 0) {
    // make current snake head black to remove from display
    tft.fillRect(x * 8, y * 8, width, height, ST77XX_BLACK);
    //place new snake head's position
    x = x - 1;
    tft.fillRect(x * 8, y * 8, width, height, color);
  }
}


void moveRight() {
  if (x < 19) {
    tft.fillRect(x * 8, y * 8, width, height, ST77XX_BLACK);
    x = x + 1;
    tft.fillRect(x * 8, y * 8, width, height, color);
  }
}


void moveUp() {
  if (y > 0) {
    tft.fillRect(x * 8, y * 8, width, height, ST77XX_BLACK);
    y = y - 1;
    tft.fillRect(x * 8, y * 8, width, height, color);
  }
}


void moveDown() {
  if (y < 15) {
    tft.fillRect(x * 8, y * 8, width, height, ST77XX_BLACK);
    y = y + 1;
    tft.fillRect(x * 8, y * 8, width, height, color);
  }
}

