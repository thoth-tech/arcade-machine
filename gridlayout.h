
void write(std::string text);


enum item
{
    EMPTY,
    BITMAP,
    SPRITE,
    BUTTON
};

class cell
{
public:
    item cellType = EMPTY;
    bitmap bmp = NULL;
    sprite sprite = NULL;
    Button* button;
    int span = 1;
};

class Grid
{
private:
    // Stores the number of columns per row
    int *_colsArray;
    // Different number of columns per row?
    bool _useColsArray = false;
    // Columns, used if number of columns is fixed
    int _cols;
    // Rows
    int _rows;
    // Stores information for each cell (bitmap, span)
    cell *_grid;
    // Number of cells
    int _cells;
    // Scale the bitmap to fill the cell
    bool _scaleToFit;

public:
    // Constructor:
    // Fixed number of columns
    Grid(int rows, int cols, bool scaleToFit = false)
    {
        _scaleToFit = scaleToFit;
        _rows = rows;
        _cols = cols;
        // Calculate number of cells
        _cells = _rows * _cols;
        // Initialise the grid
        _grid = new cell[_cells];
    }
    // Dynamic number of columns per row
    Grid(int rows, int colsArray[], bool scaleToFit = false)
    {
        // Add check to ensure that the length of colsArray matches the number of rows
        _scaleToFit = scaleToFit;
        _useColsArray = true;
        _rows = rows;
        _colsArray = colsArray;
        // Calculate the number of cells in the grid
        for (size_t i = 0; i < rows; i++)
        {
            // Sum of all columns in each row
            _cells += colsArray[i];
        }
        // Initialise the grid
        _grid = new cell[_cells];
    }

    //Calculate the bitmap scaling factor, returns options
    drawing_options BitmapScaleOpt(int bmpWidth, int bmpHeight, double cellWidth, double cellHeight, int span)
    {
        return option_scale_bmp((cellWidth / bmpWidth) * span, cellHeight / bmpHeight);
    }
    
    // Draw the cell boundaries, to help with placement
    void DrawCells()
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
                xOffset = current_window_width() / _colsArray[i];
                // Number of columns to iterate over
                _cols = _colsArray[i];
            }
            // Iterate over columns
            for (size_t j = 0; j < _cols; j++)
            {
                draw_rectangle(COLOR_BLACK,(double)(xOffset * j), (double)(yOffset * i), (double)xOffset, (double)yOffset);
                ++index;
            }
        }
    }

    // Draw the items
    void DrawGrid()
    {
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
                xOffset = current_window_width() / _colsArray[i];
                // Number of columns to iterate over
                _cols = _colsArray[i];
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
                    // Draw object into cell, centre using dimensions
                    switch (_grid[index].cellType)
                    {
                    case BITMAP:
                        if (_scaleToFit)
                        {
                            options = BitmapScaleOpt(bitmap_width(_grid[index].bmp), bitmap_height(_grid[index].bmp), xOffset, yOffset, _grid[index].span);
                        }
                        draw_bitmap(_grid[index].bmp, j * xOffset + (((xOffset * _grid[index].span) - bitmap_width(_grid[index].bmp)) / 2), i * yOffset + ((yOffset - bitmap_height(_grid[index].bmp)) / 2), options);
                        break;
                    case SPRITE:
                        if (_scaleToFit)
                            write("ScaleToFit: Feature not currently available with use of sprites.\n");
                        draw_sprite(_grid[index].sprite, j * xOffset + (((xOffset * _grid[index].span) - sprite_width(_grid[index].sprite)) / 2), i * yOffset + ((yOffset - sprite_height(_grid[index].sprite)) / 2));
                        break;
                    case BUTTON:
                        if (_scaleToFit)
                            write("ScaleToFit: Feature not currently available with use of sprites.\n");
                        sprite_set_x(_grid[index].button->btn(),  j * xOffset + (((xOffset * _grid[index].span) - sprite_width(_grid[index].button->_btn)) / 2));
                        sprite_set_y(_grid[index].button->btn(),  (i * yOffset + _grid[index].button->centre_y()));
                        draw_sprite(_grid[index].button->btn());
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

    // Find a cell in the grid using row/col
    int FindCell(int row, int col)
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
            if (_colsArray[row] < col + 1)
            {
                throw std::out_of_range("Column index out of range");
                return -1;
            }
            // Calculate the cell number
            for (size_t i = 0; i < row; i++)
            {
                cellNum += _colsArray[i];
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

    // Update a cell with a specified bitmap
    void UpdateCell(bitmap bmp, int row, int col, int span = 1)
    {
        // Stores the index of the cell
        int cellNum = FindCell(row, col);
        // Selected row is out of bounds
        // Update the cell
        _grid[cellNum].cellType = BITMAP;
        _grid[cellNum].sprite = NULL;
        _grid[cellNum].bmp = bmp;
        _grid[cellNum].button = NULL;
        _grid[cellNum].span = span;
    }

    // Update a cell with a specified sprite
    void UpdateCell(sprite sprite, int row, int col, int span = 1)
    {
        // Stores the index of the cell
        int cellNum = FindCell(row, col);
        // Selected row is out of bounds
        // Update the cell
        _grid[cellNum].cellType = SPRITE;
        _grid[cellNum].sprite = sprite;
        _grid[cellNum].bmp = NULL;
        _grid[cellNum].button = NULL;
        _grid[cellNum].span = span;
    }
    // Update a cell with a specified button
    void UpdateCell(Button *button, int row, int col, int span = 1)
    {
        // Stores the index of the cell
        int cellNum = FindCell(row, col);
        // Selected row is out of bounds
        // Update the cell
        _grid[cellNum].cellType = BUTTON;
        _grid[cellNum].sprite = NULL;
        _grid[cellNum].bmp = NULL;
        _grid[cellNum].button = button;
        _grid[cellNum].span = span;
    }

    // Update all cells with a specified bitmap
    void UpdateAllCells(bitmap bmp)
    {
        // Iterate over all the cells
        for (size_t i = 0; i < _cells; i++)
        {
            // Update bitmap
            _grid[i].cellType = BITMAP;
            _grid[i].sprite = NULL;
            _grid[i].bmp = bmp;
        }
    }

    // Update all cells with a specified sprite
    void UpdateAllCells(sprite sprite)
    {
        // Iterate over all the cells
        for (size_t i = 0; i < _cells; i++)
        {
            // Update bitmap
            _grid[i].cellType = SPRITE;
            _grid[i].sprite = sprite;
            _grid[i].bmp = NULL;
        }
    }

    // Draw the layout to console, used for testing
    void DrawLayout()
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
                colWidth = current_window_width() / _colsArray[i];
                _cols = _colsArray[i];
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

    // Clear the grid
    void ClearGrid()
    {
        // Iterate over all the cells
        for (size_t i = 0; i < _cells; i++)
        {
            // Reset cell to default
            _grid[i].cellType = EMPTY;
            _grid[i].sprite = NULL;
            _grid[i].bmp = NULL;
            _grid[i].span = 1;
        }
    }
};
