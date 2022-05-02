
using namespace std;

class Selector {
    private:
        int selected = -1;
        bool down = false;
        bool first = true; 
        int up_key_state = 0;
        int down_key_state = 0;
        int left_key_state = 0;
        int right_key_state = 0;
        int count = 0;
        sprite cursor;

    public:
        Selector(){}
        Selector(const string &cursor)
        {
            bitmap cur = load_bitmap("cursor", cursor);
            this->cursor = create_sprite(cur);
        }

        // Return the cursor sprite
        sprite get_cursor()
        {
            return this->cursor;
        }

        // Function to check which key is currently pressed and call correct method.
        // Key states ensure the functions only get called once per key press.
        // This function returns a string
        string check_key_input(vector<Button*> buttons)
        {
            // highlight play button on start
            if (first == true) 
            {
                highlight_first(buttons);
            }

            // Down key pressed
            if (key_down(DOWN_KEY) && down_key_state == 0)
            {
                down_key_state = move_down(buttons, down_key_state);
            }
            else if (key_up(DOWN_KEY) && down_key_state == 1)
            {
                down_key_state = 0;
            }
            // Up key pressed
            else if (key_down(UP_KEY) && up_key_state == 0)
            {
                up_key_state = move_up(buttons, up_key_state);
            }
            else if (key_up(UP_KEY) && up_key_state == 1)
            {
                up_key_state = 0;
            }
            // Left key pressed
            else if (key_down(LEFT_KEY) && left_key_state == 0)
            {
                left_key_state = move_up(buttons, left_key_state);
            }
            else if (key_up(LEFT_KEY) && left_key_state == 1)
            {
                left_key_state = 0;
            }
            // Right key pressed
            else if (key_down(RIGHT_KEY) && right_key_state == 0)
            {
                right_key_state = move_down(buttons, right_key_state);
            }
            else if (key_up(RIGHT_KEY) && right_key_state == 1)
            {
                right_key_state = 0;
            }
            
            // Enter key returns the action of the selected button
            if(key_down(RETURN_KEY)) return buttons[selected]->action();

            return "";
        }

        // Highlights the first button upon page load.
        void highlight_first(vector<Button*> buttons)
        {
            selected = 0;
            highlight_down(buttons);
            first = false;
            if (count > 0)
            {
                // remove the previous highlight
                remove_highlight_down(buttons);
            }
            down_key_state = 1;
            count += 1;
        }

        // Moves the selector down.
        int move_down(vector<Button*> buttons, int key_state)
        {
            // increment selected
            selected += 1;
            // call functin to highlight button
            highlight_down(buttons);
            // this if statement is used to remove the hightlight after first run
            if (count > 0)
            {
                // remove the previous highlight
                remove_highlight_down(buttons);
            }
            key_state = 1;
            count += 1;

            return key_state;
        }

        // Moves the selector up.
        int move_up(vector<Button*> buttons, int key_state)
        {
            // decrease seletced by 1
            selected -= 1;
            // call function to highlight button
            highlight_up(buttons);
            // this if statement is used to remove the hightlight after first run
            if (count > 0)
            {
                // remove the previous highlight
                remove_highlight_up(buttons);
            }
            key_state = 1;
            count += 1;

            return key_state;
        }

        // Highlight button on down arrow key (up arrow key).
        void highlight_down(vector<Button*> buttons)
        {
            // if selected is greater than or equal buttons array size, reset back to zero.
            if (selected >= buttons.size())
            {
                selected = 0;
            }
            // get the current selected sprite
            sprite currentSprite = buttons[selected]->btn();
            // set cursor pos to selected button pos
            sprite_set_y(cursor, buttons[selected]->y());
            sprite_set_x(cursor, buttons[selected]->x() - 200);
            // toggle highlight layer on.
            sprite_toggle_layer_visible(currentSprite, 1);
        }

        // Remove highlight on previous selected button (down arrow key).
        void remove_highlight_down(vector<Button*> buttons)
        {
            int newSelect = selected;
            // if selected is greater than or equal buttons array size, reset back to zero.
            if (newSelect >= buttons.size())
            {
                newSelect = 0;
            }
            // if selected is less than or equal to zero, reset to end of array.
            if (newSelect <= 0)
            {
                newSelect = buttons.size();
            }
            // get the previous selected sprite
            sprite oldSprite = buttons[newSelect-1]->btn();
            // toggle highlight layer off.
            sprite_toggle_layer_visible(oldSprite, 1);
        }

        // Highlight button on up arrow key (up arrow key).
        void highlight_up(vector<Button*> buttons)
        {   
            // if selected is greater than zero reset size to end of array.
            if (selected < 0)
            {
                selected = buttons.size() -1;
            }
            // get the current selected sprite
            sprite currentSprite = buttons[selected]->btn();
            // Set cursor pos to selected button pos
            sprite_set_y(cursor, buttons[selected]->y());
            sprite_set_x(cursor, buttons[selected]->x() - 200);
            // toggle highlight layer on.
            sprite_toggle_layer_visible(currentSprite, 1);
        }

        // Remove highlight on previous selected button (up arrow key).
        void remove_highlight_up(vector<Button*> buttons)
        {
            int new_select = selected;
            sprite old_sprite;

            // if selection is at the end of the array, reset to start of array.
            if (new_select == buttons.size() - 1)
            {
                new_select = -1;
            }
            // if the selection is less than zero, reset to end of array.
            if (new_select < 0)
            {
                new_select = buttons.size() - 1;
            }
            // if selection equals 2, remove highlight on button zero.
            if (new_select == 2)
            {
                old_sprite = buttons[0]->btn();
            }
            // else get the previous selected sprite
            else
            {
                old_sprite = buttons[new_select+1]->btn();
            }
            // toggle highlight layer off.
            sprite_toggle_layer_visible(old_sprite, 1);
        }

};