// Importer vores MKRIoTCarrier.
#include <Arduino_MKRIoTCarrier.h>
MKRIoTCarrier carrier;

// Definerer de farver vi bruger.
#define WHITE 0xFFFF
#define RED 0xF800
#define GREY 0x18E3
#define BLACK 0x0000
#define GREEN 0x0400

// Opretter en location structure.
struct location {
  float X;
  float Y;
};

// Definerer de variabler vi bruger.
bool top = false;
bool bot = false;
bool retry = false;
float x, y, z;
int p = 0;
int scoreCounter = 0;
int angle = 0;
int turnTime = 0;
bool dead = false;
bool playMusic = false;
int prevScore;

// Sætter skærmen til farven grå og tekst farven til hvid.
void setup() {
  Serial.begin(9600);
  carrier.noCase();
  carrier.begin();
  carrier.display.fillScreen(GREY);
  carrier.display.setTextColor(WHITE);
}

// Arduinoen tror skærmen er 240x240 pixels, mens det er en cirkulær skærm. Derfor opretter vi en "falsk" cirkulær skærm.
bool isInsideCircle(int x, int y, int centerX, int centerY, int radius) {
  int dx = x - centerX;
  int dy = y - centerY;
  return (dx * dx + dy * dy <= radius * radius);
}

void loop() {
  location snake = { 120, 120 };  // Sætter start koordinaterne til (120, 120) for slangen.
  location tail[1000];            // Maksimum længde for slangen.
  location berry = { 100, 100 };  // Sætter start koordinaterne til (100, 100) for bærret.
  dead = false;                   // Sætter dead til false, da man er live til at starte med.

  // Laver main screen.
  carrier.display.setCursor(55, 110);
  carrier.display.setTextSize(2);
  carrier.display.print("PICK A GAME");

  carrier.display.setCursor(20, 70);
  carrier.display.setTextSize(1);
  carrier.display.print("SNAKE");

  carrier.display.setCursor(195, 70);
  carrier.display.print("PONG");

  carrier.display.setCursor(80, 210);
  carrier.display.print("BRICK BREAKER");

  // Gøre Arduinoen klar til at modtage inputs fra knapperne.
  carrier.Buttons.update();

  // Hvis knap-4 trykkes, så bliver snake-startup screenen lavet.
  if (carrier.Buttons.onTouchDown(TOUCH4)) {
    carrier.display.setTextColor(GREY);
    carrier.display.setCursor(55, 110);
    carrier.display.setTextSize(2);
    carrier.display.print("PICK A GAME");

    carrier.display.setCursor(20, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("SNAKE");

    carrier.display.setCursor(195, 70);
    carrier.display.print("PONG");

    carrier.display.setCursor(80, 210);
    carrier.display.print("BRICK BREAKER");

    carrier.display.setTextColor(WHITE);
    carrier.display.setCursor(90, 110);
    carrier.display.setTextSize(2);
    carrier.display.print("SNAKE");

    carrier.display.setCursor(20, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("BACK");

    carrier.display.setCursor(195, 70);
    carrier.display.print("SKINS");

    carrier.display.setCursor(105, 210);
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
        carrier.display.print("SKINS");

        carrier.display.setCursor(105, 210);
        carrier.display.print("START");

        // Nedtælling til start.
        int i = 3;
        carrier.display.setTextSize(3);
        while (true) {
          while (i > 0) {
            carrier.display.setCursor(112, 35);
            carrier.display.setTextColor(WHITE);
            carrier.display.print(i);
            i--;
            delay(1000);
            carrier.display.setCursor(112, 35);
            carrier.display.setTextColor(GREY);
            carrier.display.print(i + 1);
          }

          playMusic = false;  // Musik.

          // Koordinater til vores "falske" skærm.
          int centerX = 120;
          int centerY = 120;
          int radius = 120;

          // Hvis slangen IKKE er indefor den "falske" skærm, så er den dø.
          if (!isInsideCircle(snake.X, snake.Y, centerX, centerY, radius)) {
            dead = true;
          }

          // Slangen tegnes.
          tail[p].X = snake.X;
          tail[p].Y = snake.Y;

          p++;
          for (int y = 0; y <= scoreCounter * 2 + 38; y++) {
            carrier.display.drawPixel(tail[y].X, tail[y].Y, WHITE);
          }

          carrier.display.drawPixel(snake.X, snake.Y, WHITE);
          if (p > scoreCounter * 2 + 40) {
            p = 0;
          }
          carrier.display.drawPixel(tail[p].X, tail[p].Y, GREY);
          delay(20);

          // Hvis playMusic er true, afspil vores musik.
          while (playMusic == true) {
            carrier.Buzzer.beep(294, 125);  //D4
            carrier.Buzzer.beep(294, 125);  //D4
            carrier.Buzzer.beep(587, 250);  //D5
            carrier.Buzzer.beep(440, 250);  //A4
            carrier.Buzzer.beep(415, 125);  //Ab4
            carrier.Buzzer.beep(392, 250);  //G4
            carrier.Buzzer.beep(349, 250);  //F4
            carrier.Buzzer.beep(294, 125);  //D4
            carrier.Buzzer.beep(349, 125);  //F4
            carrier.Buzzer.beep(392, 125);  //G4
            carrier.Buzzer.beep(261, 125);  //C4(middle)
            carrier.Buzzer.beep(261, 125);  //C4(middle)
            carrier.Buzzer.beep(261, 125);  //C4(middle)
            carrier.Buzzer.beep(261, 125);  //C4(middle)
            carrier.Buzzer.beep(587, 250);  //D5
            carrier.Buzzer.beep(440, 375);  //A4
            carrier.Buzzer.beep(415, 125);  //Ab4
            carrier.Buzzer.beep(392, 250);  //G4
            carrier.Buzzer.beep(349, 250);  //F4
            carrier.Buzzer.beep(294, 125);  //D4
            carrier.Buzzer.beep(349, 125);  //F4
            carrier.Buzzer.beep(392, 125);  //G4
            carrier.Buzzer.beep(247, 125);  //B3
            carrier.Buzzer.beep(247, 125);  //B3
            carrier.Buzzer.beep(587, 250);  //D5
            carrier.Buzzer.beep(440, 375);  //A4
            carrier.Buzzer.beep(415, 125);  //Ab4
            carrier.Buzzer.beep(392, 250);  //G4
            carrier.Buzzer.beep(349, 250);  //F4
            carrier.Buzzer.beep(294, 125);  //D4
            carrier.Buzzer.beep(349, 125);  //F4
            carrier.Buzzer.beep(392, 125);  //G4
            carrier.Buzzer.beep(233, 62);   //Bb3
            carrier.Buzzer.beep(233, 62);   //Bb3
            carrier.Buzzer.beep(233, 62);   //Bb3
            carrier.Buzzer.beep(233, 62);   //Bb3
            carrier.Buzzer.beep(587, 250);  //D5
            carrier.Buzzer.beep(440, 375);  //A4
            carrier.Buzzer.beep(415, 125);  //Ab4
            carrier.Buzzer.beep(392, 250);  //G4
            carrier.Buzzer.beep(349, 250);  //F4
            carrier.Buzzer.beep(294, 125);  //D4
            carrier.Buzzer.beep(349, 125);  //F4
            carrier.Buzzer.beep(392, 125);  //G4
            carrier.Buzzer.beep(294, 125);  //D4
            carrier.Buzzer.beep(294, 125);  //D4
            carrier.Buzzer.beep(587, 250);  //D5
            carrier.Buzzer.beep(440, 375);  //A4
            carrier.Buzzer.beep(415, 125);  //Ab4
            carrier.Buzzer.beep(392, 250);  //G4
            carrier.Buzzer.beep(349, 250);  //F4
            carrier.Buzzer.beep(294, 125);  //D4
            carrier.Buzzer.beep(349, 125);  //F4
            carrier.Buzzer.beep(392, 125);  //G4
            carrier.Buzzer.beep(261, 125);  //C4(middle)
            carrier.Buzzer.beep(261, 125);  //C4(middle)
            carrier.Buzzer.beep(261, 125);  //C4(middle)
            carrier.Buzzer.beep(261, 125);  //C4(middle)
            carrier.Buzzer.beep(587, 250);  //D5
            carrier.Buzzer.beep(440, 375);  //A4
            carrier.Buzzer.beep(415, 125);  //Ab4
            carrier.Buzzer.beep(392, 250);  //G4
            carrier.Buzzer.beep(349, 250);  //F4
            carrier.Buzzer.beep(294, 125);  //D4
            carrier.Buzzer.beep(349, 125);  //F4
            carrier.Buzzer.beep(392, 125);  //G4
            carrier.Buzzer.beep(247, 125);  //B3
            carrier.Buzzer.beep(247, 125);  //B3
            carrier.Buzzer.beep(587, 250);  //D5
            carrier.Buzzer.beep(440, 375);  //A4
            carrier.Buzzer.beep(415, 125);  //Ab4
            carrier.Buzzer.beep(392, 250);  //G4
            carrier.Buzzer.beep(349, 250);  //F4
            carrier.Buzzer.beep(294, 125);  //D4
            carrier.Buzzer.beep(349, 125);  //F4
            carrier.Buzzer.beep(392, 125);  //G4
            carrier.Buzzer.beep(233, 62);   //Bb3
            carrier.Buzzer.beep(233, 62);   //Bb3
            carrier.Buzzer.beep(233, 62);   //Bb3
            carrier.Buzzer.beep(233, 62);   //Bb3
            carrier.Buzzer.beep(588, 250);  //D5
            carrier.Buzzer.beep(440, 375);  //A4
            carrier.Buzzer.beep(415, 125);  //Ab4
            carrier.Buzzer.beep(392, 250);  //G4
            carrier.Buzzer.beep(349, 250);  //F4
            carrier.Buzzer.beep(294, 125);  //D4
            carrier.Buzzer.beep(349, 125);  //F4
            carrier.Buzzer.beep(392, 125);  //G4
          }

          // Bærret tegnes.
          carrier.display.drawPixel(berry.X - 1, berry.Y - 1, RED);
          carrier.display.drawPixel(berry.X - 1, berry.Y, RED);
          carrier.display.drawPixel(berry.X - 1, berry.Y + 1, RED);
          carrier.display.drawPixel(berry.X, berry.Y - 1, RED);
          carrier.display.drawPixel(berry.X, berry.Y, RED);
          carrier.display.drawPixel(berry.X, berry.Y + 1, RED);
          carrier.display.drawPixel(berry.X + 1, berry.Y - 1, RED);
          carrier.display.drawPixel(berry.X + 1, berry.Y, RED);
          carrier.display.drawPixel(berry.X + 1, berry.Y + 1, RED);

          // Hvis slangen rammer et bær, tilføj point og fjern bær.
          if (snake.X == berry.X && snake.Y == berry.Y || snake.X == berry.X - 1 && snake.Y == berry.Y - 1 || snake.X == berry.X - 1 && snake.Y == berry.Y || snake.X == berry.X - 1 && snake.Y == berry.Y + 1 || snake.X == berry.X && snake.Y == berry.Y - 1 || snake.X == berry.X && snake.Y == berry.Y + 1 || snake.X == berry.X + 1 && snake.Y == berry.Y - 1 || snake.X == berry.X + 1 && snake.Y == berry.Y || snake.X == berry.X + 1 && snake.Y == berry.Y + 1) {
            carrier.display.drawPixel(berry.X - 1, berry.Y - 1, GREY);
            carrier.display.drawPixel(berry.X - 1, berry.Y, GREY);
            carrier.display.drawPixel(berry.X - 1, berry.Y + 1, GREY);
            carrier.display.drawPixel(berry.X, berry.Y - 1, GREY);
            carrier.display.drawPixel(berry.X, berry.Y, GREY);
            carrier.display.drawPixel(berry.X, berry.Y + 1, GREY);
            carrier.display.drawPixel(berry.X + 1, berry.Y - 1, GREY);
            carrier.display.drawPixel(berry.X + 1, berry.Y, GREY);
            carrier.display.drawPixel(berry.X + 1, berry.Y + 1, GREY);
            berry.X = random(50, 150);
            berry.Y = random(50, 150);
            scoreCounter++;
            Serial.println(scoreCounter);
          }

          // Hvis vores slange er på top-delen af skærmen, så vis scoren i bunden.
          if (snake.Y > 120) {
            if (top = true) {
              carrier.display.setCursor(112, 200);
              carrier.display.setTextColor(GREY);
              carrier.display.print(scoreCounter);
              top = false;
            }
            carrier.display.setCursor(112, 20);
            carrier.display.setTextColor(GREY);

            prevScore = scoreCounter - 1;
            carrier.display.print(prevScore);

            carrier.display.setCursor(112, 20);
            carrier.display.setTextColor(GREEN);
            carrier.display.print(scoreCounter);
            bot = true;
          }

          // Hvis vores slange er på bund-delen af skærmen, så vis scoren i toppen.
          if (snake.Y <= 120) {
            if (bot = true) {
              carrier.display.setCursor(112, 20);
              carrier.display.setTextColor(GREY);
              carrier.display.print(scoreCounter);
              bot = false;
            }
            carrier.display.setCursor(112, 200);
            carrier.display.setTextColor(GREY);
            prevScore = scoreCounter - 1;
            carrier.display.print(prevScore);

            carrier.display.setCursor(112, 200);
            carrier.display.setTextColor(GREEN);
            carrier.display.print(scoreCounter);
            top = true;
          }

          // Hvis carrierens acceleration er ledig, så bliver gyroskopet brugt.
          if (carrier.IMUmodule.accelerationAvailable()) {
            carrier.IMUmodule.readAcceleration(x, y, z);

            if (x >= 0.25 && turnTime <= 0) {
              turnTime = 10;
              angle += 90;
            }
            if (x <= -0.25 && turnTime <= 0) {
              turnTime = 10;
              angle -= 90;
            }
            if (angle > 270) {
              angle = 0;
            }
            if (angle < 0) {
              angle = 270;
            }

            if (angle == 0) {
              snake.X++;
            }
            if (angle == 90) {
              snake.Y++;
            }
            if (angle == 180) {
              snake.X--;
            }
            if (angle == 270) {
              snake.Y--;
            }
            turnTime--;
            for (int t = 0; t < scoreCounter * 2 + 40; t++) {
              if (tail[t].X == snake.X && tail[t].Y == snake.Y) {
                dead = true;
              }
            }

            // Hvis dead er true, så lav defeat screen + nulstil slangen.
            if (dead == true) {
              playMusic = false;
              dead = true;
              for (int y = 0; y < scoreCounter + 40; y++) {
                tail[y].X = 120;
                tail[y].Y = 120;
              }
              carrier.display.fillScreen(RED);
              carrier.display.setTextSize(3);
              carrier.display.setCursor(70, 35);
              carrier.display.setTextColor(BLACK);
              carrier.display.print("DEFEAT");

              carrier.display.setTextSize(2);
              carrier.display.setCursor(70, 90);
              carrier.display.setTextColor(WHITE);
              carrier.display.print("Score:");
              carrier.display.print(scoreCounter);

              carrier.display.setCursor(55, 120);
              carrier.display.setTextColor(WHITE);
              carrier.display.print("Highscore:");
              //hent highscore fra database
              carrier.display.print("x");

              delay(2500);
              carrier.display.setTextColor(WHITE);
              carrier.display.setCursor(40, 190);
              carrier.display.setTextSize(1);
              carrier.display.print("BACK");

              carrier.display.setCursor(170, 190);
              carrier.display.print("RETRY");
              snake.X = 120;
              snake.Y = 120;
              scoreCounter = 0;
              break;
            }
          }
        }
        // Defeat menu.
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
    carrier.display.print("SKINS");

    carrier.display.setCursor(105, 210);
    carrier.display.print("START");

    carrier.display.setTextColor(WHITE);
    carrier.display.setCursor(55, 110);
    carrier.display.setTextSize(2);
    carrier.display.print("PICK A GAME");

    carrier.display.setCursor(20, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("SNAKE");

    carrier.display.setCursor(195, 70);
    carrier.display.print("PONG");

    carrier.display.setCursor(80, 210);
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
    carrier.display.print("PONG");

    carrier.display.setCursor(80, 210);
    carrier.display.print("BRICK BREAKER");

    carrier.display.setTextColor(WHITE);
    carrier.display.setCursor(45, 110);
    carrier.display.setTextSize(2);
    carrier.display.print("BRICK BREAKER");

    carrier.display.setCursor(20, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("BACK");

    carrier.display.setCursor(195, 70);
    carrier.display.print("SKINS");

    carrier.display.setCursor(105, 210);
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
    carrier.display.print("SKINS");

    carrier.display.setCursor(105, 210);
    carrier.display.print("START");

    carrier.display.setTextColor(WHITE);
    carrier.display.setCursor(55, 110);
    carrier.display.setTextSize(2);
    carrier.display.print("PICK A GAME");

    carrier.display.setCursor(20, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("SNAKE");

    carrier.display.setCursor(195, 70);
    carrier.display.print("PONG");

    carrier.display.setCursor(80, 210);
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
    carrier.display.print("PONG");

    carrier.display.setCursor(80, 210);
    carrier.display.print("BRICK BREAKER");

    carrier.display.setTextColor(WHITE);
    carrier.display.setCursor(95, 110);
    carrier.display.setTextSize(2);
    carrier.display.print("PONG");

    carrier.display.setCursor(20, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("BACK");

    carrier.display.setCursor(195, 70);
    carrier.display.print("SKINS");

    carrier.display.setCursor(105, 210);
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
    carrier.display.print("SKINS");

    carrier.display.setCursor(105, 210);
    carrier.display.print("START");

    carrier.display.setTextColor(WHITE);
    carrier.display.setCursor(55, 110);
    carrier.display.setTextSize(2);
    carrier.display.print("PICK A GAME");

    carrier.display.setCursor(20, 70);
    carrier.display.setTextSize(1);
    carrier.display.print("SNAKE");

    carrier.display.setCursor(195, 70);
    carrier.display.print("PONG");

    carrier.display.setCursor(80, 210);
    carrier.display.print("BRICK BREAKER");
  }
}