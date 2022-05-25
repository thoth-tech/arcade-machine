
void write(std::string text);

enum item
{
    EMPTY,
    BMP,
    SPT,
    BTN
};

class cell
{
public:
    item cellType = EMPTY;
    bitmap bmp = NULL;
    sprite sprite = NULL;
    Button *button;
    int span = 1;
    bool centre = true;
};

class Grid
{
private:
    // Stores the bitmap used for the background
    bitmap _background = NULL;
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
    // Does the grid contain configured cells
    bool _gridEmpty = true;

public:
    // Constructor:
    // Default Constructor
    Grid(){}
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

    // Update the background of the screen
    void SetBackground(bitmap bmp)
    {
        _background = bmp;
    }

    // Calculate the bitmap scaling factor, returns options
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
                draw_rectangle(COLOR_BLACK, (double)(xOffset * j), (double)(yOffset * i), (double)xOffset, (double)yOffset);
                ++index;
            }
        }
    }

    // Draw the items
    void DrawGrid()
    {
        if (_background)
           draw_bitmap(_background, 0, 0);
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
                    double x = (xOffset * j);
                    double y = (yOffset * i);

                    // Draw object into cell, centre using dimensions
                    switch (_grid[index].cellType)
                    {
                    case BMP:
                        if (_scaleToFit)
                        {
                            options = BitmapScaleOpt(bitmap_width(_grid[index].bmp), bitmap_height(_grid[index].bmp), xOffset, yOffset, _grid[index].span);
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
                            x = x + (((xOffset * _grid[index].span) - sprite_width(_grid[index].sprite)) / 2);
                            y = y + ((yOffset - sprite_height(_grid[index].sprite)) / 2);
                        }
                        draw_sprite(_grid[index].sprite, x, y);
                        break;
                    case BTN:
                        if (_scaleToFit)
                            write("ScaleToFit: Feature not currently available with use of sprites.\n");
                        if (_grid[index].centre)
                        {
                            x = x + ((xOffset * _grid[index].span) / 2) - _grid[index].button->centre_x();
                            y = y + _grid[index].button->centre_y();
                        }
                        sprite_set_x(_grid[index].button->btn(), x);
                        sprite_set_y(_grid[index].button->btn(), y);
                        // Update Button position of button
                        this->_grid[index].button->set_x(sprite_x(this->_grid[index].button->btn()));
                        this->_grid[index].button->set_y(sprite_y(this->_grid[index].button->btn()));
                        // Draw Button
                        _grid[index].button->draw_button();
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

    // Get a cell from the grid using row/col
    cell GetCell(int row, int col)
    {
        return _grid[FindCell(row, col)];
    }

    // Update a cell with a specified bitmap
    void UpdateCell(const bitmap &bmp, int row, int col, int span = 1, bool centre = true)
    {
        _gridEmpty=false;
        // Stores the index of the cell
        int cellNum = FindCell(row, col);
        // Selected row is out of bounds
        // Update the cell
        _grid[cellNum].cellType = BMP;
        _grid[cellNum].sprite = NULL;
        _grid[cellNum].bmp = bmp;
        _grid[cellNum].button = NULL;
        _grid[cellNum].span = span;
        _grid[cellNum].centre = centre;
    }

    // Update a cell with a specified sprite
    void UpdateCell(const sprite &sprite, int row, int col, int span = 1, bool centre = true)
    {
        _gridEmpty=false;
        // Stores the index of the cell
        int cellNum = FindCell(row, col);
        // Selected row is out of bounds
        // Update the cell
        _grid[cellNum].cellType = SPT;
        _grid[cellNum].sprite = sprite;
        _grid[cellNum].bmp = NULL;
        _grid[cellNum].button = NULL;
        _grid[cellNum].span = span;
        _grid[cellNum].centre = centre;
    }
    // Update a cell with a specified button
    void UpdateCell(Button *button, int row, int col, int span = 1, bool centre = true)
    {
        _gridEmpty=false;
        // Stores the index of the cell
        int cellNum = FindCell(row, col);
        // Selected row is out of bounds
        // Update the cell
        _grid[cellNum].cellType = BTN;
        _grid[cellNum].sprite = NULL;
        _grid[cellNum].bmp = NULL;
        _grid[cellNum].button = button;
        _grid[cellNum].span = span;
        _grid[cellNum].centre = centre;
    }

    // Update all cells with a specified bitmap
    void UpdateAllCells(bitmap bmp, bool centre = true)
    {
        _gridEmpty=false;
        // Iterate over all the cells
        for (size_t i = 0; i < _cells; i++)
        {
            // Update bitmap
            _grid[i].cellType = BMP;
            _grid[i].sprite = NULL;
            _grid[i].bmp = bmp;
            _grid[i].centre = centre;
        }
    }

    // Update all cells with a specified sprite
    void UpdateAllCells(sprite sprite, bool centre = true)
    {
        _gridEmpty=false;
        // Iterate over all the cells
        for (size_t i = 0; i < _cells; i++)
        {
            // Update bitmap
            _grid[i].cellType = SPT;
            _grid[i].sprite = sprite;
            _grid[i].bmp = NULL;
            _grid[i].centre = centre;
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
        if (_gridEmpty)
            return;
        // Iterate over all the cells
        for (size_t i = 0; i < _cells; i++)
        {
            // Reset cell to default
            _grid[i].cellType = EMPTY;
            _grid[i].sprite = NULL;
            _grid[i].bmp = NULL;
            _grid[i].span = 1;
            _grid[i].centre = true;
        }
        // Grid is now empty
        _gridEmpty = true;  
    }
};
