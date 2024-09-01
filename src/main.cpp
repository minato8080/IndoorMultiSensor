#include <Arduino.h>
#include <LiquidCrystal.h>
#include <RX8900RTC.h> // https://github.com/citriena/RX8900RTC
#include <I2CScanner.h>
#include <Wire.h>      // Arduino IDE のI2Cライブラリ

#include <Time.cpp>
#include <lcd.cpp>

// I2CScanner scanner;
RX8900RTC RTC;

void setup() {
    // Wire.begin();
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
	// while (!Serial) {};
	// scanner.Init();
}

void loop() {
	// scanner.Scan();
    tmElements_t read = RTC.read();
    Serial.print(read.Year);
    Serial.print(read.Month);
    Serial.print(read.Day);
    Serial.print(read.Hour);
    Serial.print(read.Minute);
    Serial.println(read.Second);
    // serialTime(RTC.read());
    delay(1000);
}
