#include <stdio.h>

// 先頭の文字埋めを行う
char *padInt(int number, int width, char padChar, char *output) {
    // バッファ
    char format[20];
    if (padChar == ' ') {
        // 空白文字の場合、長さのみ指定
        sprintf(format, "%%%dd", width);
    } else {
        // 文字指定がある場合
        sprintf(format, "%%%c%dd", padChar, width);
    }
    // フォーマットを適用して出力
    sprintf(output, format, number);
    return output;
}