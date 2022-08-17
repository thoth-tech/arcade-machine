#include "GridLayout.h"

#include <stdexcept> // used for std::out_of_range


/**
* Construct a new grid object with a fixed number of columns/rows
* @param rows number of rows
* @param cols number of columns
* @param scaleToFit scale the image to fill the cell
*/
GridLayout::GridLayout(int rows, int cols, bool scaleToFit)
{
    _scaleToFit = scaleToFit;
    _rows = rows;
    _cols = cols;
    // Calculate number of cells
    _cells = _rows * _cols;
    // Initialise the grid
    _grid = new Cell[_cells];
}

/**
* @brief Construct a new Grid object with a dynamic number of columns per row
* 
* @param rows number of rows
* @param colsArray array of columns per row
* @param scaleToFit scale the image to fill the cell
*/
GridLayout::GridLayout(int rows, int colsArray[], bool scaleToFit)
{
    // Add check to ensure that the length of colsArray matches the number of rows
    _scaleToFit = scaleToFit;
    _useColsArray = true;
    _rows = rows;
    m_colsArray = colsArray;
    // Calculate the number of cells in the grid
    for (size_t i = 0; i < rows; i++)
    {
        // Sum of all columns in each row
        _cells += colsArray[i];
    }
    // Initialise the grid
    _grid = new Cell[_cells];
}

/**
* @brief Set the background
* 
* @param bmp bitmap to use as background
*/
void GridLayout::setBackground(bitmap bmp)
{
    m_background = bmp;
}

/**
* @brief Calculate the bitmap scaling factor
* 
* @param bmp bitmap to scale
* @param cellWidth width of the cell
* @param cellHeight height of the cell
* @return drawing_options options
*/
drawing_options GridLayout::bitmapScaleOpt(int bmpWidth, int bmpHeight, double cellWidth, double cellHeight, int span)
{
    return option_scale_bmp((cellWidth / bmpWidth) * span, cellHeight / bmpHeight);
}

/**
* @brief Draw the cell boundaries, to help with placement
* 
*/
void GridLayout::drawCells()
{
    // Vertical offset between each cell
    double yOffset = current_window_height() / _rows;
    // Horizontal offset between each cell
    double xOffset;
    // Calculate the horizontal offset
    // If the number of columns is fixed
    if (!_useColsArray)
        xOffset = current_window_width() / _cols;

    int index = 0;
    // Iterate over rows
    for (size_t i = 0; i < _rows; i++)
    {
        // If user specified dynamic number of columns per row
        if (_useColsArray)
        {
            // Update horizontal offset each row
            xOffset = current_window_width() / m_colsArray[i];
            // Number of columns to iterate over
            _cols = m_colsArray[i];
        }
        // Iterate over columns
        for (size_t j = 0; j < _cols; j++)
        {
            draw_rectangle(COLOR_BLACK, (double)(xOffset * j), (double)(yOffset * i), (double)xOffset, (double)yOffset);
            ++index;
        }
    }
}

/**
* @brief Draw the items in the grid
* 
*/
void GridLayout::drawGrid()
{
    if (m_background)
        draw_bitmap(m_background, 0, 0);
    // Vertical offset between each cell
    double yOffset = current_window_height() / _rows;
    // Horizontal offset between each cell
    double xOffset;
    // How many iterations to skip, (if a cell spans multiple columns)
    int skipIterations = 0;
    drawing_options options = option_defaults();
    // Calculate the horizontal offset
    // If the number of columns is fixed
    if (!_useColsArray)
        xOffset = current_window_width() / _cols;

    int index = 0;
    // Iterate over rows
    for (size_t i = 0; i < _rows; i++)
    {
        // If user specified dynamic number of columns per row
        if (_useColsArray)
        {
            // Update horizontal offset each row
            xOffset = current_window_width() / m_colsArray[i];
            // Number of columns to iterate over
            _cols = m_colsArray[i];
        }
        // Iterate over columns
        for (size_t j = 0; j < _cols; j++)
        {
            // Skip an iteration
            if (skipIterations > 0)
            {
                --skipIterations;
                ++index;
                continue;
            }
            // If the cell is not empty
            if (_grid[index].cellType != EMPTY)
            {
                double x = (xOffset * j);
                double y = (yOffset * i);

                // Draw object into cell, centre using dimensions
                switch (_grid[index].cellType)
                {
                case BMP:
                    if (_scaleToFit)
                    {
                        options = bitmapScaleOpt(bitmap_width(_grid[index].bmp), bitmap_height(_grid[index].bmp), xOffset, yOffset, _grid[index].span);
                    }
                    if (_grid[index].centre)
                    {
                        x = x + (((xOffset * _grid[index].span) - bitmap_width(_grid[index].bmp)) / 2);
                        y = y + ((yOffset - bitmap_height(_grid[index].bmp)) / 2);
                    }
                    draw_bitmap(_grid[index].bmp, x, y, options);
                    break;
                case SPT:
                    if (_scaleToFit)
                        write("ScaleToFit: Feature not currently available with use of sprites.\n");
                    if (_grid[index].centre)
                    {
                        x = x + (((xOffset * _grid[index].span) - sprite_width(_grid[index].spr)) / 2);
                        y = y + ((yOffset - sprite_height(_grid[index].spr)) / 2);
                    }
                    draw_sprite(_grid[index].spr, x, y);
                    break;
                case BTN:
                    if (_scaleToFit)
                        write("ScaleToFit: Feature not currently available with use of sprites.\n");
                    if (_grid[index].centre)
                    {
                        x = x + ((xOffset * _grid[index].span) / 2) - _grid[index].button->centreX();
                        y = y + _grid[index].button->centreY();
                    }
                    sprite_set_x(_grid[index].button->btn(), x);
                    sprite_set_y(_grid[index].button->btn(), y);
                    // Update Button position of button
                    this->_grid[index].button->setX(sprite_x(this->_grid[index].button->btn()));
                    this->_grid[index].button->setY(sprite_y(this->_grid[index].button->btn()));
                    // Draw Button
                    _grid[index].button->drawButton();
                    break;
                default:
                    break;
                }
            }
            // If the cell spans multiple columns, skip the next iterations
            if (_grid[index].span > 1)
            {
                skipIterations = _grid[index].span - 1;
            }
            ++index;
        }
    }
}

/**
* @brief Find a cell in the grid using row/col
* 
* @param row row of the cell
* @param col column of the cell
* @return int index of the cell
*/
int GridLayout::findCell(int row, int col)
{
    int cellNum = 0;
    // Selected row is out of bounds
    if (_rows <= row)
    {
        throw std::out_of_range("Row index out of range");
        return -1;
    }
    // Dynamic number of columns/row
    if (_useColsArray)
    {
        // Selected column is out of bounds
        if (m_colsArray[row] < col + 1)
        {
            throw std::out_of_range("Column index out of range");
            return -1;
        }
        // Calculate the cell number
        for (size_t i = 0; i < row; i++)
        {
            cellNum += m_colsArray[i];
        }

        cellNum += col;
    }
    // Selected column is out of bounds
    else if (_cols <= col)
    {
        throw std::out_of_range("Column index out of range");
        return -1;
    }
    // Fixed number of columns
    else
    {
        // Calculate the cell number
        cellNum = row * _cols + col;
    }
    return cellNum;
}

/**
* @brief Get a cell from the grid using row/col
* 
* @param row row of the cell
* @param col column of the cell
* @return cell* pointer to the cell
*/
Cell GridLayout::getCell(int row, int col)
{
    return _grid[findCell(row, col)];
}

/**
* @brief Update a cell with a specified bitmap
* 
* @param bmp bitmap to update the cell with
* @param row row of the cell
* @param col column of the cell
* @param span number of columns the bitmap spans
* @param centre whether the bitmap should be centered
*/
void GridLayout::updateCell(const bitmap &bmp, int row, int col, int span, bool centre)
{
    _gridEmpty=false;
    // Stores the index of the cell
    int cellNum = findCell(row, col);
    // Selected row is out of bounds
    // Update the cell
    _grid[cellNum].cellType = BMP;
    _grid[cellNum].spr = NULL;
    _grid[cellNum].bmp = bmp;
    _grid[cellNum].button = NULL;
    _grid[cellNum].span = span;
    _grid[cellNum].centre = centre;
}

/**
* @brief Update a cell with a specified sprite
* 
* @param sprite sprite to update the cell with
* @param row row of the cell
* @param col column of the cell
* @param span number of columns the sprite spans
* @param centre whether the sprite should be centered
*/
void GridLayout::updateCell(const sprite &sprite, int row, int col, int span, bool centre)
{
    _gridEmpty=false;
    // Stores the index of the cell
    int cellNum = findCell(row, col);
    // Selected row is out of bounds
    // Update the cell
    _grid[cellNum].cellType = SPT;
    _grid[cellNum].spr = sprite;
    _grid[cellNum].bmp = NULL;
    _grid[cellNum].button = NULL;
    _grid[cellNum].span = span;
    _grid[cellNum].centre = centre;
}

/**
* @brief Update a cell with a specified button
* 
* @param button button to update the cell with
* @param row row of the cell
* @param col column of the cell
* @param span number of columns the button spans
* @param centre whether the button should be centered
*/
void GridLayout::updateCell(Button *button, int row, int col, int span, bool centre)
{
    _gridEmpty=false;
    // Stores the index of the cell
    int cellNum = findCell(row, col);
    // Selected row is out of bounds
    // Update the cell
    _grid[cellNum].cellType = BTN;
    _grid[cellNum].spr = NULL;
    _grid[cellNum].bmp = NULL;
    _grid[cellNum].button = button;
    _grid[cellNum].span = span;
    _grid[cellNum].centre = centre;
}

/**
* @brief Update all cells with a specified bitmap
* 
* @param bmp bitmap to update the cells with
* @param centre whether the bitmaps should be centered
*/
void GridLayout::updateAllCells(bitmap bmp, bool centre)
{
    _gridEmpty=false;
    // Iterate over all the cells
    for (size_t i = 0; i < _cells; i++)
    {
        // Update bitmap
        _grid[i].cellType = BMP;
        _grid[i].spr = NULL;
        _grid[i].bmp = bmp;
        _grid[i].centre = centre;
    }
}

/**
* @brief Update all cells with a specified sprite
* 
* @param sprite sprite to update the cells with
* @param centre whether the sprites should be centered
*/
void GridLayout::updateAllCells(sprite sprite, bool centre)
{
    _gridEmpty=false;
    // Iterate over all the cells
    for (size_t i = 0; i < _cells; i++)
    {
        // Update bitmap
        _grid[i].cellType = SPT;
        _grid[i].spr = sprite;
        _grid[i].bmp = NULL;
        _grid[i].centre = centre;
    }
}

/**
* @brief Log the dimensions of the cells to console
* 
*/
void GridLayout::drawLayout()
{
    int colWidth = 0;
    write("Drawing layout\n");
    write("rowHeight: " + std::to_string(current_window_height() / _rows) + "\n");
    if (!_useColsArray)
        colWidth = current_window_width() / _cols;
    for (size_t i = 0; i < _rows; i++)
    {
        if (_useColsArray)
        {
            colWidth = current_window_width() / m_colsArray[i];
            _cols = m_colsArray[i];
        }
        write("Row " + std::to_string(i) + " (colWidth: " + std::to_string(colWidth) + "): ");
        for (size_t j = 0; j < _cols; j++)
        {
            string content = "[" + std::to_string(i) + "," + std::to_string(j) + "]";
            write(content);
        }
        write("\n");
    }
}

/**
* @brief Clear the grid
* 
*/
void GridLayout::clearGrid()
{
    if (_gridEmpty)
        return;
    // Iterate over all the cells
    for (size_t i = 0; i < _cells; i++)
    {
        // Reset cell to default
        _grid[i].cellType = EMPTY;
        _grid[i].spr = NULL;
        _grid[i].bmp = NULL;
        _grid[i].span = 1;
        _grid[i].centre = true;
    }
    // Grid is now empty
    _gridEmpty = true;  
}

/**
* @brief Find the nearest cells row/col from x, y coordinates
* 
* @param x x-coordinate (px)
* @param y y-coordinate (px)
* @return point_2d 
*/
point_2d GridLayout::findCellFromLoc(int x, int y)
{
    int rowNum;
    // Selected row is out of bounds
    int yOffset = current_window_height() / _rows;
    int runningSum = 0;
    for (int i = 0; i < _rows; i++)
    {
        if (y >= runningSum && y < runningSum + yOffset)
        {
            rowNum = i;
        }
        runningSum += yOffset;
    }
    int xOffset;
    if (_useColsArray)
        xOffset = (current_window_width() / m_colsArray[rowNum]);
    else
        xOffset = (current_window_width() / _cols);
    runningSum = 0;
    int colNum;
    for (int i = 0; i < _cols; i++)
    {
        if (x >= runningSum && x < runningSum  + xOffset)
        {
            colNum = i;
        }
        runningSum += xOffset;
    }
    point_2d point;
    point.x = colNum;
    point.y = rowNum;
    return point;
}

/**
* @brief Clear the cell
* 
*/
void GridLayout::clearCell(int row, int col)
{
    // Gets the index of the cell
    int cellNum = findCell(row, col);
    // Clear cell
    _grid[cellNum].cellType = EMPTY;
    _grid[cellNum].spr = NULL;
    _grid[cellNum].bmp = NULL;
    _grid[cellNum].button = NULL;
    _grid[cellNum].span = 1;
    _grid[cellNum].centre = true;
}