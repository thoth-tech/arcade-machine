#ifndef ARCADE_MACHINE_TIP_H
#define ARCADE_MACHINE_TIP_H

#include "splashkit.h"

#include <chrono>
#include <iostream>

// Font size
#define FONT_SIZE 20
// Width of the border
#define BORDER_WIDTH 3
// Space to leave between the content and container
#define CONTENT_BUFFER 10
// Space offset from the window-border
#define WBORDER_OFFSET 20

// Possible locations of the container
enum location {
    TOPLEFT,
    TOPRIGHT,
    TOPCENTER,
    BOTLEFT,
    BOTRIGHT,
    BOTCENTER
};

class Tip
{
private:
    bitmap m_image;
    // bitmap dimensions
    int m_bmpWidth;
    int m_bmpHeight;
    string m_text;
    // Length of the string, text
    int m_textLength;
    // Number of characters per line
    int m_charsPerLine = 0;
    // Number of lines
    int m_numLines;
    // Location within the window where the container lies
    location m_loc;
    // Where the container will be anchored within the screen.
    int m_xOffset;
    int m_yOffset;
    std::chrono::time_point<std::chrono::steady_clock> m_startTime;
    int m_duration;

    // Height of the container
    int m_containerHeight;
    // Width of the container
    int m_containerWidth;

    // Stores the animation
    animation m_anim = nullptr;
    // Drawing options required to load the animation
    drawing_options m_opt = option_defaults();

    int m_i = 30;

    void calculatePosition();

public:
    Tip()
    {
    }
    Tip(string text, bitmap image, int duration = 3000, int charsPerLine = 30, location loc = TOPCENTER);
    Tip(string text, bitmap image, animation anim, drawing_options opt, int duration = 3000, int charsPerLine = 30,
        location loc = TOPCENTER);

    ~Tip();

    void draw();
};

#endif