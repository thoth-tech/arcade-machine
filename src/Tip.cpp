#include "Tip.h"

/**
 * @brief calculate the positioning of the container
 *
 */
void Tip::calculatePosition()
{
    switch (m_loc) {
    case TOPLEFT:
        m_xOffset = WBORDER_OFFSET;
        m_yOffset = WBORDER_OFFSET;
        break;
    case TOPRIGHT:
        m_xOffset = screen_width() - m_containerWidth - WBORDER_OFFSET;
        m_yOffset = WBORDER_OFFSET;
        break;
    case TOPCENTER:
        m_xOffset = screen_width() / 2 - m_containerWidth / 2;
        m_yOffset = WBORDER_OFFSET;
        break;
    case BOTRIGHT:
        m_xOffset = screen_width() - m_containerWidth - WBORDER_OFFSET;
        m_yOffset = screen_height() - m_containerHeight - WBORDER_OFFSET;
        break;
    case BOTLEFT:
        m_xOffset = WBORDER_OFFSET;
        m_yOffset = screen_height() - m_containerHeight - WBORDER_OFFSET;
        break;
    case BOTCENTER:
        m_xOffset = screen_width() / 2 - m_containerWidth / 2;
        m_yOffset = screen_height() - m_containerHeight - WBORDER_OFFSET;
    default:
        break;
    }
}

Tip::Tip(string text, bitmap image, int duration, int charsPerLine, location loc)
{
    this->m_text = text;
    this->m_textLength = text.length();
    this->m_charsPerLine = charsPerLine;
    this->m_image = image;
    this->m_loc = loc;
    this->m_duration = duration;

    // Initialise bitmap
    m_bmpWidth = bitmap_width(image);
    m_bmpHeight = bitmap_height(image);
    // Calculate number of lines
    m_numLines = m_textLength / charsPerLine;
    // Calculate container height based off lines of text
    m_containerHeight = m_numLines * FONT_SIZE + FONT_SIZE + 2 * CONTENT_BUFFER;
    // If the bitmap is bigger than the container, resize
    if (m_containerHeight < m_bmpHeight)
        m_containerHeight = 2 * CONTENT_BUFFER + m_bmpHeight;
    // Calculate container width based on number of characters per line and bitmap width
    m_containerWidth = charsPerLine * 9 + 3 * CONTENT_BUFFER + m_bmpWidth;

    calculatePosition();
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
Tip::Tip(string text, bitmap image, animation anim, drawing_options opt, int duration, int charsPerLine, location loc)
{
    this->m_text = text;
    this->m_textLength = text.length();
    this->m_charsPerLine = charsPerLine;
    this->m_image = image;
    this->m_anim = anim;
    this->m_opt = opt;
    this->m_loc = loc;
    this->m_duration = duration;

    // Initialise bitmap
    m_bmpWidth = bitmap_cell_width(image);
    m_bmpHeight = bitmap_cell_height(image);
    // Calculate number of lines
    m_numLines = m_textLength / charsPerLine;
    // Calculate container height based off lines of text
    m_containerHeight = m_numLines * FONT_SIZE + FONT_SIZE + 2 * CONTENT_BUFFER;
    // If the bitmap is bigger than the container, resize
    if (m_containerHeight < m_bmpHeight)
        m_containerHeight = 2 * CONTENT_BUFFER + m_bmpHeight;
    // Calculate container width based on number of characters per line and bitmap width
    m_containerWidth = charsPerLine * 9 + 3 * CONTENT_BUFFER + m_bmpWidth;

    calculatePosition();
};

Tip::~Tip()
{
    std::cout << "Destructor called on Tip\n";
}

/**
 * @brief draw the tip
 *
 */
void Tip::draw()
{
    // Initialise startTime upon first draw
    if (m_startTime.time_since_epoch().count() == 0)
        m_startTime = std::chrono::steady_clock::now();

    // The tip has been visible for more than the specified duration, stop drawing
    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_startTime).count() >
        3000)
        return;

    // Draw border rectangle
    // NOTE: Variable i is used to scale the rectangle, each function call, animating the border.
    fill_rectangle(rgba_color(0.0, 67.5, 75.7, 0.30), m_xOffset - BORDER_WIDTH, m_yOffset - BORDER_WIDTH,
                   (m_containerWidth + (BORDER_WIDTH * 2)) / (m_i / 2), m_containerHeight + (BORDER_WIDTH * 2));
    if (m_i != 2)
        m_i--;

    // Draw container rectangle
    fill_rectangle(COLOR_BLACK, m_xOffset, m_yOffset, m_containerWidth, m_containerHeight);
    // Draw icon
    draw_bitmap(m_image, m_xOffset + CONTENT_BUFFER, m_yOffset - m_bmpWidth / 2 + m_containerHeight / 2, m_opt);
    // Draw the text
    for (int i = 0; i < m_numLines + 1; i++)
        draw_text(m_text.substr(i * m_charsPerLine, m_charsPerLine), COLOR_WHITE, "font_text", FONT_SIZE,
                  m_xOffset + CONTENT_BUFFER * 2 + (m_bmpWidth), (FONT_SIZE * i) + m_yOffset + CONTENT_BUFFER);

    // Update the animation
    if (m_anim)
        update_animation(m_anim);
}
