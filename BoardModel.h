#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include <QAbstractTableModel>
#include "Cell.h"
#include "SudokuClass.h"

class BoardModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    BoardModel();
    int rowCount(const QModelIndex& index) const;
    int columnCount(const QModelIndex& index) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
protected:
};

#endif // BOARDMODEL_H
