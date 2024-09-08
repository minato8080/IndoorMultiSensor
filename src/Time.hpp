#include <Arduino.h>
#include <LiquidCrystal.h>
#include <TimeLib.h> // https://github.com/PaulStoffregen/Time

#include "lcd.hpp"

// 曜日の計算（ツェラー(Zeller)の公式)
int getDayWeek(int year, int month, int day) {
    int w;
    if (month < 3) {
        year = year - 1;
        month = month + 12;
    }
    w = (year + (year / 4) - (year / 100) + (year / 400) +
         (13 * month + 8) / 5 + day) %
        7;
    return w;
}

// コンパイルした時間を返す
time_t compileTime() {
    // fudge factor to allow for upload time, etc. (seconds, YMMV)
    const time_t FUDGE(10);
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

// 秋月I2C液晶ディスプレイに表示
void serialTime(LiquidCrystal lcd, tmElements_t tm) {
    char buffer[50];
    //  年月日の表示
    lcd.setCursor(0, 0);
    lcd.print(tmYearToCalendar(tm.Year));
    lcd.setCursor(4, 0);
    lcd.print("/");
    lcd.setCursor(5, 0);
    lcd.print(padInt(tm.Month, 2, '0', buffer));
    lcd.setCursor(7, 0);
    lcd.print("/");
    lcd.setCursor(8, 0);
    lcd.print(padInt(tm.Day, 2, '0', buffer));

    // 曜日の表示 0=日曜  1=月曜  2=火曜  3=水曜  4=木曜  5=金曜  6=土曜
    int day_week;
    char DayWeekData[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    day_week = getDayWeek(tmYearToCalendar(tm.Year), tm.Month, tm.Day);
    lcd.setCursor(11, 0);
    lcd.print("(");
    lcd.print(DayWeekData[day_week]);
    lcd.print(")");

    // 時分秒の表示
    lcd.setCursor(0, 1);
    lcd.print(padInt(tm.Hour, 2, '0', buffer));
    lcd.setCursor(2, 1);
    lcd.print(":");
    lcd.setCursor(3, 1);
    lcd.print(padInt(tm.Minute, 2, '0', buffer));
    lcd.setCursor(5, 1);
    lcd.print(":");
    lcd.setCursor(6, 1);
    lcd.print(padInt(tm.Second, 2, '0', buffer));
}
