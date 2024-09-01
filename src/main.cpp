#include <Arduino.h>
#include <LiquidCrystal.h>
#include <RX8900RTC.h> // https://github.com/citriena/RX8900RTC
#include <TimeLib.h>   // https://github.com/PaulStoffregen/Time
#include <I2CScanner.h>
#include <Wire.h>      // Arduino IDE のI2Cライブラリ

void lcdzeroSup(int digit);
void serialTime(tmElements_t tm);
time_t compileTime();
int getDayWeek(int year, int month, int day);

// I2CScanner scanner;
RX8900RTC RTC;
LiquidCrystal lcd(1, 0, 12, 13, 14, 15);

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

// // 秋月I2C液晶ディスプレイに表示
// void serialTime(tmElements_t tm) {
//     //  年月日の表示
//     lcd.setCursor(0, 0);
//     lcd.print(tmYearToCalendar(tm.Year));
//     lcd.setCursor(4, 0);
//     lcd.print("/");
//     lcd.setCursor(5, 0);
//     lcdzeroSup(tm.Month);
//     lcd.setCursor(7, 0);
//     lcd.print("/");
//     lcd.setCursor(8, 0);
//     lcdzeroSup(tm.Day);
//     // 曜日の表示 0=日曜  1=月曜  2=火曜  3=水曜  4=木曜  5=金曜  6=土曜
//     int day_week;
//     char DayWeekData[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
//     day_week = getDayWeek(tmYearToCalendar(tm.Year), tm.Month, tm.Day);
//     lcd.setCursor(11, 0);
//     lcd.print("(");
//     lcd.print(DayWeekData[day_week]);
//     lcd.print(")");
//     // 時分秒の表示
//     lcd.setCursor(0, 1);
//     lcdzeroSup(tm.Hour);
//     lcd.setCursor(2, 1);
//     lcd.print(":");
//     lcd.setCursor(3, 1);
//     lcdzeroSup(tm.Minute);
//     lcd.setCursor(5, 1);
//     lcd.print(":");
//     lcd.setCursor(6, 1);
//     lcdzeroSup(tm.Second);
// }

// // 曜日の計算（ツェラー(Zeller)の公式)
// int getDayWeek(int year, int month, int day) {
//     int w;
//     if (month < 3) {
//         year = year - 1;
//         month = month + 12;
//     }
//     w = (year + (year / 4) - (year / 100) + (year / 400) +
//          (13 * month + 8) / 5 + day) %
//         7;
//     return w;
// }

// // 月、日、時、分、秒が0～9の場合、1桁目を 空白 もしくは 0 に置換
// void lcdzeroSup(int digit) {
//     if (digit < 10)
//         lcd.print(' '); // 現在「空白」
//     lcd.print(digit);
// }

// function to return the compile date and time as a time_t value
// from alarm_ex1.ino in Arduino DS3232RTC Library sample sketch by Jack
// Christensen.
time_t compileTime() {
    const time_t FUDGE(
        10); // fudge factor to allow for upload time, etc. (seconds, YMMV)
    const char *compDate = __DATE__, *compTime = __TIME__,
               *months = "JanFebMarAprMayJunJulAugSepOctNovDec";
    char compMon[4], *m;
    strncpy(compMon, compDate, 3);
    compMon[3] = '\0';
    m = strstr(months, compMon);
    tmElements_t tm;
    tm.Month = ((m - months) / 3 + 1);
    tm.Day = atoi(compDate + 4);
    tm.Year = atoi(compDate + 7) - 1970;
    tm.Hour = atoi(compTime);
    tm.Minute = atoi(compTime + 3);
    tm.Second = atoi(compTime + 6);
    time_t t = makeTime(tm);
    return t + FUDGE; // add fudge factor to allow for compile time
}