#include "BoardModel.h"
#include "Cell.h"
#include "SudokuClass.h"

using namespace std;

BoardModel::BoardModel() : QAbstractTableModel()
{
}

int BoardModel::rowCount(const QModelIndex& index) const
{
    return 9;
}

int BoardModel::columnCount(const QModelIndex& index) const
{
    return 9;
}

QVariant BoardModel::data(const QModelIndex& index, int role) const
{
    return QVariant::Invalid;
}

QVariant BoardModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant::Invalid;
}

QModelIndex BoardModel::index(int row, int column, const QModelIndex &parent) const
{
    return this->createIndex(row, column);
}
