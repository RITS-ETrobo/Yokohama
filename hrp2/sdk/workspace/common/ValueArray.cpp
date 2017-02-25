/**
 * @file    ValueArray.cpp
 * @brief   This file has the class for storing values associated with key.
 */
#include <string.h>
#include "ValueArray.h"

/**
 * @brief   ValueArrayクラスのコンストラクター
 * @return  なし
*/
ValueArray::ValueArray()
{
}

/**
 * @brief   ValueArrayクラスの初期化
 * @return  なし
*/
void ValueArray::initialize()
{
    if (value) {
        value = NULL;
        delete [] value;
    }

    arraySize = 0;
}

/**
 * @brief   配列に数値を設定する
 * @param   key キー
 * @param   input   設定する数値
 * @return  なし
*/
void ValueArray::setNumberValue(uint_t key, long input)
{
    int index = findIndex(key);
    if (index == -1) {
        addArray();
        index = arraySize - 1;
    }

    value[index].key = key;
    value[index].valueNumber = input;
}

/**
 * @brief   配列に文字列を設定する
 * @param   key キー
 * @param   input   設定する文字列
 * @return  なし
*/
void ValueArray::setStringValue(uint_t key, char* input)
{
    int index = findIndex(key);
    if (index == -1) {
        addArray();
        index = arraySize - 1;
    }

    value[index].key = key;
    strcpy(value[index].valueString, input);
}

/**
 * @brief   配列から数値を取得する
 * @param   key キー
 * @return  キーに紐づいている数値
*/
long ValueArray::getNumberValue(uint_t key)
{
    return  value[findIndex(key)].valueNumber;
}

/**
 * @brief   配列から文字列を取得する
 * @param   key キー
 * @return  キーに紐づいている文字列
*/
char* ValueArray::getStringValue(uint_t key)
{
    return  value[findIndex(key)].valueString;
}

/**
 * @brief   配列を拡張する
 * @return  なし
*/
void ValueArray::addArray()
{
    arraySize++;
    value = new VALUE_ELEMENT[arraySize];
}

/**
 * @brief   配列で要素が一致するインデックスを探す
 * @param   key キー
 * @return  要素が一致するインデックス。見つからない場合は、-1
*/
int ValueArray::findIndex(uint_t key)
{
    if (arraySize == 0) {
        return  -1;
    }

    for (int index = 0; index < arraySize; index++) {
        if (value[index].key == key) {
            return  index;
        }
    }

    return  -1;
}
