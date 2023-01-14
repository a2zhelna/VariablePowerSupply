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

char string[100]; 
char v_val[10];

Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST);    //8, 7 are the "RES"  & "DC" pins of
                                              //the display driver

void setup() {
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
}

void loop() {
  tft.fillRect(150, 10, 90, 20, DBLUE);
  tft.setCursor(170,10);
  tft.println(1.5);
}
