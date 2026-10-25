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


enum GameState { MENU, PLAYING, GAME_OVER };


// declare variables
int x;
int y;
int width;
int height;
int color;
int current_direction;
unsigned long previous_move_time;
int movement_interval;
GameState current_state;
bool is_menu_title_drawn;


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

  current_state = MENU;
  is_menu_title_drawn = false;
}

void loop() {
// loops through code

  if (current_state == MENU) {
    runMenu();
  } else if (current_state == PLAYING) {
    runGame();
  }
}


void runMenu() {
  if (!is_menu_title_drawn) {
    tft.fillScreen(ST77XX_BLACK);
    drawText("Press Any Button To Start", ST77XX_GREEN);
    is_menu_title_drawn = true;
    }

  readButtons();
  if (switch_1 == HIGH || switch_2 == HIGH || switch_3 == HIGH || switch_4 == HIGH) {
    is_menu_title_drawn = false;
    startGame();
    current_state = PLAYING;
  }
}


void startGame() {
  //clear screen
  tft.fillScreen(ST77XX_BLACK);

  // set up initial position of snake head
  x = 15;
  y = 8;
  width = 8;
  height = 8;
  color = ST77XX_GREEN;

  // set current direction
  current_direction = 0;

  // stores the time since the last move
  previous_move_time = 0;
  movement_interval = 300;

  tft.fillRect(x * 8, y * 8, width, height, color);
}


void runGame() {
  // read electric output of switches
  readButtons();

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
  unsigned long current_move_time = millis();
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


void drawText(char *text, uint16_t color) {
  tft.setCursor(5, 64);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}


void readButtons() {
  switch_1 = digitalRead(5);
  switch_2 = digitalRead(4);
  switch_3 = digitalRead(3);
  switch_4 = digitalRead(2);
}