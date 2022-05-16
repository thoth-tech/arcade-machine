
using namespace std;

class Selector {
    private:
        bool first = true; 
        sprite cursor;
        bool slide_left = false;
        bool slide_right = false;
        bool game_menu;

    public:
        Selector(){}
        Selector(const string &cursor)
        {
            bitmap cur = bitmap_named("cursor");
            this->cursor = create_sprite(cur);
        }

        auto get_slide_left() const -> const bool& { return this->slide_left; }
        auto get_slide_right() const -> const bool& { return this->slide_right; }
        auto set_slide_left(bool left) { slide_left = left; }
        auto set_slide_right(bool right) { slide_right = right; }

        // Return the cursor sprite
        sprite get_cursor()
        {
            return this->cursor;
        }

        // Function to check which key is currently pressed and call correct method.
        // Key states ensure the functions only get called once per key press.
        // This function returns a string
        ButtonNode* check_key_input(ButtonNode* button_node, bool game_menu = false)
        {
            this->game_menu = game_menu;

            // highlight play button on start
            if (first == true) 
            {
                highlight_first(button_node);
            }
            
            // If it is the game menu only allow left/right arrows selection.
            if (game_menu == true)
            {
                if (slide_left == false && slide_right == false)
                {
                    if (key_typed(LEFT_KEY) && !key_typed(RIGHT_KEY))
                    {
                        slide_left = true;
                        button_node = button_node->getPrev();
                        highlight_center_button(button_node, "prev");
                    }
                    if (key_typed(RIGHT_KEY) && !key_typed(LEFT_KEY))
                    {
                        slide_right = true;
                        button_node = button_node->getNext();
                        highlight_center_button(button_node, "next");
                    }
                }
            }
            // Else use up/down selection.
            else if (game_menu == false)
            {
                if (key_typed(UP_KEY))
                {
                    write_line(button_node->getPrev()->button->color());
                    button_node = button_node->getPrev();
                    highlight_center_button(button_node, "prev");
                    // move cursor
                    sprite_set_y(this->cursor, sprite_y(button_node->button->btn()));
                }
                if (key_typed(DOWN_KEY))
                {
                    write_line(button_node->getNext()->button->color());
                    button_node = button_node->getNext();
                    highlight_center_button(button_node, "next");
                    // move cursor
                    sprite_set_y(this->cursor, sprite_y(button_node->button->btn()));
                }
            }

            return button_node;
        }

        // Check for return key input.
        string check_for_selection(ButtonNode* button_node, bool game_menu = false)
        {
            if (game_menu == false)
            {
                // Enter key returns the action of the selected button
                if(key_typed(RETURN_KEY)) 
                    return button_node->button->action();
            }
            else
            {
                // Enter key returns the action of the selected button
                if(key_typed(RETURN_KEY)) 
                     return button_node->button->action("return");

                // Esc key returns the action of the selected button
                if(key_typed(ESCAPE_KEY)) 
                    return button_node->button->action("escape");
            }

            return "";
        }

        // Highlights the first button upon page load.
        void highlight_first(ButtonNode* button_node)
        {
            sprite currentSprite = button_node->button->_btn;
            sprite_toggle_layer_visible(currentSprite, 1);

            // set start location of cursor
            if (!game_menu)
            {
                sprite_set_x(this->cursor, sprite_x(button_node->button->btn()) - 200);
                sprite_set_y(this->cursor, sprite_y(button_node->button->btn()));
            }

            first = false;
        }

        // Highlights the selected button.
        void highlight_center_button(ButtonNode* button_node, string direction)
        {
            sprite oldSprite;

            // Get current sprite.
            sprite currentSprite = button_node->button->_btn;

            // Toggle current sprites highlight layer.
            sprite_toggle_layer_visible(currentSprite, 1);

            // Get previous sprite by checking direction of movement.
            if (direction == "prev")
                oldSprite = button_node->getNext()->button->btn();
            else if (direction == "next")
                oldSprite = button_node->getPrev()->button->btn();

            // Toggle previous sprite highlight layer off.
            sprite_toggle_layer_visible(oldSprite, 1);
        }
};