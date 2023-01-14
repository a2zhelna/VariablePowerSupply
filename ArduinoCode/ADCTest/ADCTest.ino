#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads; //This is the I2C address of the ADC.
                            //I chose it by connecting the ADC's ADDR pin to GND

short adc0, adc1, adc2;
float step = 0.0001875; //Using the lowest gain where each step is 0.1875mV

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  ads.begin(0x48); //Initialize the ADC

  Serial.println("Setup complete");
}

void loop() {
  // put your main code here, to run repeatedly:
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);

  Serial.println(adc0 * step);
  Serial.println(adc1 * step);
  Serial.println("-----=======------");
}
