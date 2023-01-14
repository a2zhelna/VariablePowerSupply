#include "myDisplay.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Arduino_ST7789_Fast.h>
#define TFT_DC 7
#define TFT_RST 8

#define BLACK    0x0000
#define BLUE     0x001F
#define BLUE     0x001F
#define DBLUE    0x000A
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST);

char buf[10];

void displaySetup() {
  tft.init(tft.width(), tft.height());
  tft.fillScreen(DBLUE);
  tft.setCursor(10,10);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("Set Voltage:");
  tft.setCursor(10,50);
  tft.println("Voltage:");
  tft.setCursor(10,90);
  tft.println("Set Current:");
  tft.setCursor(10,130);
  tft.println("Current:");
  tft.setCursor(10,190);
  tft.println("Voltage Step:");
  tft.setCursor(10,215);
  tft.println("Current Step:");
}

void displaySVoltage(float v) {
  dtostrf(v, 5, 2, buf);  //Convert float to char arr with of length 5, and 2 digits after decimal point
  tft.fillRect(150, 10, 90, 20, DBLUE);
  tft.setCursor(170, 10);
  tft.println(buf);
}
void displayAVoltage(float v) {
  dtostrf(v, 5, 2, buf);
  tft.fillRect(150, 50, 90, 20, DBLUE);
  tft.setCursor(170, 50);
  tft.println(buf);
}
void displaySCurrent(float c) {
  dtostrf(c, 5, 2, buf);
  tft.fillRect(150, 90, 90, 20, DBLUE);
  tft.setCursor(170, 90);
  tft.println(buf);
}
void displayACurrent(float c) {
  dtostrf(c, 5, 2, buf);
  tft.fillRect(150, 130, 90, 20, DBLUE);
  tft.setCursor(170, 130);
  tft.println(buf);
}
void displayVStep(float s) {
  dtostrf(s, 5, 2, buf);
  tft.fillRect(165, 190, 90, 20, DBLUE);
  tft.setCursor(170, 190);
  tft.println(buf);
}
void displayCStep(float s) {
  dtostrf(s, 5, 2, buf);
  tft.fillRect(165, 215, 90, 20, DBLUE);
  tft.setCursor(170, 215);
  tft.println(buf);
}
