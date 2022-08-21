#ifndef ARCADE_MACHINE_OPTION_H
#define ARCADE_MACHINE_OPTION_H

#include "splashkit.h"
#include <string>

// Options class
class Option
{
private: 
    int m_displayStyle = 1;
    int _selector = 1;
    bool m_isSelected = false;
    int m_currentMusic = 1;
    int m_insideSeletor = 1;
    float m_volume = 40;
    bool m_isOptionOpen = true;

public:
    Option() {}

    float getVolume();
    int getCurrentMusic();

    void setCurrentMusic();
    
    void volumeControl();
    void changeDisplay();

    bool isChangeMusic();
    bool isChangeVoLume();

    void changeSelector();
    void updateOption();
    void drawIntinialHub();

};

#endif