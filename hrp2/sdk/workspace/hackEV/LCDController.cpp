/**
 * @file    LCDController.cpp
 * @brief   This file has LCDController.
 *          
 */

#include "LCDController.h"

//! 現在の行の高さ
unsigned int    currentLineHeight = 0;

//! 描画済みエリアの高さ
unsigned int    currentAreaHeight = 0;

//! 1行辺りの高さ : フォントMEDIUM
const unsigned char fontHeightMedium = 16;

//! 1行辺りの高さ : フォントSMALL
const unsigned char fontHeightSmall = 8;

//! 現在のフォントサイズ
lcdfont_t   fontSize = EV3_FONT_MEDIUM;

//! 現在のLCDの色
lcdcolor_t  lcdColor = EV3_LCD_WHITE;

/**
 * @brief   フォントサイズを設定する
 * @param   [in]    fontSize_ フォントサイズ
 * @return  ev3_lcd_set_font()参照
*/
ER setFontSize(lcdfont_t fontSize_)
{
    ER  result = ev3_lcd_set_font(fontSize_);
    if (result == E_OK) {
        fontSize = fontSize_;
    }

    return  result;
}

/**
 * @brief   LCDの色を設定する
 * @param   [in]    lcdColor_ LCDの色
 * @return  なし
*/
void setLCDColor(lcdcolor_t lcdColor_)
{
    lcdColor = lcdColor_;
}

/**
 * @brief   LCDの色を取得する
 * @return  lcdColor
*/
lcdcolor_t getLCDColor()
{
    return  lcdColor;
}

/**
 * @brief   1行の高さを取得する
 * @return  1行の高さ
*/
unsigned char getLineheight()
{
    if (fontSize == EV3_FONT_SMALL) {
        return  fontHeightSmall;
    }

    return  fontHeightMedium;
}

/**
 * @brief   LCDに文字を書く
 * @param   [in]    message 出力する文字列
 * @return  ev3_lcd_draw_string()参照
*/
ER writeStringLCD(const char* message)
{
    unsigned int lineHeight = getLineheight();
    if (currentAreaHeight + lineHeight > EV3_LCD_HEIGHT) {
        ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, lineHeight, getLCDColor());
        currentAreaHeight = 0;
    }

    ER  result = ev3_lcd_draw_string(message, 0, currentAreaHeight);
    if (result == E_OK) {
        currentAreaHeight += lineHeight;
    }

    return  result;
}

/**
 * @brief   LCDの表示を消す
 * @return  ev3_lcd_fill_rect()参照
*/
ER clearLCD()
{
    return  ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, getLCDColor());
}
