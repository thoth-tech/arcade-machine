#ifndef ARCADE_MACHINE_SPLASH_SCREEN_H
#define ARCADE_MACHINE_SPLASH_SCREEN_H

#include "splashkit.h"
#include <string>

class Splashscreen
{
private:
    std::string m_bmp;

public:
    // Default constructor
    Splashscreen()
    {
    }

    // Overloaded constructor
    Splashscreen(std::string bitmap)
    {
        this->m_bmp = bitmap;
    }

    void drawTitlePage();
};

#endif