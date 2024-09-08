#include <Arduino.h>
#include <LiquidCrystal.h>
#include <RX8900RTC.h>
#include <Wire.h>

#include "../lib/bme280.hpp"
#include "../lib/mhz19c.hpp"
#include "lcd.hpp"
#include "time.hpp"

RX8900RTC RTC;
LiquidCrystal lcd(1, 0, 6, 7, 8, 9);
MHZ19C mhz19c(uart0, 12, 13);
BME280 bme280;

// 温度は湿度・気圧の補正用であり気温よりやや高いため適当に補正する
// 補正値の適正値はセンサの使用条件により異なる
static const float TEMPERATURE_OFFSET = -1.5f;

void serialCO2(LiquidCrystal lcd, MHZ19C mhz19c);
void serialEnvironment(LiquidCrystal lcd, BME280 bme280);

void setup() {
    // picoの場合setup内で再度begin
    Wire.begin();
    Wire1.begin();

    // LCDディスプレイ初期化
    lcd.begin(16, 2);
    // CO2測定モジュール初期化
    mhz19c.init();
    // 温湿度気圧測定モジュール初期化
    bme280.init();
    // RTCモジュール初期化
    RTC.init();

    // RTCの時刻とコンパイル時刻の新しい方をセット
    time_t time_now = RTC.get();
    time_t compiletime = compileTime();
    if (time_now < compiletime)
        RTC.set(compiletime);

    // テスト用
    Serial.begin(9600);
}

void loop() {
    // serialTime(lcd, RTC.read());
    serialCO2(lcd, mhz19c);
    serialEnvironment(lcd, bme280);
    delay(1000);
}

/** CO2を測定し、LCDに出力
 * \param[in] lcd LiquidCrystalインスタンス
 * \param[in] mhz19c MHZ19Cインスタンス
 */
void serialCO2(LiquidCrystal lcd, MHZ19C mhz19c) {
    // 文字変換用のバッファ
    char buffer[50];

    // CO2数値を読み取る
    int co2;
    mhz19c.measure(&co2);

    // LCDに出力
    lcd.setCursor(9, 1);
    lcd.print(padInt(co2, 4, ' ', buffer));
    lcd.setCursor(13, 1);
    lcd.print("ppm");
}

/** 温湿度気圧を測定し、LCDに出力
 * \param[in] lcd LiquidCrystalインスタンス
 * \param[in] bme280 BME280インスタンス
 */
void serialEnvironment(LiquidCrystal lcd, BME280 bme280) {
    // 文字変換用のバッファ
    char buffer[50];
    // 温度、気圧、湿度を読み取る
    float temperature, pressure, humidity;
    bme280.read_env(&temperature, &humidity, &pressure);
    temperature += TEMPERATURE_OFFSET;

    // LCDに出力
    // 温度
    lcd.setCursor(0, 0);
    lcd.print(padFloat(temperature, 2, 1, ' ', buffer));
    lcd.setCursor(4, 0);
    lcd.print(char(223)); // '゜'記号
    lcd.setCursor(5, 0);
    lcd.print('C');
    // 気圧
    lcd.setCursor(7, 0);
    lcd.print(padFloat(pressure, 4, 1, ' ', buffer));
    lcd.setCursor(13, 0);
    lcd.print("hPa");
    // 湿度
    lcd.setCursor(0, 1);
    lcd.print(padFloat(humidity, 2, 1, ' ', buffer));
    lcd.setCursor(4, 1);
    lcd.print('%');
}