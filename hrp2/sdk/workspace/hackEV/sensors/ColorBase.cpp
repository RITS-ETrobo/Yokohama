/**
 * @file    ColorBase.h
 * @brief   �J���[�Z���T�[���g���ĐF���擾����N���X�̊��N���X
 */
#include "ColorBase.h"

/**
    @brief  �R���X�g���N�^
*/
ColorBase::ColorBase(sensor_port_t portColor_)
    : portColor(portColor_)
{
}

/**
    @brief  �F�̐��̒l���擾����(����)
    @return �F�̐��̒l(����)
*/
uint32_t ColorBase::getRGB_BW()
{
}

/**
    @brief  �F��ID���擾����(��/�D/��)
    @return �F��ID(��/�D/��)
*/
uint8_t ColorBase::getID_BW()
{
}

/**
    @brief  �F�̖��O���擾����(��/�D/��)
    @return �F�̖��O(��/�D/��)
*/
char* ColorBase::getName_BW()
{
}

/**
    @brief  �J���[�𔒍��ł̒l�ɕύX����
@return �F�̐��̒l(����)
*/
uint8 ColorBase::color2BW(uint32_t color)
{
}
