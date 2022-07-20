#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <mios32.h>

class Visualization
{
private:
    /**
        TEXT = 0,
        PIANO = 1,
        ACCORD = 2
    */
    int visualizationmode;

public:

    static u8 piano_bitmap[];
    void initKeyboard();

    void draw();
    void drawKeyboard(mios32_midi_package_t midi_package);

    bool isFlat(int note_value);

    void changeVisualizationMode();

    int getVisualizationMode();
    const char *getVisualizationModeAsString();
};

#endif