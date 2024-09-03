#include <Arduino.h>
#include <LiquidCrystal.h>
#include <RX8900RTC.h>
#include <Wire.h>

#include <Time.hpp>

RX8900RTC RTC;
LiquidCrystal lcd(1, 0, 12, 13, 14, 15);

void setup() {
    // picoの場合setup内で再度beginする必要がある
    Wire.begin();

    // ディスプレイの行数(16)と桁数(2)
    lcd.begin(16, 2);

    // RTC初期化
    RTC.init();

    // RTC.get()よりcompileTime()が新しい時はcompiletimeをRTC.set
    time_t time_now = RTC.get();
    time_t compiletime = compileTime();
    if (time_now < compiletime) {
        RTC.set(compiletime);
    }

    // テスト用
    // Serial.begin(9600);
}

void loop() {
    serialTime(lcd, RTC.read());
    delay(1000);
}
