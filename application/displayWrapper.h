/**
 * @file display.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-07-14
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DISPLAYWRAPPER_H
#define DISPLAYWRAPPER_H

#include <mios32.h>
/**
 *only supports horizontal display orientation
 */
class DisplayWrapper
{
private:
    int width, height;
    int displayCount;

public:
    void init(int displayCount, int heightPixelsPerDisplay, int widthPixelsPerDisplay);
    void setGCursor(int verticalPixelOffset, int horizontalPixelOffset);
    void setTextCursor(int row, int column);
    void printBitmap(mios32_lcd_bitmap_t bitmap);
    void printByte(u8 byte);
    void clear();
};

#endif