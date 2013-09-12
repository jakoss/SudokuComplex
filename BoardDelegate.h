#ifndef BOARDDELEGATE_H
#define BOARDDELEGATE_H

#include <QStyledItemDelegate>
#include "BoardModel.h"
#include "SudokuClass.h"

class BoardDelegate : public QStyledItemDelegate{
private:
    BoardModel * model;
    SudokuClass * sudoku;
public:
    BoardDelegate(BoardModel* model, SudokuClass * sudoku);
    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    SudokuClass * getSudoku();
};

#endif // BOARDDELEGATE_H
