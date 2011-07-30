/*****************************************************************************
    This file is part of Voxigame.

    Copyright (C) 2011 Jean-Marie Favreau <J-Marie.Favreau@u-clermont1.fr>
                       Université d'Auvergne (France)

    Voxigame is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Voxigame is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

 *****************************************************************************/

#include "ListModelBoard.hxx"

ListModelBoard::ListModelBoard(QObject * p) : QAbstractListModel(p), board(NULL) {
}

ListModelBoard::ListModelBoard(const InteractiveBoard & b, QObject * p) : QAbstractListModel(p), board(&b) {
}

void ListModelBoard::setBoard(const InteractiveBoard & b) {
  board = &b;
}

const InteractiveBoard & ListModelBoard::getBoard() const {
  return *board;
}

QVariant ListModelBoard::data(const QModelIndex & i, int role) const {
  if ((i.row() < 0) || ((unsigned int)i.row() >= (*board).getNbPieces()))
    return QVariant();

  if ((role == Qt::DisplayRole) || (role == Qt::EditRole)) {
    return (*board).at(i.row()).getDescription();
  }

  return QVariant();
}

Qt::ItemFlags ListModelBoard::flags(const QModelIndex & i) const {
  if (!i.isValid())
    return QAbstractItemModel::flags(i) | Qt::ItemIsDropEnabled;

  return Qt::ItemIsUserCheckable | Qt::ItemIsSelectable;
}

bool ListModelBoard::insertRows(int row, int count, const QModelIndex & p) {
  // TODO
}

bool ListModelBoard::removeRows(int row, int count, const QModelIndex & p) {
  // TODO
}

int ListModelBoard::rowCount(const QModelIndex & p) const {
  if (p.isValid())
    return 0;

  return (*board).getNbPieces();
}

bool ListModelBoard::setData(const QModelIndex & i, const QVariant & value, int role) {
  // TODO
}


Qt::DropActions ListModelBoard::supportedDropActions() const {
  return QAbstractItemModel::supportedDropActions();
}
