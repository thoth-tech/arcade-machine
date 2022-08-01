#ifndef ARCADE_MACHINE_TIP_H
#define ARCADE_MACHINE_TIP_H

// Font size
#define FONT_SIZE 20
// Width of the border
#define BORDER_WIDTH 3
// Space to leave between the content and container
#define CONTENT_BUFFER 10
// Space offset from the window-border
#define WBORDER_OFFSET 20

// Possible locations of the container
enum location
{
    TOPLEFT,
    TOPRIGHT,
    TOPCENTER,
    BOTLEFT,
    BOTRIGHT,
    BOTCENTER
};

class Tip
{
private:
    bitmap image;
    //bitmap dimensions
    int bmpWidth;
    int bmpHeight;
    string text;
    //Length of the string, text
    int textLength;
    //Number of characters per line
    int charsPerLine = 0;
    //Number of lines
    int numLines;
    //Location within the window where the container lies
    location loc;
    //Where the container will be anchored within the screen.
    int xOffset;
    int yOffset;
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    int duration;

    //Height of the container
    int containerHeight;
    //Width of the container
    int containerWidth;

    //Stores the animation
    animation anim = NULL;
    //Drawing options required to load the animation
    drawing_options opt = option_defaults();

    int i = 30;

    /**
     * @brief calculate the positioning of the container
     * 
     */
    void CalculatePosition()
    {
        switch (loc)
        {
        case TOPLEFT:
            xOffset = WBORDER_OFFSET;
            yOffset = WBORDER_OFFSET;
            break;
        case TOPRIGHT:
            xOffset = screen_width() - containerWidth - WBORDER_OFFSET;
            yOffset = WBORDER_OFFSET;
            break;
        case TOPCENTER:
            xOffset = screen_width() / 2 - containerWidth / 2;
            yOffset = WBORDER_OFFSET;
            break;
        case BOTRIGHT:
            xOffset = screen_width() - containerWidth - WBORDER_OFFSET;
            yOffset = screen_height() - containerHeight - WBORDER_OFFSET;
            break;
        case BOTLEFT:
            xOffset = WBORDER_OFFSET;
            yOffset = screen_height() - containerHeight - WBORDER_OFFSET;
            break;
        case BOTCENTER:
            xOffset = screen_width() / 2 - containerWidth / 2;
            yOffset = screen_height() - containerHeight - WBORDER_OFFSET;
        default:
            break;
        }
    }
public:
    Tip(){};
    /**
     * @brief Construct a new Tip object
     * 
     * @param text the text to be displayed
     * @param image the image to be displayed
     * @param duration the duration of the tip
     * @param charsPerLine the number of characters per line
     * @param loc the location of the container
     */
    Tip(string text, bitmap image, int duration = 3000, int charsPerLine = 30, location loc = TOPCENTER)
    {
        this->text = text;
        this->textLength = text.length();
        this->charsPerLine = charsPerLine;
        this->image = image;
        this->loc = loc;
        this->duration = duration;

        //Initialise bitmap
        bmpWidth = bitmap_width(image);
        bmpHeight = bitmap_height(image);
        //Calculate number of lines
        numLines = textLength / charsPerLine;
        //Calculate container height based off lines of text
        containerHeight = numLines * FONT_SIZE + FONT_SIZE + 2 * CONTENT_BUFFER;
        //If the bitmap is bigger than the container, resize
        if (containerHeight < bmpHeight)
            containerHeight = 2*CONTENT_BUFFER + bmpHeight;
        //Calculate container width based on number of characters per line and bitmap width
        containerWidth = charsPerLine * 9 + 3 * CONTENT_BUFFER + bmpWidth;

        CalculatePosition();
    };
    /**
     * @brief Construct a new Tip object
     * 
     * @param text the text to be displayed 
     * @param image the image to be displayed
     * @param anim the animation to be displayed
     * @param opt the drawing options for the bitmap
     * @param duration the duration of the tip
     * @param charsPerLine the number of characters per line
     * @param loc the location of the container
     */
    Tip(string text, bitmap image, animation anim, drawing_options opt, int duration = 3000, int charsPerLine = 30, location loc = TOPCENTER)
    {
        this->text = text;
        this->textLength = text.length();
        this->charsPerLine = charsPerLine;
        this->image = image;
        this->anim = anim;
        this->opt = opt;
        this->loc = loc;
        this-> duration = duration;

        //Initialise bitmap
        bmpWidth = bitmap_cell_width(image);
        bmpHeight = bitmap_cell_height(image);
        //Calculate number of lines
        numLines = textLength / charsPerLine;
        //Calculate container height based off lines of text
        containerHeight = numLines * FONT_SIZE + FONT_SIZE + 2 * CONTENT_BUFFER;
        //If the bitmap is bigger than the container, resize
        if (containerHeight < bmpHeight)
            containerHeight = 2*CONTENT_BUFFER + bmpHeight;
        //Calculate container width based on number of characters per line and bitmap width
        containerWidth = charsPerLine * 9 + 3 * CONTENT_BUFFER + bmpWidth;

        CalculatePosition();
    };
    ~Tip();
    
    /**
     * @brief draw the tip
     * 
     */
    void draw()
    {
        //Initialise startTime upon first draw
        if (startTime.time_since_epoch().count() == 0)
            startTime = std::chrono::steady_clock::now();
        //The tip has been visible for more than the specified duration, stop drawing
        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count() > 3000)
            return;

        //Draw border rectangle
        //NOTE: Variable i is used to scale the rectangle, each function call, animating the border.
        fill_rectangle(rgba_color(0.0, 67.5, 75.7, 0.30), xOffset - BORDER_WIDTH, yOffset - BORDER_WIDTH, (containerWidth + (BORDER_WIDTH * 2)) / (i / 2), containerHeight + (BORDER_WIDTH * 2));
        if (i != 2)
            i--;
        //Draw container rectangle
        fill_rectangle(COLOR_BLACK, xOffset, yOffset, containerWidth, containerHeight);
        //Draw icon
        draw_bitmap(image, xOffset + CONTENT_BUFFER, yOffset - bmpWidth / 2 + containerHeight / 2, opt);
        //Draw the text
        for (int i = 0; i < numLines + 1; i++)
        {
            draw_text(text.substr(i * charsPerLine, charsPerLine), COLOR_WHITE, "font_text", FONT_SIZE, xOffset + CONTENT_BUFFER * 2 + (bmpWidth), (FONT_SIZE * i) + yOffset + CONTENT_BUFFER);
        }

        //Update the animation
        if (anim)
            update_animation(anim);
    }
};

#endif