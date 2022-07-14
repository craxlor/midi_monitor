/**
 * @file display.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-07-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "application/displayWrapper.h"

void DisplayWrapper::init(int displayCount, int heightPixelsPerDisplay, int widthPixelsPerDisplay)
{
    this->displayCount = displayCount;
    this->height = heightPixelsPerDisplay;
    this->width = widthPixelsPerDisplay;
}

void DisplayWrapper::setGCursor(int horizontalPixelOffset, int verticalPixelOffset)
{
    MIOS32_LCD_DeviceSet(horizontalPixelOffset / width);
    MIOS32_LCD_GCursorSet(horizontalPixelOffset % width, verticalPixelOffset);
}

void DisplayWrapper::setTextCursor(int column, int row)
{   

    MIOS32_LCD_CursorSet(column * mios32_lcd_column, row);
}
void DisplayWrapper::printBitmap(mios32_lcd_bitmap_t bitmap)
{
}
void DisplayWrapper::printByte(u8 byte)
{
}
void DisplayWrapper::clear()
{
}