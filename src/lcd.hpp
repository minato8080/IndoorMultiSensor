#ifndef LCD_HPP
#define LCD_HPP

#include <stdio.h>

/** 先頭の文字埋めを行う
 * \param[in] number 元の数字
 * \param[in] width 桁数
 * \param[in] padChar 先頭埋めする文字
 * \param[in] output 出力先
 * \return 出力先
 */
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

/** 先頭の文字埋めを行う
 * \param[in] number 元の数字
 * \param[in] width 桁数
 * \param[in] precision 小数点以下桁数
 * \param[in] padChar 先頭埋めする文字
 * \param[in] output 出力先
 * \return 出力先
 */
char *padFloat(float number, int width, int precision, char padChar,
               char *output) {
    // バッファ
    char format[30]; // フォーマット文字列の最大長さを考慮
    if (padChar == ' ') {
        // 空白文字の場合、長さと精度のみ指定
        sprintf(format, "%%%d.%df", width, precision);
    } else {
        // 文字指定がある場合
        sprintf(format, "%%%c%d.%df", padChar, width, precision);
    }
    // フォーマットを適用して出力
    sprintf(output, format, number);
    return output;
}

#endif
