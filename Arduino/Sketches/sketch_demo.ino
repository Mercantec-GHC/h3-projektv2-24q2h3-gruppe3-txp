// Importer vores MKRIoTCarrier
#include <Arduino_MKRIoTCarrier.h>
MKRIoTCarrier carrier;

// Definere de farver vi bruger
#define ST7735_GREY 0x18E3
#define ST7735_WHITE 0xFFFF
#define ST7735_GREEN 0x07E0
#define ST7735_RED 0xF800

// Opretter vores snake punkter
float snakeX, snakeY, osnakeX, osnakeY;
float vx, vy;

void setup() {
  Serial.begin(9600);
  carrier.noCase();
  carrier.begin();
  // Sætter skærmen til farven grå og tekst farven til hvid
  carrier.display.fillScreen(ST7735_GREY);
  carrier.display.setTextColor(ST7735_WHITE);

  // Ændrer på vores treshold så vores knapper ikke er så sensitivity
  unsigned int threshold = 500;
  carrier.Buttons.updateConfig(threshold);

  // Indsætter vores xy-punkter
  snakeX = 128;
  snakeY = 150;
  osnakeX = 128;
  osnakeY = 150;
  vx = 0.;
  vy = 0.76;
}

void loop() {
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
    carrier.display.setTextColor(ST7735_GREY);

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

    carrier.display.setTextColor(ST7735_WHITE);
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

    while (true) {
      carrier.Buttons.update();
      if (carrier.Buttons.onTouchDown(TOUCH4)) {
        break;
      }
      if (carrier.Buttons.onTouchDown(TOUCH2)) {
        carrier.display.setTextColor(ST7735_GREY);

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

        while (true) {
          carrier.display.drawPixel(snakeX, snakeY, ST7735_WHITE);
          carrier.display.drawPixel(snakeX + 1, snakeY - 1, 23275);
          carrier.display.drawPixel(snakeX + 1, snakeY + 1, 23275);
          carrier.display.drawPixel(snakeX - 1, snakeY - 1, 23275);
          carrier.display.drawPixel(snakeX - 1, snakeY + 1, 23275);

          carrier.display.drawPixel(snakeX - 1, snakeY, 33808);
          carrier.display.drawPixel(snakeX + 1, snakeY, 33808);
          carrier.display.drawPixel(snakeX, snakeY - 1, 33808);
          carrier.display.drawPixel(snakeX, snakeY - 1, 33808);

          // If snake dead = break;
          bool alive = false;
          if (alive == false) {
            break;
          }
        }
      }
    }
    carrier.display.setTextColor(ST7735_GREY);

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

    carrier.display.setTextColor(ST7735_WHITE);

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
    carrier.display.setTextColor(ST7735_GREY);

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

    carrier.display.setTextColor(ST7735_WHITE);
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

    while (true) {
      carrier.Buttons.update();
      if (carrier.Buttons.onTouchDown(TOUCH4)) {

        break;
      }
    }
    carrier.display.setTextColor(ST7735_GREY);

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

    carrier.display.setTextColor(ST7735_WHITE);

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
    carrier.display.setTextColor(ST7735_GREY);

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

    carrier.display.setTextColor(ST7735_WHITE);
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

    while (true) {
      carrier.Buttons.update();
      if (carrier.Buttons.onTouchDown(TOUCH4)) {
        break;
      }
    }
    carrier.display.setTextColor(ST7735_GREY);

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

    carrier.display.setTextColor(ST7735_WHITE);

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