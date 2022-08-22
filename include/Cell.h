#ifndef ARCADE_MACHINE_CELL_H
#define ARCADE_MACHINE_CELL_H

enum item
{
    EMPTY,
    BMP,
    SPT,
    BTN
};

class Cell
{
public:
    item cellType = EMPTY;
    bitmap bmp = NULL;
    sprite spr = NULL;
    Button *button;
    int span = 1;
    bool centre = true;
};

#endif