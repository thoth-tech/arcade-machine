#ifndef ARCADE_MACHINE_GRID_LAYOUT_H
#define ARCADE_MACHINE_GRID_LAYOUT_H

#include "Button.h"
#include "Cell.h"
#include "splashkit.h"
#include <string>

class GridLayout
{
private:
    // Stores the bitmap used for the background
    bitmap m_background = nullptr;
    // Stores the number of columns per row
    int *m_colsArray;
    // Different number of columns per row?
    bool _useColsArray = false;
    // Columns, used if number of columns is fixed
    int _cols;
    // Rows
    int _rows;
    // Stores information for each cell (bitmap, span)
    Cell *_grid;
    // Number of cells
    int _cells = 0;
    // Scale the bitmap to fill the cell
    bool _scaleToFit;
    // Does the grid contain configured cells
    bool _gridEmpty = true;

public:
    GridLayout()
    {
    }
    GridLayout(int rows, int cols, bool scaleToFit = false);
    GridLayout(int rows, int colsArray[], bool scaleToFit = false);

    void destroy(); // clears memory

    void setBackground(bitmap bmp);

    drawing_options bitmapScaleOpt(int bmpWidth, int bmpHeight, double cellWidth, double cellHeight, int span);

    void drawCells();
    void drawGrid();

    int findCell(int row, int col);
    Cell getCell(int row, int col);

    void updateCell(const bitmap &bmp, int row, int col, int span = 1, bool centre = true);
    void updateCell(const sprite &sprite, int row, int col, int span = 1, bool centre = true);
    void updateCell(Button *button, int row, int col, int span = 1, bool centre = true);

    void updateAllCells(bitmap bmp, bool centre = true);
    void updateAllCells(sprite sprite, bool centre = true);

    void drawLayout();
    void clearGrid();
    point_2d findCellFromLoc(int x, int y);
    void clearCell(int row, int col);
};

#endif