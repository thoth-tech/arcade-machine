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
    //Width of the bitmap
    int bmpWidth;
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
    
    //Height of the container
    int containerHeight;
    //Width of the container
    int containerWidth;

    //Stores the animation
    animation anim = NULL;
    //Drawing options required to load the animation
    drawing_options opt = option_defaults();

    int i = 30;

    //Calculate the positioning of the container
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
    Tip(string text, bitmap image, int charsPerLine = 30, location loc = TOPCENTER)
    {
        this->text = text;
        this->textLength = text.length();
        this->charsPerLine = charsPerLine;
        this->image = image;
        this->loc = loc;

        //Initialise bitmap
        bmpWidth = bitmap_width(image);

        //Calculate number of lines
        numLines = textLength / charsPerLine;
        //Calculate container dimensions
        containerHeight = numLines * FONT_SIZE + FONT_SIZE + 2 * CONTENT_BUFFER;
        containerWidth = charsPerLine * 9 + 3 * CONTENT_BUFFER + bmpWidth;

        CalculatePosition();
    };
    Tip(string text, bitmap image, animation anim, drawing_options opt, int charsPerLine = 30, location loc = TOPCENTER)
    {
        this->text = text;
        this->textLength = text.length();
        this->charsPerLine = charsPerLine;
        this->image = image;
        this->anim = anim;
        this->opt = opt;
        this->loc = loc;
        //Initialise bitmap
        bmpWidth = bitmap_cell_width(image);

        //Calculate number of lines
        numLines = textLength / charsPerLine;
        //Calculate container dimensions
        containerHeight = numLines * FONT_SIZE + FONT_SIZE + 2 * CONTENT_BUFFER;
        containerWidth = charsPerLine * 9 + 3 * CONTENT_BUFFER + bmpWidth;

        CalculatePosition();
    };
    ~Tip();

    void draw()
    {
        //Draw border rectangle
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