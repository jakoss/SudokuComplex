#include "Cell.h"

Cell::Cell(int posX, int posY, int val, bool fixed)
{
    this->posX = posX;
    this->posY = posY;
    this->val = val;
    this->fixed = fixed;
    this->hints.reserve(9);
    this->clicked = false;
    for(int i = 0; i < 9; i++){
        this->hints.push_back(false);
    }
    //domyślny "krzyż" na planszy
    QColor background;
    if( ( ( (posX - 1) / 3 ) & 1 ) == ( ( (posY - 1) / 3 ) & 1 ) )
        background = QColor( Qt::white );
    else
        background = QColor( 225, 225, 225, 255 );
    this->cellColor = background;
    this->defCellColor = background;
    this->defFontColor = QColor(Qt::black);
}

int Cell::getVal()
{
    return this->val;
}

void Cell::setVal(int val)
{
    if((val < 0) || (val > 9)) return;
    this->val = val;
}

void Cell::addHint(int pos)
{
    if((pos < 0) || pos > 8) return;
    this->hints[pos] = true;
}

void Cell::removeHint(int pos)
{
    if((pos < 0) || pos > 8) return;
    this->hints[pos] = false;
}

bool Cell::isHint(int pos)
{
    if((pos < 0) || pos > 8) return false;
    return this->hints[pos];
}

bool Cell::getFixed()
{
    return this->fixed;
}

void Cell::setClicked(bool clicked)
{
    this->clicked = clicked;
}

bool Cell::getClicked()
{
    return this->clicked;
}

int Cell::getRow()
{
    return this->posX;
}

int Cell::getCol()
{
    return this->posY;
}

QColor Cell::getCellColor()
{
    return this->cellColor;
}

void Cell::setCellColor(QColor color)
{
    this->cellColor = color;
}

void Cell::setDefColor()
{
    this->cellColor = this->defCellColor;
}

QColor Cell::getFontColor()
{
    return this->fontColor;
}

void Cell::setFontColor(QColor color)
{
    this->fontColor = color;
}

void Cell::setDefFontColor()
{
    this->fontColor = this->defFontColor;
}


