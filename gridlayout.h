
void write(std::string text);

class cell
{
public:
    std::string bmpName;
    int span;
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
        // Initialise the cells
        for (size_t i = 0; i < _cells; i++)
        {
            _grid[i].bmpName = "";
            _grid[i].span = 1;
        }
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
        // Initialise the cells
        for (size_t i = 0; i < _cells; i++)
        {
            _grid[i].bmpName = "";
            _grid[i].span = 1;
        }
    }
    // Draw the bitmaps
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
                if (_grid[index].bmpName != "")
                {
                    // Scale the bitmap to fill the cell
                    if (_scaleToFit)
                    {
                        // Calculate the scaling factor
                        //(xOffset = cell width, yOffset = cell height)
                        options = option_scale_bmp((xOffset / bitmap_width(_grid[index].bmpName)) * _grid[index].span, yOffset / bitmap_height(_grid[index].bmpName));
                    }
                    // Draw bitmap into cell, centre using bitmap dimensions
                    draw_bitmap(_grid[index].bmpName, j * xOffset + (((xOffset * _grid[index].span) - bitmap_width(_grid[index].bmpName)) / 2), i * yOffset + ((yOffset - bitmap_height(_grid[index].bmpName)) / 2), options);
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
    // Update a cell with a specified bitmap
    void UpdateCell(string bmp, int row, int col, int span = 1)
    {
        // Stores the index of the cell
        int cellNum = 0;
        // Selected row is out of bounds
        if (_rows <= row)
        {
            throw std::out_of_range("Row index out of range");
        }
        // Dynamic number of columns/row
        if (_useColsArray)
        {
            // Selected column is out of bounds
            if (_colsArray[row] < col + 1)
            {
                throw std::out_of_range("Column index out of range");
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
        }
        // Fixed number of columns
        else
        {
            // Calculate the cell number
            cellNum = row * _cols + col;
        }
        // Update the cell
        _grid[cellNum].bmpName = bmp;
        _grid[cellNum].span = span;
    }
    // Update all cells with a specified bitmap
    void UpdateAllCells(string bmp)
    {
        // Iterate over all the cells
        for (size_t i = 0; i < _cells; i++)
        {
            // Update bitmap
            _grid[i].bmpName = bmp;
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
            _grid[i].bmpName = "";
            _grid[i].span = 1;
        }
    }
};
