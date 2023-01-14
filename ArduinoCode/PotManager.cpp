#include "PotManager.h"
#include <SPI.h>
#include <EEPROM.h>
#define CS 10
#include "myDisplay.h"

char v_counter = 0;

float v_vals[23] = {1.23, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 
                    5.5, 6.0, 6.5, 7.0, 7.5, 8.0, 8.5, 9.0,
                    9.5, 10.0, 10.5, 11.0, 11.5, 11.83}; 
float c_val = 0;

char vStep = 1;
float cStep = 0.05;

void changeVStep() {
  //Change vStep from 1 to 2 & and vice versa
  if (vStep == 1) {
    vStep = 2;
    displayVStep(1.0);
  } else if (vStep == 2) {
    vStep = 4;
    displayVStep(2.0);
  } else if (vStep == 4) {
    vStep = 1;
    displayVStep(0.5);
   }
}

float incrementVoltage() {
  return manageVoltage(1);
}

float decrementVoltage() {
  return manageVoltage(0);
}

float manageVoltage(bool incr) {
  if (incr) {
    if (v_counter < 22 ) {
      v_counter += vStep;
    }
  } else {
    if (v_counter > 0) {
      v_counter -= vStep;
    }
  }
  //In the case of an overflow due to vStep being greater than 1.
  if (v_counter > 22) {v_counter = 22;}
  else if (v_counter < 0) {v_counter = 0;}

  //Save the counter value to EEPROM
  Serial.println("Writing voltage to EEPROM");
  EEPROM.put(0, v_counter);
  
  //Find & set the pot config corresponding to the counter value 
  //and return the desired/expected voltage from this config

  //Get the config for the output voltage "v_vals[v_counter]"
  float v_configf = (255/(48*0.9969))*(53.4-(1.23*53.4/v_vals[v_counter]));
  //Since this config is a decimal, not an int, we need to find 
  //an integer config that will produce an output voltage closest to
  //v_vals[v_counter]. I found that using the simple round function 
  //does this.
  int v_config = round(v_configf);
  if (v_config == 256) { v_config--; }

  delayMicroseconds(5);
  SPI.transfer16(0x0000); //It seems like the data sent to the potentiometer
                          //gets corrupted if just before, the display was
                          //communicated with. So I'm sending a "dummy" set
                          //of data to get "corrupted" instead of the pot data.
                          //This is a quick, hacky solution which I won't adjust
                          //since I have a big time constraint
  delayMicroseconds(5);


  digitalWrite(CS, 0);
  delayMicroseconds(5);

  SPI.transfer16(v_config);

  delayMicroseconds(5);
  digitalWrite(CS, 1);
  delayMicroseconds(5);

  
  //Display the voltage associated with the binary config.
  //displaySVoltage(1.23 * (53.4)/(5.4 + (48.0 - v_potConfig*(48.0/255.0)*0.9969)));
  displaySVoltage(v_vals[v_counter]);

  //As you can see above, I multiply the binary configuration value by 0.9969. This is because
  //I noticed that as the top pot terminal to wiper resistance increased, the error
  //of this resistance grew. IE. at the max configuration value, 255, the resistance was expected to
  //be 48k, however, it was 47.8k.
  //This error almost seems like any binary configuration gives a resistance that is supposed to 
  //be given by a lower configuration.
  //So what I've done in the function which calculates the expected voltage output is, I
  //multiply the binary configuration value by a number <1 so that the voltage is calculated for a 
  //slightly lower configuration. And that way I accounted for the error.
    
}

void changeCStep() {
  if (cStep == 0.05) {
    cStep = 0.1;
    displayCStep(0.1);
  } else if (cStep == 0.1) {
    cStep = 0.5;
    displayCStep(0.5);
  } else if (cStep == 0.5) {
    cStep = 0.05;
    displayCStep(0.05);
  }
}

float incrementCurrent() {
  return manageCurrent(1);
}

float decrementCurrent() {
  return manageCurrent(0);
}

float manageCurrent(bool incr) {
  if (incr) {
    if (c_val < 3.0) {
      c_val += cStep;
    }
  } else {
    if (c_val > 0.00) {
      c_val -= cStep;
    }
  }
  if (c_val > 3.0) { c_val = 3.0; }
  else if (c_val < 0) { c_val = 0; };

  //Save the current value to EEPROM
  Serial.println("Writing current to EEPROM");
  EEPROM.put(1, c_val);
  
  //Right now, the maximum current is 3.3A. I want it to be 3.0A.
  //At config 255 I get 3.3A, and at 231 I get ~3.0A.

  //Get the config associated with the current "c_val"
  int c_config = round(c_val * 231/3.0);
  
  delayMicroseconds(5);
  SPI.transfer16(0x0000);
  delayMicroseconds(5); 


  digitalWrite(CS, 0);
  delayMicroseconds(5);

  SPI.transfer16(255-c_config + 4096);   //The higher the config setting, the lower the current limit
                                         //Hence why I subtract c_config from 255

  delayMicroseconds(5);
  digitalWrite(CS, 1);
  delayMicroseconds(5);

  displaySCurrent(c_val);
}

void potSetup(){
  delayMicroseconds(5);
  SPI.transfer16(0x0000); 
  delayMicroseconds(5);

  //Read values stored on EEPROM
  EEPROM.get(0, v_counter);
  EEPROM.get(1, c_val);
  //By default, every single binary digit at these addresses is '1'
  //In that case, I don't want to cause an out of range error or 
  //use an undefined float value:
  if (c_val > 3.0 && c_val < 0) {
    c_val = 0;
  }
  if (v_counter > 22) {
    v_counter = 0;
  }
  
  digitalWrite(CS, 1);
  delay(1);
  digitalWrite(CS, 0);
  delay(1);
  //Set lastly used voltage pot wiper config
  float v_configf = (255/(48*0.9969))*(53.4-(1.23*53.4/v_vals[v_counter]));
  int v_config = round(v_configf);
  if (v_config == 256) { v_config--; }
  SPI.transfer16(v_config);
  digitalWrite(CS, 1);
  delay(1);
  digitalWrite(CS, 0);
  delay(1);
  //Set lastly used current pot wiper config
  int c_config = round(c_val * 231/3.0);
  SPI.transfer16(255-c_config + 4096);
  delay(1);
  digitalWrite(CS, 1);
  delay(1);

  displaySVoltage(v_vals[v_counter]);
  displaySCurrent(c_val);

  displayVStep(0.5);
  displayCStep(0.05);
}
