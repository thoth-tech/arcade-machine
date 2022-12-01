#include "splashkit.h"
#include "GridLayout.h"
#include "Button.h"

class AudioSetting
{
private: 
    int _selector = 1;
    float m_volume = 40;
    bool m_isOptionOpen = true;

    GridLayout m_grid;
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