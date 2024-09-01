#include <Arduino.h>
#include <TimeLib.h>   // https://github.com/PaulStoffregen/Time
#include <LiquidCrystal.h>

int getDayWeek(int year, int month, int day);
time_t compileTime();
void lcdzeroSup(LiquidCrystal lcd, int digit);
void serialTime(LiquidCrystal lcd, tmElements_t tm);