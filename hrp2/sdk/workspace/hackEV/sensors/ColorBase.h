/**
 * @file    ColorBase.h
 * @brief   �J���[�Z���T�[���g���ĐF���擾����N���X�̊��N���X
 */
#pragma once

//! Class for ColorBase
class ColorBase
{
public:
    explicit ColorBase();
    virtual void initialize() = 0;
    virtual uint32_t getRGB(int colorCount = 2) = 0;
    virtual uint8_t getID(int colorCount = 2) = 0;
    virtual char* getName(int colorCount = 2) = 0;

protected:
    virtual uint32_t getRGB_BW();
    virtual uint8_t getID_BW();
    virtual char* getName_BW();
    virtual uint8 color2BW();

    sensor_port_t portColor;
};
