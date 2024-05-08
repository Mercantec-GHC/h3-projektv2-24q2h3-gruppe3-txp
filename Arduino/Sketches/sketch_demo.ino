// Importer vores MKRIoTCarrier
#include <Arduino_MKRIoTCarrier.h>
MKRIoTCarrier carrier;

#include <Arduino_LSM6DSOX.h>

// Definere de farver vi bruger
#define WHITE 0xFFFF
#define RED 0xF800
#define GREY 0x18E3
#define BLACK 0x0000

// Opretter vores snake punkter
bool retry;
float x, y, z;
struct location {
  float X;
  float Y;
};
int p = 0;

void setup() {
  Serial.begin(9600);
  carrier.noCase();
  carrier.begin();
  // Sætter skærmen til farven grå og tekst farven til hvid
  carrier.display.fillScreen(GREY);
  carrier.display.setTextColor(WHITE);

  retry = false;
}

void loop() {
  location snake = { 120, 120 };
  location tail[30];
  // Laver main screen
  carrier.display.setCursor(55, 110);
  carrier.display.setTextSize(2);
  carrier.display.print("PICK A GAME");

  carrier.display.setCursor(20, 70);
  carrier.display.setTextSize(1);
  carrier.display.print("SNAKE");

  carrier.display.setCursor(195, 70);
  carrier.display.setTextSize(1);
  carrier.display.print("PONG");

  carrier.display.setCursor(80, 210);
  carrier.display.setTextSize(1);
  carrier.display.print("BRICK BREAKER");

  // Gøre Arduinoen klar til at modtage inputs fra knapperne
  carrier.Buttons.update();

  // Hvis knap-4 trykkes, så bliver snake-startup screenen lavet
  if (carrier.Buttons.onTouchDown(TOUCH4)) {
    carrier.display.setTextColor(GREY);

    carrier.display.setCursor(55, 110);
    carrier.display.setTextSize(2);
    carrier.display.print("PICK A GAME");

    carrier.display.setCursor(20, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("SNAKE");

    carrier.display.setCursor(195, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("PONG");

    carrier.display.setCursor(80, 210);
    carrier.display.setTextSize(1);
    carrier.display.print("BRICK BREAKER");

    carrier.display.setTextColor(WHITE);
    carrier.display.setCursor(90, 110);
    carrier.display.setTextSize(2);
    carrier.display.print("SNAKE");

    carrier.display.setCursor(20, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("BACK");

    carrier.display.setCursor(195, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("SKINS");

    carrier.display.setCursor(105, 210);
    carrier.display.setTextSize(1);
    carrier.display.print("START");

    delay(500);
    while (true) {
      carrier.Buttons.update();
      if (carrier.Buttons.onTouchDown(TOUCH4)) {
        break;
      }
      if (carrier.Buttons.onTouchDown(TOUCH2) || retry == true) {
        retry = false;
        carrier.display.setTextColor(GREY);

        carrier.display.setCursor(90, 110);
        carrier.display.setTextSize(2);
        carrier.display.print("SNAKE");

        carrier.display.setCursor(20, 70);
        carrier.display.setTextSize(1);
        carrier.display.print("BACK");

        carrier.display.setCursor(195, 70);
        carrier.display.setTextSize(1);
        carrier.display.print("SKINS");

        carrier.display.setCursor(105, 210);
        carrier.display.setTextSize(1);
        carrier.display.print("START");

        bool dead = false;
        int i = 3;
        while (true) {
          while (i > 0) {
            carrier.display.setTextSize(3);
            carrier.display.setCursor(112, 35);
            carrier.display.setTextColor(GREY);
            carrier.display.print(i + 1);
            carrier.display.setCursor(112, 35);
            carrier.display.setTextColor(WHITE);
            carrier.display.print(i);
            delay(1000);
            i--;
          }
          carrier.display.setCursor(112, 35);
          carrier.display.setTextSize(3);
          carrier.display.setTextColor(GREY);
          carrier.display.print("1");

          carrier.display.drawPixel(snake.X, snake.Y, WHITE);

          tail[p].X = snake.X;
          tail[p].Y = snake.Y;
          for (int y = 0; y < 30; y++) {
            carrier.display.drawPixel(tail[y].X, tail[y].Y, WHITE);
          }
          delay(20);
          p++;
          if (p == 30) {
            p = 0;
          }

          Serial.println(tail[p].X);
          Serial.println(snake.X);

          if (carrier.IMUmodule.accelerationAvailable()) {
            carrier.IMUmodule.readAcceleration(x, y, z);
            if (x > 0) {
              snake.X++;
            }
            if (x < 0) {
              snake.X--;
            }

            if (y > 0) {
              snake.Y--;
            }
            if (y < 0) {
              snake.Y++;
            }
            carrier.display.drawPixel(tail[p].X, tail[p].Y, GREY);
            for (int t = 0; t < 30; t++) {
              if (tail[t].X == snake.X && tail[t].Y == snake.Y) {
                dead = true;
              }
            }

            if (snake.X == 240 || snake.X == 0 || snake.Y == 240 || snake.Y == 0 || dead == true) {
              dead = true;
              carrier.display.fillScreen(RED);
              carrier.display.setTextSize(3);
              carrier.display.setCursor(70, 35);
              carrier.display.setTextColor(BLACK);
              carrier.display.print("DEFEAT");

              carrier.display.setTextColor(WHITE);
              carrier.display.setCursor(40, 190);
              carrier.display.setTextSize(1);
              carrier.display.print("BACK");

              carrier.display.setCursor(170, 190);
              carrier.display.print("RETRY");
              snake.X = 120;
              snake.Y = 120;
              break;
            }
          }
        }
        while (dead) {
          carrier.Buttons.update();

          if (carrier.Buttons.onTouchDown(TOUCH1)) {
            carrier.display.fillScreen(GREY);
            retry = true;
            dead = false;
            while (dead = false) {
              Serial.println("test");
            }
            break;
          }

          if (carrier.Buttons.onTouchDown(TOUCH3)) {
            carrier.display.fillScreen(GREY);
            break;
          }
        }
        if (retry) {
        } else {
          break;
        }
      }
    }
    carrier.display.setTextColor(GREY);

    carrier.display.setCursor(90, 110);
    carrier.display.setTextSize(2);
    carrier.display.print("SNAKE");

    carrier.display.setCursor(20, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("BACK");

    carrier.display.setCursor(195, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("SKINS");

    carrier.display.setCursor(105, 210);
    carrier.display.setTextSize(1);
    carrier.display.print("START");

    carrier.display.setTextColor(WHITE);

    carrier.display.setCursor(55, 110);
    carrier.display.setTextSize(2);
    carrier.display.print("PICK A GAME");

    carrier.display.setCursor(20, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("SNAKE");

    carrier.display.setCursor(195, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("PONG");

    carrier.display.setCursor(80, 210);
    carrier.display.setTextSize(1);
    carrier.display.print("BRICK BREAKER");
  }

  if (carrier.Buttons.onTouchDown(TOUCH2)) {
    carrier.display.setTextColor(GREY);

    carrier.display.setCursor(55, 110);
    carrier.display.setTextSize(2);
    carrier.display.print("PICK A GAME");

    carrier.display.setCursor(20, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("SNAKE");

    carrier.display.setCursor(195, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("PONG");

    carrier.display.setCursor(80, 210);
    carrier.display.setTextSize(1);
    carrier.display.print("BRICK BREAKER");

    carrier.display.setTextColor(WHITE);
    carrier.display.setCursor(45, 110);
    carrier.display.setTextSize(2);
    carrier.display.print("BRICK BREAKER");

    carrier.display.setCursor(20, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("BACK");

    carrier.display.setCursor(195, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("SKINS");

    carrier.display.setCursor(105, 210);
    carrier.display.setTextSize(1);
    carrier.display.print("START");

    delay(500);
    while (true) {
      carrier.Buttons.update();
      if (carrier.Buttons.onTouchDown(TOUCH4)) {
        break;
      }
    }
    carrier.display.setTextColor(GREY);

    carrier.display.setCursor(45, 110);
    carrier.display.setTextSize(2);
    carrier.display.print("BRICK BREAKER");

    carrier.display.setCursor(20, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("BACK");

    carrier.display.setCursor(195, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("SKINS");

    carrier.display.setCursor(105, 210);
    carrier.display.setTextSize(1);
    carrier.display.print("START");

    carrier.display.setTextColor(WHITE);

    carrier.display.setCursor(55, 110);
    carrier.display.setTextSize(2);
    carrier.display.print("PICK A GAME");

    carrier.display.setCursor(20, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("SNAKE");

    carrier.display.setCursor(195, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("PONG");

    carrier.display.setCursor(80, 210);
    carrier.display.setTextSize(1);
    carrier.display.print("BRICK BREAKER");
  }

  if (carrier.Buttons.onTouchDown(TOUCH0)) {
    carrier.display.setTextColor(GREY);

    carrier.display.setCursor(55, 110);
    carrier.display.setTextSize(2);
    carrier.display.print("PICK A GAME");

    carrier.display.setCursor(20, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("SNAKE");

    carrier.display.setCursor(195, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("PONG");

    carrier.display.setCursor(80, 210);
    carrier.display.setTextSize(1);
    carrier.display.print("BRICK BREAKER");

    carrier.display.setTextColor(WHITE);
    carrier.display.setCursor(95, 110);
    carrier.display.setTextSize(2);
    carrier.display.print("PONG");

    carrier.display.setCursor(20, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("BACK");

    carrier.display.setCursor(195, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("SKINS");

    carrier.display.setCursor(105, 210);
    carrier.display.setTextSize(1);
    carrier.display.print("START");

    delay(500);
    while (true) {
      carrier.Buttons.update();
      if (carrier.Buttons.onTouchDown(TOUCH4)) {
        break;
      }
    }
    carrier.display.setTextColor(GREY);

    carrier.display.setCursor(95, 110);
    carrier.display.setTextSize(2);
    carrier.display.print("PONG");

    carrier.display.setCursor(20, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("BACK");

    carrier.display.setCursor(195, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("SKINS");

    carrier.display.setCursor(105, 210);
    carrier.display.setTextSize(1);
    carrier.display.print("START");

    carrier.display.setTextColor(WHITE);

    carrier.display.setCursor(55, 110);
    carrier.display.setTextSize(2);
    carrier.display.print("PICK A GAME");

    carrier.display.setCursor(20, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("SNAKE");

    carrier.display.setCursor(195, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("PONG");

    carrier.display.setCursor(80, 210);
    carrier.display.setTextSize(1);
    carrier.display.print("BRICK BREAKER");
  }
}