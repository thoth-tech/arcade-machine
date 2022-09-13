#ifndef ARCADE_MACHINE_SELECTOR_H
#define ARCADE_MACHINE_SELECTOR_H

#include "splashkit.h"
#include "ButtonNode.h"
#include <string>

class Selector {
private:
    /// Checks first button.
    bool m_isFirstButton = true;
    /// Splashscreen cursor sprite. 
    sprite m_cursorSprite;
    /// Checks if game menu currently sliding left.
    bool m_isSlidingLeft = false;
    /// Checks if game menu currently sliding right.
    bool m_isSlidingRight = false;
    /// Checks if current button is from game menu.
    bool m_isFromGameMenu;
    /// Check to render cursor on Selector
    bool m_renderCursor = false;

public:
    Selector() {}
    Selector(const std::string &cursor);

    // Properties used to detect if game menu slide is occurring.
    auto getSlideLeft() const -> const bool& { return this->m_isSlidingLeft; }
    auto getSlideRight() const -> const bool& { return this->m_isSlidingRight; }
    auto setSlideLeft(bool left) { this->m_isSlidingLeft = left; }
    auto setSlideRight(bool right) { this->m_isSlidingRight = right; }
    auto setRenderCursor(bool cursor) { this->m_renderCursor = cursor; }
    // Return the cursor sprite
    sprite getCursor() { return this->m_cursorSprite; }

    ButtonNode* checkKeyInput(ButtonNode* buttonNode, bool isFromGameMenu = false);
    std::string checkForSelection(ButtonNode* buttonNode, bool isFromGameMenu = false);
    void highlightFirst(ButtonNode* buttonNode);
    void highlightButton(ButtonNode* buttonNode, std::string direction);
    void setNoRenderCursor();
    
};

#endif