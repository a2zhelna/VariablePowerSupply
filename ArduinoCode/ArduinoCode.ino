#include <SPI.h>

#include "PotManager.h"
#include "myDisplay.h"
#define CS 10

#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads; //This is the I2C address of the ADC.
                            //I chose it by connecting the ADC's ADDR pin to GND

short adc0, adc1, adc2;
float v_step = 0.0001875; //Using the lowest gain where each step is 0.1875mV

#define c_rotA 9
#define c_rotB 6
#define c_potBTN 5
#define v_rotA 4
#define v_rotB 3
#define v_potBTN 2

bool c_statePotBTN = 0;
bool c_prevStatePotBTN = 0;
bool v_statePotBTN = 0;
bool v_prevStatePotBTN = 0;

int c_rotCount = 0;
int c_stateA = 0;
int c_prevStateA = 0;
int v_rotCount = 0;
int v_stateA = 0;
int v_prevStateA = 0;

//SPI pins:
// MOSI   11
// MISO   12
// SCK    13

int readData = 0;

//TEST GARBOONIES
int rDisplayCounter = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //Setup adc
  ads.begin(0x48); //Initialize the ADC

  pinMode(c_rotA, INPUT);
  pinMode(c_rotB, INPUT);
  c_prevStateA = digitalRead(c_rotA);

  pinMode(CS, OUTPUT);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE2);

  //Setup display
  displaySetup();

  //Setup the potentiometer
  potSetup(); 
}

void loop() {
  c_stateA = digitalRead(c_rotA);
  if ((c_stateA != c_prevStateA) && c_stateA == 1) {
    if (c_stateA != digitalRead(c_rotB)) {
      //Once pin A's state changes, if it is equal to the state of pin B,
      //then pin A is lagging behind, and thus the rotation is CCW
      decrementCurrent();
    }
    else {
      incrementCurrent();
    }
  }

  c_prevStateA = c_stateA;

  c_statePotBTN = digitalRead(c_potBTN);
  if ((c_statePotBTN != c_prevStatePotBTN) & c_statePotBTN == 1) {
    //Serial.println("CURRENT BUTTON PRESSED!");
    changeCStep();
  }
  c_prevStatePotBTN = c_statePotBTN;


  v_stateA = digitalRead(v_rotA);
  if ((v_stateA != v_prevStateA) && v_stateA == 1) {
    if (v_stateA == digitalRead(v_rotB)) {
      //Once pin A's state changes, if it is equal to the state of pin B,
      //then pin A is lagging behind, and thus the rotation is CCW
      decrementVoltage();
    } else {
      incrementVoltage();
    } 
  }
  
  v_prevStateA = v_stateA;

  v_statePotBTN = digitalRead(v_potBTN);
  if ((v_statePotBTN != v_prevStatePotBTN) & v_statePotBTN == 1) {
    //Serial.println("VOLTAGE BUTTON PRESSED!");
    changeVStep();
  }
  v_prevStatePotBTN = v_statePotBTN;



  //Read the voltage and current and update the display with those read values
  if (rDisplayCounter >= 10000) {
    //Read the voltage
    adc0 = ads.readADC_SingleEnded(0);
    //Display the voltage
    displayAVoltage(adc0 * v_step * 3);
    
    //Read the current
    adc1 = ads.readADC_SingleEnded(1);
    //Display the current
    displayACurrent(adc1 * v_step);
    
    rDisplayCounter = 0;
  } else {
    rDisplayCounter++;
  }

  
}
