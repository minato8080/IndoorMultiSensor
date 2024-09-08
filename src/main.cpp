#include <Arduino.h>
#include <LiquidCrystal.h>
#include <RX8900RTC.h>
#include <Wire.h>

#include "../lib/mhz19c.hpp"
#include "lcd.hpp"
#include "time.hpp"

RX8900RTC RTC;
LiquidCrystal lcd(1, 0, 12, 13, 14, 15);
MHZ19C mhz19c(uart0, 16, 17);

void serialCO2(LiquidCrystal lcd, MHZ19C mhz19c);

void setup() {
    // picoの場合setup内で再度beginする必要がある
    Wire.begin();

    // ディスプレイの行数(16)と桁数(2)
    lcd.begin(16, 2);
    mhz19c.init();

    // RTC初期化
    RTC.init();

    // RTC.get()よりcompileTime()が新しい時はcompiletimeをRTC.set
    time_t time_now = RTC.get();
    time_t compiletime = compileTime();
    if (time_now < compiletime) {
        RTC.set(compiletime);
    }

    // テスト用
    Serial.begin(9600);
}

void loop() {
    serialTime(lcd, RTC.read());
    // serialCO2(lcd, mhz19c);
    delay(1000);
}

void serialCO2(LiquidCrystal lcd, MHZ19C mhz19c) {
    // read CO2
    int co2;
    mhz19c.measure(&co2);

    // CO2をフォーマットしてLCDに出力
    char buffer[50];
    lcd.setCursor(0, 0);
    lcd.print(padInt(co2, 4, ' ', buffer));

    lcd.setCursor(4, 0);
    lcd.print("ppm");
}