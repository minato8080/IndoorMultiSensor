#include <Arduino.h>
#include <LiquidCrystal.h>
#include <RX8900RTC.h> // https://github.com/citriena/RX8900RTC
#include <Wire.h>      // Arduino IDE のI2Cライブラリ

#include <Time.hpp>

RX8900RTC RTC;
LiquidCrystal lcd(1, 0, 12, 13, 14, 15);
// LiquidCrystal lcd(1, 0, 8, 9, 10, 11, 12, 13, 14, 15);

void setup() {
    Wire.begin(); // picoの場合setup内で再度beginする
    lcd.begin(16, 2); // ディスプレイの行数(16)と桁数(2)
    RTC.init();       // RTC初期化
    // RTC.get()よりcompileTime()が新しい時はcompiletimeをRTC.set
    time_t time_now, compiletime;
    time_now = RTC.get();
    compiletime = compileTime();
    if (time_now < compiletime) {
        RTC.set(compiletime); // set compiled time to RTC
        // tmElements_t tm = {0, 0, 12, 2, 5, 5, CalendarYrToTm(2020)}; //
        // second, minute, hour, week, day, month, year RTC.write(tm);
    }
    Serial.begin(9600);
}

void loop() {
    rx8900tmElements_t read = RTC.read(true);
    Serial.print(read.err1);
    Serial.print(read.err2);
    Serial.println();
    Serial.print(read.Month);
    Serial.print(read.Year);
    Serial.print(read.Month);
    Serial.print(read.Day);
    Serial.print(read.Hour);
    Serial.print(read.Minute);
    Serial.print(read.Second);
    Serial.println();
    serialTime(lcd, RTC.read());
    delay(1000);
}
