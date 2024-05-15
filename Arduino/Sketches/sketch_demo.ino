// Importer vores MKRIoTCarrier
#include <Arduino_MKRIoTCarrier.h>
MKRIoTCarrier carrier;

#include <Arduino_LSM6DSOX.h>

// Definere de farver vi bruger
#define WHITE 0xFFFF
#define RED 0xF800
#define GREY 0x18E3
#define BLACK 0x0000
#define GREEN 0x0400
bool top = false;
bool bot = false;


bool retry;
float x, y, z;
struct location {
  float X;
  float Y;
};

int p = 0;
int scoreCounter = 0;
int scoreX = 110;
int scoreY = 35;
int angle = 0;
int turnTime = 0;
bool dead = false;
bool playMusic = false;

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
  location tail[56700];
  location berry = { 100, 100 };
  dead = false;
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
        int i = 3;
        carrier.display.setTextSize(3);
        while (true) {
          //Countdown
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
          playMusic = false;
          //drawing of snake
          carrier.display.drawPixel(snake.X, snake.Y, WHITE);

          tail[p].X = snake.X;
          tail[p].Y = snake.Y;

          for (int y = 0; y <= scoreCounter; y++) {
            carrier.display.drawPixel(tail[y].X, tail[y].Y, WHITE);
          }
          delay(20);
          p++;
          if (p >= scoreCounter) {
            p = 0;
          }


          //Music
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

          carrier.display.drawPixel(berry.X, berry.Y, RED);

          Serial.println(snake.X);
          if (snake.X == berry.X && snake.Y == berry.Y || snake.X == berry.X - 1 && snake.Y == berry.Y - 1 || snake.X == berry.X - 1 && snake.Y == berry.Y || snake.X == berry.X - 1 && snake.Y == berry.Y + 1 || snake.X == berry.X && snake.Y == berry.Y - 1 || snake.X == berry.X && snake.Y == berry.Y + 1 || snake.X == berry.X + 1 && snake.Y == berry.Y - 1 || snake.X == berry.X + 1 && snake.Y == berry.Y || snake.X == berry.X + 1 && snake.Y == berry.Y + 1) {
            carrier.display.drawPixel(berry.X, berry.Y, GREY);
            berry.X = random(50, 150);
            berry.Y = random(50, 150);
            scoreCounter++;
          }

          if (snake.Y > 120) {
            if (top = true) {
              carrier.display.setCursor(105, 200);
              carrier.display.setTextColor(GREY);
              carrier.display.print(scoreCounter);
              top = false;
            }
            carrier.display.setCursor(105, 20);
            carrier.display.setTextColor(GREY);

            //carrier.display.print(scoreCounter);
            //scoreCounter++;
            carrier.display.setCursor(105, 20);
            carrier.display.setTextColor(GREEN);
            carrier.display.print(scoreCounter);
            bot = true;
          }
          if (snake.Y <= 120) {
            if (bot = true) {
              carrier.display.setCursor(105, 20);
              carrier.display.setTextColor(GREY);
              carrier.display.print(scoreCounter);
              bot = false;
            }
            carrier.display.setCursor(105, 200);
            carrier.display.setTextColor(GREY);
            carrier.display.print(scoreCounter);
            //scoreCounter++;
            carrier.display.setCursor(105, 200);
            carrier.display.setTextColor(GREEN);
            carrier.display.print(scoreCounter);
            top = true;
          }

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
            carrier.display.drawPixel(tail[p].X, tail[p].Y, GREY);
            for (int t = 0; t < scoreCounter; t++) {
              if (tail[t].X == snake.X && tail[t].Y == snake.Y) {
                dead = true;
              }
            }

            if (snake.X == 240 || snake.X == 0 || snake.Y == 240 || snake.Y == 0 || dead == true) {
              playMusic = false;
              dead = true;
              for (int y = 0; y < scoreCounter; y++) {
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
        //death menu
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