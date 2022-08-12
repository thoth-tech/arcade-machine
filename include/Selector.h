#ifndef ARCADE_MACHINE_SELECTOR_H
#define ARCADE_MACHINE_SELECTOR_H

#include "splashkit.h"
#include "ButtonNode.h"
#include <string>

using namespace std;

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

    public:
        Selector(){}
        Selector(const string &cursor)
        {
            bitmap cur = bitmap_named("cursor");
            this->m_cursorSprite = create_sprite(cur);
        }

        // Properties used to detect if game menu slide is occurring.
        auto getSlideLeft() const -> const bool& { return this->m_isSlidingLeft; }
        auto getSlideRight() const -> const bool& { return this->m_isSlidingRight; }
        auto setSlideLeft(bool left) { m_isSlidingLeft = left; }
        auto setSlideRight(bool right) { m_isSlidingRight = right; }

        // Return the cursor sprite
        sprite getCursor()
        {
            return this->m_cursorSprite;
        }

        /**
         * @brief Checks key input, determining whether from game menu or splashscreen and updates that button.
         * 
         * @param buttonNode The current button that is selected.
         * @param gameMenu Checking if from game menu.
         * @return ButtonNode* 
         */
        ButtonNode* checkKeyInput(ButtonNode* buttonNode, bool isFromGameMenu = false)
        {
            this->m_isFromGameMenu = isFromGameMenu;

            // Highlight play button on start.
            if (m_isFirstButton) 
                highlightFirst(buttonNode);
            
            // If it is the game menu only allow left/right arrows selection.
            if (isFromGameMenu)
            {
                // Check to ensure menu isn't currently sliding.
                if (! m_isSlidingLeft && ! m_isSlidingRight)
                {
                    // Slide left.
                    if (key_typed(LEFT_KEY) && !key_typed(RIGHT_KEY))
                    {
                        m_isSlidingLeft = true;
                        // Previous button becomes current button.
                        buttonNode = buttonNode->getPrev();
                        // Highlight the center/current button.
                        highlightButton(buttonNode, "prev");
                    }
                    // Slide right.
                    if (key_typed(RIGHT_KEY) && !key_typed(LEFT_KEY))
                    {
                        m_isSlidingRight = true;
                        // Next button becomes current button.
                        buttonNode = buttonNode->getNext();
                        // Highlight the center/current button.
                        highlightButton(buttonNode, "next");
                    }
                }
            }
            else 
            {
                // Move the selector up.
                if (key_typed(UP_KEY))
                {
                    // Previous button becomes current button.
                    buttonNode = buttonNode->getPrev();
                    // Highlight the current button.
                    highlightButton(buttonNode, "prev");
                    // move cursor
                    sprite_set_y(this->m_cursorSprite, sprite_y(buttonNode->button->btn()));
                }
                // Move the selector down.
                if (key_typed(DOWN_KEY))
                {
                    // next button becomes current button.
                    buttonNode = buttonNode->getNext();
                    // Highlight the current button.
                    highlightButton(buttonNode, "next");
                    // Move cursor.
                    sprite_set_y(this->m_cursorSprite, sprite_y(buttonNode->button->btn()));
                }
            }

            return buttonNode;
        }

        /**
         * @brief Checks for selection of a button.
         * 
         * @param buttonNode The current button that is selected.
         * @param gameMenu Check if selection is coming from game menu.
         * @return ** string 
         */
        string checkForSelection(ButtonNode* buttonNode, bool isFromGameMenu = false)
        {
            // If selection not from game menu.
            if (! isFromGameMenu)
            {
                // Return key returns the action of the selected button.
                if (key_typed(RETURN_KEY)) 
                    return buttonNode->button->action();
            }
            else
            {
                // Return key returns the action of the selected button.
                if (key_typed(RETURN_KEY)) 
                    return buttonNode->button->action("return");

                // Escape key returns the action of the selected button.
                if (key_typed(ESCAPE_KEY)) 
                    return buttonNode->button->action("escape");
            }

            return "";
        }

        /**
         * @brief Highlights the first button upon page load.
         * 
         * @param buttonNode The current selected button.
         * @return void 
         */
        void highlightFirst(ButtonNode* buttonNode)
        {
            // Get the current buttons sprite.
            sprite currentSprite = buttonNode->button->m_btn;
            // Toggle on the highlight layer.
            sprite_toggle_layer_visible(currentSprite, 1);

            // Set start location of cursor.
            if (! m_isFromGameMenu)
            {
                sprite_set_x(this->m_cursorSprite, sprite_x(buttonNode->button->btn()) - 200);
                sprite_set_y(this->m_cursorSprite, sprite_y(buttonNode->button->btn()));
            }

            m_isFirstButton = false;
        }

        /**
         * @brief Highlights the current selected button.
         * 
         * @param buttonNode The current button.
         * @param direction The direction the selector is moving.
         * @return ** void 
         */
        void highlightButton(ButtonNode* buttonNode, string direction)
        {
            // Sprite to store the previous sprite.
            sprite prevSprite;

            // Get current sprite.
            sprite currentSprite = buttonNode->button->m_btn;

            // Toggle current sprites highlight layer.
            sprite_toggle_layer_visible(currentSprite, 1);

            // Get previous sprite by checking direction of movement.
            if (direction == "prev")
                prevSprite = buttonNode->getNext()->button->btn();
            else if (direction == "next")
                prevSprite = buttonNode->getPrev()->button->btn();

            // Toggle previous sprite highlight layer off.
            sprite_toggle_layer_visible(prevSprite, 1);
        }
};

#endif