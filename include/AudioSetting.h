#ifndef ARCADE_MACHINE_AUDIO_SETTING_H
#define ARCADE_MACHINE_AUDIO_SETTING_H

#include "splashkit.h"
#include "GridLayout.h"
#include "Button.h"
#include "Selector.h"
class AudioSetting
{
private: 
    int _selector = 1;
    float m_volume = 40;
    bool m_isOptionOpen = true;

    GridLayout m_grid;
    std::vector<Button*> m_optionsBtns;
    // ButtonNode *m_optionsButtonNode = nullptr;
    // Selector m_selectorOptionsMenu;
    // point_2d m_mouse;
    // std::string m_action; 
    // AboutScreen m_aboutScreen;

public:
    AudioSetting();

    void changeVolume();
    void changeMucis();
    void drawHub(){
        draw_text("CHANGE SOUND", color_white(), 3131312 + 100, 200);
    };
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