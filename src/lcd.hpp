#include <stdio.h>

/** 先頭の文字埋めを行う
 * \param[in] number 元の数字
 * \param[in] width 桁数
 * \param[in] padChar 先頭埋めする文字
 * \param[in] output 出力先
 * \return 変換後の文字列
 */
char *padInt(int number, int width, char padChar, char *output);