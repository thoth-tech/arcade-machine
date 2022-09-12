#ifndef ARCADE_MACHINE_OPTION_H
#define ARCADE_MACHINE_OPTION_H

#include "AboutScreen.h"
#include "splashkit.h"
#include <string>
#include "GridLayout.h"
#include "Button.h"
#include "ArcadeMachine.h"
#include "Selector.h"
#include "OptionsScreenButton.h"
#include "Audio.h"

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

    GridLayout m_grid;
    Audio m_audio;
    std::vector<Button*> m_optionsBtns;
    ButtonNode *m_optionsButtonNode = nullptr;
    Selector m_selectorOptionsMenu;
    point_2d m_mouse;
    std::string m_action; 
    AboutScreen m_aboutScreen;

public:
    Option();

    void createOptionsButtons();
    void drawOptionsMenu();
    bool checkAction();
    void soundMenu();


    float getVolume();
    int getCurrentMusic();
    void setCurrentMusic();
    void volumeControl();
    void changeDisplay();
    void playAboutScreen();
    bool isChangeMusic();
    bool isChangeVoLume();
    void changeSelector();
    void updateOption();
    void drawIntinialHub();
};

#endif