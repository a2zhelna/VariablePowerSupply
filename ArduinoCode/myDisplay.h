#ifndef myDispaly
#define myDisplay
#include <Arduino.h>

void displaySetup();
void displayVStep(float s);
void displayCStep(float s);
void displaySVoltage(float v);
void displayAVoltage(float v);
void displaySCurrent(float c);
void displayACurrent(float c);

#endif
