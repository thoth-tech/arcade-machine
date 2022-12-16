#include <string>

using namespace std::string;

class AbstractMenu
{
public:
public:
    Menu();
    Menu(std::vector<ConfigData> configs);
    ~Menu();

    // Getters
    auto getButtons() const -> const std::vector<Button *> { return this->m_btns; }
    bool getOverlayState() { return m_overlayActive; }

    std::vector<std::string> getGameSprites(std::vector<ConfigData> configs); // gets game images from the config files and returns vector

    void createGrid();    // Create a GridLayout object
    void createButtons(); // Create a list of games
    void createTip();     // create a tip to display to the user.
    void updateCarousel();
    void carouselHandler();
    void drawMenuPage();
    void updateSlide(sprite buttonSprite, int position); // Method to update the sprite positions and draw sprite.
    void drawUpdateSlideLeft();                          // Slide the game buttons on left key input.
    void drawUpdateSlideRight();                         // Slide the game buttons on right key input.
    void drawOverlay(ConfigData config, GameData stats); // Draw an overlay over the game, using data from the config.

#ifdef _WIN32
    bool focusWindow(std::string windowName, int timeout = 2000);
    void startGame(LPCSTR gamePath, LPSTR gameExe, LPCSTR gameDirectory);
#else
    void startGame(struct s_ExecutablePath path);
#endif
    void checkGameExit();

    void backToGamesMenu(); // Fade back to games menu
    void fade(double alphaStart, double alphaEnd, double alphaStep);
}
