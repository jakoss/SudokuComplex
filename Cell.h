#ifndef CELL_H
#define CELL_H

#include <vector>
#include <QColor>

class Cell{
private:
    int posX;
    int posY;
    int val;
    bool fixed;
    bool clicked;
    QColor cellColor;
    QColor defCellColor;
    QColor defFontColor;
    QColor fontColor;
    std::vector<bool> hints;
public:
    Cell(int posX, int posY, int val, bool fixed = false);
    int getVal();
    void setVal(int val);
    void addHint(int pos);
    void removeHint(int pos);
    bool isHint(int pos);
    bool getFixed();
    void setClicked(bool clicked);
    bool getClicked();
    int getRow();
    int getCol();
    QColor getCellColor();
    void setCellColor(QColor color);
    void setDefColor();
    QColor getFontColor();
    void setFontColor(QColor color);
    void setDefFontColor();
};

#endif // CELL_H
