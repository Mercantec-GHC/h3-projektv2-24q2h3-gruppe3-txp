#include <Arduino_MKRIoTCarrier.h>
MKRIoTCarrier carrier;

#define ST7735_GREY 0x18E3
#define ST7735_WHITE 0xFFFF
#define ST7735_GREEN 0x07E0
#define ST7735_RED 0xF800

void setup() {
  Serial.begin(9600);
  carrier.noCase();
  carrier.begin();
  carrier.display.fillScreen(ST7735_GREY);
  carrier.display.setTextColor(ST7735_WHITE);
}

int number = 0;

void loop() {
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

  carrier.Buttons.update();
  /*
  if (carrier.Buttons.onTouchDown(TOUCH0)) {
    Serial.println("PONG");
  }

  if (carrier.Buttons.onTouchDown(TOUCH2)) {
    Serial.println("BRICK BREAKER");
  }*/

  Serial.println(number);
  if (carrier.Buttons.onTouchDown(TOUCH4)) {
    number = 1;
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

    Serial.println(number);
    if (carrier.Buttons.onTouchDown(TOUCH1) && number == 1) {
      Serial.println("test");
    }
    delay(4000);
  }
  if (carrier.Buttons.onTouchDown(TOUCH2)) {
    Serial.println("test");
  }
}
