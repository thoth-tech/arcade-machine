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
// #include "AudioSetting.h"

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
    // AudioSetting m_audio_setting;
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
    bool checkAction(Audio &audio);
    void soundMenu(Audio &audio);
    float getVolume();
    void setCurrentMusic(Audio &audio)
    {

        if(key_typed(LEFT_KEY))
        {
            if(audio.getCurrentMusic()>1)
            audio.setSongId(audio.getCurrentMusic()-1);
            else
            audio.setSongId(4);

        }
        if(key_typed(RIGHT_KEY)){
        if(audio.getCurrentMusic()<4)     
        audio.setSongId(audio.getCurrentMusic()+1);
        else
        audio.setSongId(1);}
    }
    void changeVolume(){
        if(key_typed(K_KEY)){
            if(music_volume()>0){
                set_music_volume(music_volume()-0.1);
            }
        }
        if(key_typed(L_KEY)){
            if(music_volume()<1){
                set_music_volume(music_volume()+0.1);
            }
        }
        
    }
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