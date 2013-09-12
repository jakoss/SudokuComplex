#include "boardDelegate.h"
#include "BoardModel.h"
#include <QString>
#include <QPainter>
#include <QPointF>
#include <QFont>
#include <QEvent>
#include <QStandardItem>
#include "SudokuClass.h"

using namespace std;

BoardDelegate::BoardDelegate(BoardModel* model, SudokuClass *sudoku){
    this->model = model;
    this->sudoku = sudoku;
}

void BoardDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    Cell *cell = this->sudoku->getCell(index.row()+1, index.column()+1);
    QStyledItemDelegate::paint(painter, option, index);
    //tło komórki
    painter->fillRect(option.rect, cell->getCellColor());

    QFont font("Times", 18);
    if(cell->getFixed())
        font.setBold(true);
    QPointF point;
    painter->setPen(cell->getFontColor());

    //rysowanie liczby głównej
    if(cell->getVal() > 0){
        QFontMetrics fm(font);
        painter->setFont(font);
        point.setX(option.rect.left() + option.rect.width() / 2 - (fm.width(QString::number(cell->getVal())) / 2));
        point.setY(option.rect.top() + option.rect.height() / 2 + (fm.height() / 2) - 6);
        painter->drawText(point, QString::number(cell->getVal()));
    }else{
        //rysowanie "hintów"
        font.setPointSize(8);
        QFontMetrics fms(font);
        painter->setFont(font);
        //blok ifów, wygląda brzydko, ale jest wydajniejszy od pętli z switchem w środku
        if(cell->isHint(0)){
            point.setX(option.rect.left() + 4);
            point.setY(option.rect.top() + fms.height() - 2);
            painter->drawText(point, "1");
        }
        if(cell->isHint(1)){
            point.setX(option.rect.left() + option.rect.width() / 2 - (fms.width("2") / 2));
            point.setY(option.rect.top() + fms.height() - 2);
            painter->drawText(point, "2");
        }
        if(cell->isHint(2)){
            point.setX(option.rect.left() + option.rect.width() - fms.width("3") - 4);
            point.setY(option.rect.top() + fms.height() - 2);
            painter->drawText(point, "3");
        }
        if(cell->isHint(3)){
            point.setX(option.rect.left() + 4);
            point.setY(option.rect.top() + option.rect.height() / 2 + (fms.height() / 2) - 3);
            painter->drawText(point, "4");
        }
        if(cell->isHint(4)){
            point.setX(option.rect.left() + option.rect.width() / 2 - (fms.width("5") / 2));
            point.setY(option.rect.top() + option.rect.height() / 2 + (fms.height() / 2) - 3);
            painter->drawText(point, "5");
        }
        if(cell->isHint(5)){
            point.setX(option.rect.left() + option.rect.width() - fms.width("6") - 4);
            point.setY(option.rect.top() + option.rect.height() / 2 + (fms.height() / 2) - 3);
            painter->drawText(point, "6");
        }
        if(cell->isHint(6)){
            point.setX(option.rect.left() + 4);
            point.setY(option.rect.top() + option.rect.height() - 4);
            painter->drawText(point, "7");
        }
        if(cell->isHint(7)){
            point.setX(option.rect.left() + option.rect.width() / 2 - (fms.width("5") / 2));
            point.setY(option.rect.top() + option.rect.height() - 4);
            painter->drawText(point, "8");
        }
        if(cell->isHint(8)){
            point.setX(option.rect.left() + option.rect.width() - fms.width("6") - 4);
            point.setY(option.rect.top() + option.rect.height() - 4);
            painter->drawText(point, "9");
        }
    }
}

QSize BoardDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(50,50);
}

SudokuClass *BoardDelegate::getSudoku()
{
    return this->sudoku;
}
