#ifndef ARCADE_MACHINE_SELECTOR_H
#define ARCADE_MACHINE_SELECTOR_H

using namespace std;

class Selector {
    private:
        /// Checks first button.
        bool first = true;
        /// Splashscreen cursor sprite. 
        sprite cursor;
        /// Checks if game menu currently sliding left.
        bool slide_left = false;
        /// Checks if game menu currently sliding right.
        bool slide_right = false;
        /// Checks if current button is from game menu.
        bool game_menu;

    public:
        Selector(){}
        Selector(const string &cursor)
        {
            bitmap cur = bitmap_named("cursor");
            this->cursor = create_sprite(cur);
        }

        // Properties used to detect if game menu slide is occurring.
        auto get_slide_left() const -> const bool& { return this->slide_left; }
        auto get_slide_right() const -> const bool& { return this->slide_right; }
        auto set_slide_left(bool left) { slide_left = left; }
        auto set_slide_right(bool right) { slide_right = right; }

        // Return the cursor sprite
        sprite get_cursor()
        {
            return this->cursor;
        }

        /**
         * @brief Checks key input, determining whether from game menu or splashscreen and updates that button.
         * 
         * @param button_node The current button that is selected.
         * @param game_menu Checking if from game menu.
         * @return ButtonNode* 
         */
        ButtonNode* check_key_input(ButtonNode* button_node, bool game_menu = false)
        {
            this->game_menu = game_menu;

            // Highlight play button on start.
            if (first == true) 
            {
                highlight_first(button_node);
            }
            
            // If it is the game menu only allow left/right arrows selection.
            if (game_menu == true)
            {
                // Check to ensure menu isn't currently sliding.
                if (slide_left == false && slide_right == false)
                {
                    // Slide left.
                    if (key_typed(LEFT_KEY) && !key_typed(RIGHT_KEY))
                    {
                        slide_left = true;
                        // Previous button becomes current button.
                        button_node = button_node->getPrev();
                        // Highlight the center/current button.
                        highlight_button(button_node, "prev");
                    }
                    // Slide right.
                    if (key_typed(RIGHT_KEY) && !key_typed(LEFT_KEY))
                    {
                        slide_right = true;
                        // Next button becomes current button.
                        button_node = button_node->getNext();
                        // Highlight the center/current button.
                        highlight_button(button_node, "next");
                    }
                }
            }
            // Else use up/down selection.
            else if (game_menu == false)
            {
                // Move the selector up.
                if (key_typed(UP_KEY))
                {
                    // Previous button becomes current button.
                    button_node = button_node->getPrev();
                    // Highlight the current button.
                    highlight_button(button_node, "prev");
                    // move cursor
                    sprite_set_y(this->cursor, sprite_y(button_node->button->btn()));
                }
                // Move the selector down.
                if (key_typed(DOWN_KEY))
                {
                    // next button becomes current button.
                    button_node = button_node->getNext();
                    // Highlight the current button.
                    highlight_button(button_node, "next");
                    // Move cursor.
                    sprite_set_y(this->cursor, sprite_y(button_node->button->btn()));
                }
            }

            return button_node;
        }

        /**
         * @brief Checks for selection of a button.
         * 
         * @param button_node The current button that is selected.
         * @param game_menu Check if selection is coming from game menu.
         * @return ** string 
         */
        string check_for_selection(ButtonNode* button_node, bool game_menu = false)
        {
            // If selection not from game menu.
            if (game_menu == false)
            {
                // Return key returns the action of the selected button.
                if(key_typed(RETURN_KEY)) 
                    return button_node->button->action();
            }
            else
            {
                // Return key returns the action of the selected button.
                if(key_typed(RETURN_KEY)) 
                     return button_node->button->action("return");

                // Escape key returns the action of the selected button.
                if(key_typed(ESCAPE_KEY)) 
                    return button_node->button->action("escape");
            }

            return "";
        }

        /**
         * @brief Highlights the first button upon page load.
         * 
         * @param button_node The current selected button.
         * @return ** void 
         */
        void highlight_first(ButtonNode* button_node)
        {
            // Get the current buttons sprite.
            sprite currentSprite = button_node->button->m_btn;
            // Toggle on the highlight layer.
            sprite_toggle_layer_visible(currentSprite, 1);

            // Set start location of cursor.
            if (!game_menu)
            {
                sprite_set_x(this->cursor, sprite_x(button_node->button->btn()) - 200);
                sprite_set_y(this->cursor, sprite_y(button_node->button->btn()));
            }

            first = false;
        }

        /**
         * @brief Highlights the current selected button.
         * 
         * @param button_node The current button.
         * @param direction The direction the selector is moving.
         * @return ** void 
         */
        void highlight_button(ButtonNode* button_node, string direction)
        {
            // Sprite to store the previous sprite.
            sprite prev_sprite;

            // Get current sprite.
            sprite current_sprite = button_node->button->m_btn;

            // Toggle current sprites highlight layer.
            sprite_toggle_layer_visible(current_sprite, 1);

            // Get previous sprite by checking direction of movement.
            if (direction == "prev")
                prev_sprite = button_node->getNext()->button->btn();
            else if (direction == "next")
                prev_sprite = button_node->getPrev()->button->btn();

            // Toggle previous sprite highlight layer off.
            sprite_toggle_layer_visible(prev_sprite, 1);
        }
};

#endif