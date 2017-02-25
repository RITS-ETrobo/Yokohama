/**
 * @file    ValueArray.h
 * @brief   This file has the class for storing values associated with key.
 */
#pragma once

#include "product.h"

//! 文字列を格納する為のバッファーのサイズ
#define BUFFER_SIZE_ELEMENT 32

/*! @struct VALUE_ELEMENT
    @brief  ValueArrayクラスが持つ配列に格納する要素
*/
typedef struct {
    //! 配列のキー(1Bytes)
    uint_t  key;

    //! 数値(4Bytes)
    long    valueNumber;

    //! ログに出力する文字列(8Bytes)
    char    valueString[BUFFER_SIZE_ELEMENT];
} VALUE_ELEMENT;

//! Class for array
class ValueArray {
public:
    explicit ValueArray();
    virtual void initialize();
    virtual void setNumberValue(uint_t key, long input);
    virtual void setStringValue(uint_t key, char* input);
    virtual long getNumberValue(uint_t key);
    virtual char* getStringValue(uint_t key);

private:
    //! 配列
    VALUE_ELEMENT *value;

    //! 配列のサイズ
    int arraySize;

    void addArray();
    int findIndex(uint_t key);
};
