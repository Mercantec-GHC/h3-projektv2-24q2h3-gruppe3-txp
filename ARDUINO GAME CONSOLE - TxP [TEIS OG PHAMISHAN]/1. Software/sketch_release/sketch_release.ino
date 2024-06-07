// Libraries and secrets.
#include <WiFiNINA.h>
#include <Arduino_MKRIoTCarrier.h>
#include <CustomJWT.h>
#include <ArduinoJson.h>
#include "arduino_secrets.h"
MKRIoTCarrier carrier;

// Network login.
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS;

// API server.
char server[] = "h3-projektv2-24q2h3-gruppe3-txp.onrender.com";

// Cookie [OBS: ALREADY SET COOKIE]
String cookie = "";

char jwtSecret[] = SECRET_JWT_SECRET;
CustomJWT jwt(jwtSecret, 256);

// Defining our colors.
#define WHITE 0xFFFF
#define RED 0xF800
#define GREY 0x18E3
#define BLACK 0x0000
#define GREEN 0x0400

// Creating location structure.
struct location {
  float X;
  float Y;
};

// Creating our variables.
bool top = false;
bool bot = false;
bool retry = false;
float x, y, z;
int p = 0;
int scoreCounter = 0;
int angle = 0;
int turnTime = 0;
bool dead = false;
int prevScore;
String body;
int snakeGameId = 1;
bool cookieFound = false;

// WiFiSSLClient to connect to our API on render.com.
WiFiSSLClient client;

// Connecting to WiFi and setting up main-menu-screen (Grey background, white text)
void setup() {
  Serial.begin(9600);
  carrier.noCase();
  carrier.begin();

  // Attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);

    carrier.display.fillScreen(BLACK);
    carrier.display.setTextColor(WHITE);
    carrier.display.setTextSize(2);
    carrier.display.setCursor(45, 80);
    carrier.display.print("Connecting to");
    carrier.display.setCursor(75, 110);
    carrier.display.print("WIFI...");

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // Wait 10 seconds for connection:
    delay(10000);
  }
  carrier.display.setTextColor(GREEN);
  carrier.display.setTextSize(1);
  carrier.display.setCursor(55, 140);
  carrier.display.print("Connected to:");
  carrier.display.print(ssid);
  carrier.display.print("!");

  String res = "";
  if (client.connect(server, 443)) {
    while (cookieFound == false) {
      Serial.println("Attempting to get token...");
      carrier.display.fillScreen(BLACK);
      carrier.display.setTextColor(WHITE);
      carrier.display.setTextSize(2);
      carrier.display.setCursor(45, 80);
      carrier.display.print("Checking for");
      carrier.display.setCursor(75, 110);
      carrier.display.print("user...");

      getDevice();
      String extractResponse = client.readString();
      String extractedJWT = extractJWT(extractResponse);
      Serial.println(extractedJWT);

      if (extractedJWT == "") {
        Serial.println("NULL");
      } else {
        cookieFound = true;
        cookie = extractedJWT;
        break;
      }
      delay(10000);
    }
  }
  carrier.display.setTextColor(GREEN);
  carrier.display.setTextSize(1);
  carrier.display.setCursor(55, 140);
  carrier.display.print("Connected to user!");

  delay(5000);

  carrier.display.fillScreen(GREY);
  carrier.display.setTextColor(WHITE);
}

// The Arduino's screen is set to 240x240 pixels which creates a squared screen.
// When in reality the screen is circular. Therefore, we need to create a circular "map".
bool isInsideCircle(int x, int y, int centerX, int centerY, int radius) {
  int dx = x - centerX;
  int dy = y - centerY;
  return (dx * dx + dy * dy <= radius * radius);
}

void loop() {

  location snake = { 120, 120 };  // Start coordinates for our snake (120, 120).
  location tail[200];             // Max length of the snake.
  location berry = { 100, 100 };  // Start coordinates for our berry (100, 100).
  dead = false;                   // Sets snake-status to alive.

  // Creates main-menu-screen.
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

  // Tells our Arduino to get ready for inputs from our buttons.
  carrier.Buttons.update();

  // If button-4 is pressed -> create snake-start-menu-screen.
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
      // If button-4 is pressed AGAIN -> go back to main-menu-screen.
      if (carrier.Buttons.onTouchDown(TOUCH4)) {
        break;
      }
      // If button-2 is pressed -> create snake-gameplay-screen.
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

        // Countdown.
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

          char string[cookie.length() + 1];
          cookie.toCharArray(string, cookie.length() + 1);  // Convert String to char array
          //Serial.println(string);  // Print the string

          //char string[] = String(cookie);

          // Coordinates for our "fake" screen/map.
          int centerX = 120;
          int centerY = 120;
          int radius = 120;

          // If the snake ISN'T inside of our "fake" screen/map -> dead.
          if (!isInsideCircle(snake.X, snake.Y, centerX, centerY, radius)) {
            dead = true;
          }

          // Draws snake.
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

          // Draws berry.
          carrier.display.drawPixel(berry.X - 1, berry.Y - 1, RED);
          carrier.display.drawPixel(berry.X - 1, berry.Y, RED);
          carrier.display.drawPixel(berry.X - 1, berry.Y + 1, RED);
          carrier.display.drawPixel(berry.X, berry.Y - 1, RED);
          carrier.display.drawPixel(berry.X, berry.Y, RED);
          carrier.display.drawPixel(berry.X, berry.Y + 1, RED);
          carrier.display.drawPixel(berry.X + 1, berry.Y - 1, RED);
          carrier.display.drawPixel(berry.X + 1, berry.Y, RED);
          carrier.display.drawPixel(berry.X + 1, berry.Y + 1, RED);

          // If the snake hits a berry -> add point, remove the berry and spawn a new berry.
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
            soundEffect();
            Serial.println(scoreCounter);
          }

          // If the snake is at the top of the screen -> show score at the bottom.
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

          // If the snake is at the bottom of the screen -> show score at the top.
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

          // If the accelerator is available -> use the gyroscope.
          if (carrier.IMUmodule.accelerationAvailable()) {
            carrier.IMUmodule.readAcceleration(x, y, z);

            // Movement code. (Includes: Interval of gyroscope-X value, turntime and angle (stages of snake))
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

            // If dead is true -> create defeat-screen and reset snake (Coordinates + tail).
            if (dead == true) {
              // Parse the JSON response
              StaticJsonDocument<200> doc;

              // DECODE JWT
              jwt.allocateJWTMemory();

              jwt.decodeJWT(string);
              String res = jwt.payload;

              jwt.clear();
              // Test if parsing succeeds
              DeserializationError error = deserializeJson(doc, res);
              if (error) {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
                return;
              }
              int id = doc["id"];
              int userId = id;

              String personalHighscoreRes = "";

              if (client.connect(server, 443)) {
                saveScore(scoreCounter, snakeGameId);
                delay(2500);
                personalHighscore(userId, snakeGameId);
                personalHighscoreRes = readResponse();
                personalHighscoreRes = extractValue(personalHighscoreRes);
              }

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
              carrier.display.print(personalHighscoreRes);

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
        // Defeat-screen-menu.
        while (dead) {
          carrier.Buttons.update();
          // If button-1 is pressed -> send user back to snake-gameplay-screen.
          if (carrier.Buttons.onTouchDown(TOUCH1)) {
            carrier.display.fillScreen(GREY);
            retry = true;
            dead = false;
            break;
          }

          // If button-3 is pressed -> send user back to main-menu-screen.
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

  // If button-2 is pressed -> create brick-breaker-start-menu-screen.
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
      // If button-4 is pressed -> go back to main-menu-screen.
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

  // If button-0 is pressed -> create pong-start-menu-screen.
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
      // If button-4 is pressed -> go back to main-menu-screen.
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

// POST score endpoint:
String saveScore(int score, int gameId) {
  String body =
    "{\n  \"score\": " + String(score) + ", \n  \"gameId\": " + String(gameId) + " \n}";
  Serial.println(body);
  Serial.println("Trying to send POST request to /saveScore...");
  client.println("POST /saveScore HTTP/1.1");
  client.println("Host: h3-projektv2-24q2h3-gruppe3-txp.onrender.com");
  client.println("Content-Type: application/json");
  client.println("Cookie: JWT=" + cookie);
  client.println("Content-Length: " + String(body.length()));
  client.println();  // end HTTP request header
  client.println(body);
  client.println();
  Serial.println("POST request sent!");
}

// POST personalHighscore endpoint:
String personalHighscore(int userId, int gameId) {
  if (client.connect(server, 443)) {
    String body = "{\n  \"userId\": " + String(userId) + ", \n  \"gameId\": " + String(gameId) + " \n}";
    Serial.println("Trying to send POST request to /personalHighscore...");
    client.println("POST /personalHighscore HTTP/1.1");
    client.println("Host: h3-projektv2-24q2h3-gruppe3-txp.onrender.com");
    client.println("Content-Type: application/json");
    client.println("Cookie: JWT=" + cookie);
    client.println("Content-Length: " + String(body.length()));
    client.println();  // end HTTP request header
    client.println(body);
    client.println();
    Serial.println("POST request sent!");
  }
}

// POST sendArduinoName endpoint:
String sendArduinoName(String name, String token) {
  if (client.connect(server, 443)) {
    String body = "{\n  \"arduinoDevice\": \"" + name + "\", \n  \"Account\": \"" + token + "\" \n}";
    Serial.println(body);
    Serial.println("Trying to send POST request to /sendArduinoName...");
    client.println("POST /sendArduinoName HTTP/1.1");
    client.println("Host: h3-projektv2-24q2h3-gruppe3-txp.onrender.com");
    client.println("Content-Type: application/json");
    client.println("Content-Length: " + String(body.length()));
    client.println();  // end HTTP request header
    client.println(body);
    client.println();
    Serial.println("POST request sent!");
  }
}

void getDevice() {
  if (client.connect(server, 443)) {
    Serial.println("Trying to send GET request to /getDevice/:arduinoDevice...");
    client.println("GET /getDevice/Arduino2 HTTP/1.1");
    client.println("Host: h3-projektv2-24q2h3-gruppe3-txp.onrender.com");
    client.println();
    Serial.println("GET request sent!");
  }
}

String readResponse() {
  bool headersEnded = false;
  String responseBody = "";

  while (client.connected()) {
    if (client.available()) {
      String line = client.readStringUntil('\n');
      //Serial.print(line);
      if (line == "\r") {
        headersEnded = true;
      }
      if (headersEnded) {
        responseBody += line + "\n";
      }
    }
  }
  return responseBody;
}

String extractValue(String response) {
  int secondIndex = response.indexOf('\n', response.indexOf('\n') + 1);  // Find the index of the second newline character

  // Extract the substring following the second newline character
  String valueStr = response.substring(secondIndex + 1);
  valueStr.trim();
  valueStr.remove(valueStr.length() - 1);
  return valueStr;
}

String extractJWT(String response) {
  int tokenStartIndex = response.indexOf("eyJ");                // Find the start of the token
  int tokenEndIndex = response.indexOf("\"", tokenStartIndex);  // Find the end of the token
  if (tokenStartIndex != -1 && tokenEndIndex != -1) {
    return response.substring(tokenStartIndex, tokenEndIndex);
  } else {
    return "";
  }
}

void soundEffect() {
  carrier.Buzzer.beep(294, 125);  //D4
}
