#ifndef PotMNG
#define PotMNG
#include <Arduino.h>

void potSetup();

float incrementVoltage();
float decrementVoltage();
float manageVoltage(bool incr);
void changeVStep();

float incrementCurrent();
float decrementCurrent();
float manageCurrent(bool incr);
void changeCStep();

#endif
