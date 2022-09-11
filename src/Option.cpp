#include "Option.h"

Option::Option()
{
}

void Option::createOptionsButtons()
{
    // Initialise grid
    GridLayout grid(ROWS, COLS);
    this->m_grid = grid;

    // Initialise audio
    Audio audio;
    this->m_audio = audio;

    // Do we need config data in the options menu ?? maybe we will..
    // this->m_configs = this->m_helper.ConfigDataList();
    // Create options buttons
    Button *home = new OptionsScreenButton(Button::HOME, 0.5);
    Button *sound = new OptionsScreenButton(Button::SOUND, 0.5);
    Button *stats = new OptionsScreenButton(Button::STATS, 0.5);
    Button *display = new OptionsScreenButton(Button::DISPLAY, 0.5);

    // Add options buttons to local vector
    this->m_optionsBtns.push_back(home);
    this->m_optionsBtns.push_back(sound);
    this->m_optionsBtns.push_back(stats);
    this->m_optionsBtns.push_back(display);

    // Fetch options menu background
    bitmap options_thoth = bitmap_named("options_thoth");

    // Update grid cells with brackground asset
    this->m_grid.setBackground(options_thoth);

    // Make new nodes with buttons.
    this->m_optionsButtonNode = new ButtonNode(home);
    this->m_optionsButtonNode->addAfter(new ButtonNode(sound));
    this->m_optionsButtonNode->addBefore(new ButtonNode(display));
    this->m_optionsButtonNode->getPrev()->addBefore(new ButtonNode(stats));

    // Update grid with nodes.
    this->m_grid.updateCell(m_optionsButtonNode->button, 0, 1);
    this->m_grid.updateCell(m_optionsButtonNode->getNext()->button, 1, 1);
    this->m_grid.updateCell(m_optionsButtonNode->getPrev()->button, 3, 1);
    this->m_grid.updateCell(m_optionsButtonNode->getPrev()->getPrev()->button, 2, 1);
}

void Option::drawOptionsMenu()
{
    // Get mouse position
    this->m_mouse = mouse_position();

    // Draw grid
    this->m_grid.drawGrid();

    // Draw cursor -- no need for cursor in options menu, highlights work well
    // draw_sprite(this->m_selectorMainMenu.getCursor());

    // Get button postions
    Cell home = this->m_grid.getCell(0, 1);
    Cell sound = this->m_grid.getCell(1, 1);
    Cell display = this->m_grid.getCell(3, 1);
    Cell stats = this->m_grid.getCell(2, 1);

    // Options title text
    int font_size = 100;
    draw_text("Options", COLOR_BLACK, "font_title", font_size, (ARCADE_MACHINE_RES_X / 4), 50);

    // Check input in selector class.
    this->m_optionsButtonNode = this->m_selectorOptionsMenu.checkKeyInput(this->m_optionsButtonNode);

    int x_pos = home.button->x() + (sprite_width(home.button->btn()) - 40);
    int y_pos;

    if (this->m_optionsButtonNode->button->color() == "opts_home")
        y_pos = home.button->y() + ((sprite_width(home.button->btn()) * 0.5) - 30);
    else if (this->m_optionsButtonNode->button->color() == "opts_sound")
        y_pos = sound.button->y() + ((sprite_width(sound.button->btn()) * 0.5) - 30);
    else if (this->m_optionsButtonNode->button->color() == "opts_display")
        y_pos = display.button->y() + ((sprite_width(display.button->btn()) * 0.5) - 30);
    else if (this->m_optionsButtonNode->button->color() == "opts_stats")
        y_pos = stats.button->y() + ((sprite_width(stats.button->btn()) * 0.5) - 30);

    draw_text(this->m_optionsButtonNode->button->color().substr(5),
              COLOR_BLACK,  // colour
              "font_title", // font
              60,           // size
              x_pos,        // x position
              y_pos         // y position
    );

    // Check input in selector class.
    this->m_action = this->m_selectorOptionsMenu.checkForSelection(this->m_optionsButtonNode);
}

bool Option::checkAction()
{
    if (this->m_action == "home")
        return true;

    if (this->m_action == "sound")
        soundMenu();

    return false;
}

void Option::soundMenu()
{
    write_line("into sound options mini menu");

    // draw_text("Volume", COLOR_BLACK, "font_title", 60, 300, 200);
}

float Option::getVolume()
{
    return m_volume / 100;
}

void Option::volumeControl()
{
    if (_selector == 2 && m_isSelected && key_typed(RIGHT_KEY) && m_volume < 100 && m_insideSeletor == 1)
        m_volume += 20;

    if (_selector == 2 && m_isSelected && key_typed(LEFT_KEY) && m_volume > 0 && m_insideSeletor == 1)
        m_volume -= 20;
}

void Option::setCurrentMusic()
{
    if (_selector == 2 && m_isSelected && m_volume < 100 && m_insideSeletor == 2) {
        if (key_typed(LEFT_KEY) && m_currentMusic > 1)
            m_currentMusic = m_currentMusic - 1;
        if (key_typed(RIGHT_KEY) && m_currentMusic < 3)
            m_currentMusic = m_currentMusic + 1;
    }
}

int Option::getCurrentMusic()
{
    return m_currentMusic;
}

void Option::changeDisplay()
{
    if (_selector == 3 && m_isSelected) {
        switch (m_displayStyle) {
        case 1:
            if (key_typed(RIGHT_KEY))
                m_displayStyle = 2;
            else if (key_typed(DOWN_KEY))
                m_displayStyle = 3;
            break;
        case 2:
            if (key_typed(DOWN_KEY))
                m_displayStyle = 4;
            else if (key_typed(LEFT_KEY))
                m_displayStyle = 1;
            break;
        case 3:
            if (key_typed(UP_KEY))
                m_displayStyle = 1;
            else if (key_typed(RIGHT_KEY))
                m_displayStyle = 4;
            break;
        }
    }
}

bool Option::isChangeMusic()
{
    if (_selector == 2 && m_isSelected && m_volume < 100 && m_insideSeletor == 2 &&
        (key_typed(LEFT_KEY) || key_typed(RIGHT_KEY))) {
        return true;
    } else
        return false;
}

bool Option::isChangeVoLume()
{
    if (_selector == 2 && m_isSelected && m_volume < 100 && m_insideSeletor == 1 &&
        (key_typed(LEFT_KEY) || key_typed(RIGHT_KEY))) {
        return true;
    } else
        return false;
}

void Option::changeSelector()
{
    if (key_typed(P_KEY)) {
        if (m_isSelected == false) {
            m_isSelected = true;
        } else if (m_isSelected == true) {
            m_isSelected = false;
        }
    }

    if (key_typed(DOWN_KEY) && _selector < 4 && !m_isSelected) {
        _selector = _selector + 1;
    }

    if (key_typed(UP_KEY) && _selector > 1 && !m_isSelected) {
        _selector = _selector - 1;
    }

    if (_selector == 2 && m_isSelected == true && m_insideSeletor == 1) {
        if (key_typed(DOWN_KEY)) {
            m_insideSeletor = 2;
        }
    }

    if (_selector == 2 && m_isSelected == true && m_insideSeletor == 2) {
        if (key_typed(UP_KEY)) {
            m_insideSeletor = 1;
        }
    }
}

void Option::updateOption()
{
    if (m_isOptionOpen) {
        changeSelector();
        changeDisplay();
        setCurrentMusic();
        volumeControl();
        drawIntinialHub();
    }

    // if(!_isOptionOpen&&key_typed(O_KEY)){_isOptionOpen=true;}
    // if(_isOptionOpen&&key_typed(O_KEY)){_isOptionOpen=false;}

    if (key_typed(O_KEY)) {
        m_isOptionOpen = !m_isOptionOpen;
    }
}

void Option::drawIntinialHub()
{

    draw_bitmap("back_ground", bitmap_width("backCurrentGame"), 0);

    if (_selector == 1) {
        draw_bitmap("backCurrentGame", 0, 0);
        draw_bitmap("sound_notSelected", 0, bitmap_height("backCurrentGame"));
        // draw_bitmap();
        draw_bitmap("backMenu_notSelected", 0, 3 * bitmap_height("backCurrentGame"));
        draw_text("RESET DEFAULT", color_red(), 400, 100);
        draw_text("yes", color_red(), 300, 300);
        draw_text("no", color_red(), 700, 300);
    }

    if (_selector == 2) {
        draw_bitmap("changeSound", 0, bitmap_height("backCurrentGame"));
        draw_bitmap("backGame_notSelected", 0, 0);
        // draw_bitmap();
        draw_bitmap("backMenu_notSelected", 0, 3 * bitmap_height("backCurrentGame"));

        fill_rectangle(color_red(), bitmap_width("backCurrentGame") + 200, 200,
                       screen_width() - bitmap_width("backCurrentGame") - 400, 50);
        int a = screen_width() - bitmap_width("backCurrentGame") - 400;
        int b = a / 100 * m_volume;
        fill_rectangle(color_yellow(), bitmap_width("backCurrentGame") + 200, 200, b, 50);
        draw_text(std::to_string(screen_width() - bitmap_width("backCurrentGame")), color_red(), 300, 300);

        if (m_insideSeletor == 1) {
            draw_text("CHANGE SOUND", color_white(), bitmap_width("backCurrentGame") + 100, 200);
        }

        if (m_insideSeletor == 2) {
            draw_text("CHANGE MUSIC", color_white(), bitmap_width("backCurrentGame") + 100, 500);
            draw_text(std::to_string(m_currentMusic), color_white(), bitmap_width("backCurrentGame") + 250, 500);
        }
    }

    if (_selector == 3) {
        double x = screen_width();
        double y = screen_height();
        double a = bitmap_width("backCurrentGame");
        double rec_width = (x - a - 450) / 2;
        double rec_height = (y - 450) / 2;
        double first_column_x = 150 + a;
        double second_column_x = a + 300 + rec_width;
        double first_row_y = 150;
        double second_row_y = 300 + rec_height;
        fill_rectangle(color_white(), first_column_x, first_row_y, rec_width, rec_height);
        fill_rectangle(color_white(), second_column_x, first_row_y, rec_width, rec_height);
        fill_rectangle(color_white(), first_column_x, second_row_y, rec_width, rec_height);
        fill_rectangle(color_white(), second_column_x, second_row_y, rec_width, rec_height);
        if (m_displayStyle == 1) {
            fill_rectangle(color_red(), first_column_x, first_row_y, rec_width, rec_height);
        }
        if (m_displayStyle == 2) {
            fill_rectangle(color_red(), second_column_x, first_row_y, rec_width, rec_height);
        }
        if (m_displayStyle == 3) {
            fill_rectangle(color_red(), first_column_x, second_row_y, rec_width, rec_height);
        }
        if (m_displayStyle == 4) {
            fill_rectangle(color_red(), second_column_x, second_row_y, rec_width, rec_height);
        }
    }

    if (_selector == 4) {
        draw_bitmap("backMenu", 0, 3 * bitmap_height("backCurrentGame"));
        draw_bitmap("sound_notSelected", 0, bitmap_height("backCurrentGame"));
        // draw_bitmap();
        draw_bitmap("backGame_notSelected", 0, 0);
    }
}