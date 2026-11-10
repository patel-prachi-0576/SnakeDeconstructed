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
enum Direction { UP, DOWN, LEFT, RIGHT };

// constants
const int SCREEN_WIDTH = 20;
const int SCREEN_HEIGHT = 16;
const int BLOCK_WIDTH = 8;
const int BLOCK_HEIGHT = 8;
const int BEGIN_LENGTH = 3;

// declare variables
int color;
int current_direction;
unsigned long previous_move_time;
int movement_interval;
GameState current_state;
bool is_menu_title_drawn;
byte x_position [SCREEN_WIDTH * SCREEN_HEIGHT] = {0};
byte y_position [SCREEN_WIDTH * SCREEN_HEIGHT] = {0};
int current_snake_length;


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
// loops through code infinitely

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
        while (switch_1 == HIGH || switch_2 == HIGH || switch_3 == HIGH || switch_4 == HIGH) {
            readButtons();
        }
        startGame();
        current_state = PLAYING;
    }
}


void startGame() {

    // set length of snake
    current_snake_length = BEGIN_LENGTH;

    //clear screen
    tft.fillScreen(ST77XX_BLACK);

    // set up initial position of snake
    for (int i = 0; i < current_snake_length; i++) {
        x_position[i] = 15 + i;
        y_position[i] = 8;
    }

    color = ST77XX_GREEN;

    // set current direction
    current_direction = LEFT;

    // stores the time since the last move
    previous_move_time = millis();
    movement_interval = 300;

    for (int i = 0; i < current_snake_length; i++)
        tft.fillRect(x_position[i] * BLOCK_WIDTH, y_position[i] * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT, color);
}


void runGame() {
    // read electric output of switches
    readButtons();

    // change current_direction value if switch is pressed
    if (switch_1 == HIGH) {
        current_direction = UP;
    } else if (switch_2 == HIGH) {
        current_direction = DOWN;
    } else if (switch_3 == HIGH) {
        current_direction = LEFT;
    } else if (switch_4 == HIGH) {
        current_direction = RIGHT;
    }

    // move snake dependending on the current direction
    unsigned long current_move_time = millis();
    if (current_move_time - previous_move_time >= movement_interval) {
        moveSnake();
        previous_move_time = current_move_time;
    }
}


void moveSnake() {
    int x_change = 0;
    int y_change = 0;
    int new_x;
    int new_y;

    // Figure out change in x and y depending on the current direction
    if (current_direction == UP) {
        y_change = -1;
    } else if (current_direction == DOWN) {
        y_change = 1;
    } else if (current_direction == LEFT) {
        x_change = -1;
    } else if (current_direction == RIGHT) {
        x_change = 1;
    }

    // Calculate new x and y positions based on current direction
    new_x = x_position[0] + x_change;
    new_y = y_position[0] + y_change;

    // Erase the previous position and display new position of snake head
    if ((new_x >= 0 && new_x <= SCREEN_WIDTH - 1) && (new_y >= 0 && new_y <= SCREEN_HEIGHT - 1)) {
        tft.fillRect(x_position[current_snake_length - 1] * BLOCK_WIDTH, y_position[current_snake_length - 1] * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT, ST77XX_BLACK);

        // Shift x and y positions down so each block takes the previous block's place
        for (int i = 2; i > 0; i--) {
            x_position[i] = x_position[i - 1];
            y_position[i] = y_position[i - 1];
        }

        // Draw new snake head position
        x_position[0] = new_x;
        y_position[0] = new_y;
        tft.fillRect(x_position[0] * BLOCK_WIDTH, y_position[0] * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT, color);
    }
}


void drawText(const char *text, uint16_t text_color) {
    tft.setCursor(5, 64);
    tft.setTextColor(text_color);
    tft.setTextWrap(true);
    tft.print(text);
}


void readButtons() {
    switch_1 = digitalRead(5);
    switch_2 = digitalRead(4);
    switch_3 = digitalRead(3);
    switch_4 = digitalRead(2);
}