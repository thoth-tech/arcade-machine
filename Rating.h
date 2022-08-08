#ifndef ARCADE_MACHINE_RATING_H
#define ARCADE_MACHINE_RATING_H

#include "GridLayout.h"

class Rating
{
private:
    Grid _grid;
    int _rating;
    int _maxRating;
    bool _ratingCollected;

public:
    Rating()
    {
        _ratingCollected = false;
        _grid = Grid(3, 7);
        _rating = 1;
        _maxRating = 5;
        _grid.SetBackground(bitmap_named("rating_bg"));

        updateGrid();
    }
    ~Rating() {}

    //Draw the rating page
    void draw()
    {
        handleInput();
        _grid.DrawGrid();
        draw_text("Rate your experience",  COLOR_WHITE, "font_title", 100 ,480, 120);  
    }

    //handle input, to be moved to selector?
    void handleInput()
    {
        if (key_typed(LEFT_KEY))
        {
            --_rating;
            if (_rating < 0)
                _rating = _maxRating;
            updateGrid();
        }
        else if (key_typed(RIGHT_KEY))
        {
            // rating is between 1 and 5
                ++_rating;
                _rating = _rating % (_maxRating + 1);

            updateGrid();
        }
        else if (key_typed(RETURN_KEY))
        {
            _ratingCollected = true;
        }
    }

    int getRating() {
        _rating = 1;
        _ratingCollected = false;
        updateGrid();
        process_events();
        while (!_ratingCollected)
        {
            clear_screen();
            draw();
            process_events();
            refresh_screen(60);
        }
        return _rating;
    }

    //update the grid, when the rating changes
    void updateGrid()
    {
        {
            _grid.ClearGrid();
            for (int i = 1; i <= _maxRating; i++)
            {
                if (i <= _rating)
                    _grid.UpdateCell(bitmap_named("star-gold"), 1, i);
                else
                    _grid.UpdateCell(bitmap_named("star-black"), 1, i);
            }
        }
    }
};

#endif