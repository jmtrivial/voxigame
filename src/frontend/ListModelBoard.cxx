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

ListModelBoard::ListModelBoard(QObject * p) : QAbstractTableModel(p), board(NULL) {
}

ListModelBoard::ListModelBoard(const InteractiveBoard & b, QObject * p) : QAbstractTableModel(p), board(&b) {
}

void ListModelBoard::setBoard(const InteractiveBoard & b) {
  board = &b;
}

const InteractiveBoard & ListModelBoard::getBoard() const {
  return *board;
}

QVariant ListModelBoard::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      return tr("Selected");
    case 1:
      return tr("Color");
    case 2:
      return tr("Name");
    case 3:
      return tr("Type");
    default:
      return QVariant();
    }
  }
  else {
    return section + 1;
  }

  return QVariant();
}

Qt::ItemFlags ListModelBoard::flags(const QModelIndex & i) const {
  if (!i.isValid())
    return QAbstractItemModel::flags(i);
  if ((i.row() < 0) || ((unsigned int)i.row() >= (*board).getNbPieces()) ||
      (i.column() < 0) || ((i.column() > 3)))
    return QAbstractItemModel::flags(i);

  if (i.column() == 0)
    return QAbstractItemModel::flags(i) | Qt::ItemIsUserCheckable;
  else if (i.column() == 2)
    return QAbstractItemModel::flags(i) | Qt::ItemIsEditable;
  else
    return QAbstractItemModel::flags(i);

}

QVariant ListModelBoard::data(const QModelIndex & i, int role) const {
  if ((i.row() < 0) || ((unsigned int)i.row() >= (*board).getNbPieces()) ||
      (i.column() < 0) || ((i.column() > 3)))
    return QVariant();

  if ((role == Qt::DisplayRole) || (role == Qt::EditRole)) {
    switch(i.column()) {
    case 0:
    case 1:
      return QVariant();
    case 2:
      return (*board).at(i.row()).getName();
    case 3:
      return (*board).at(i.row()).getType();
    default:
      return QVariant();
    }
  }
  else if (role == Qt::DecorationRole) {
    if (i.column() == 1)
      return (*board).at(i.row()).getColor();
    else
      return QVariant();
  }
  else if (role == Qt::CheckStateRole) {
    if (i.column() == 0)
      return (*board).at(i.row()).isSelected();
    else
      return QVariant();
  }


  return QVariant();
}

int ListModelBoard::columnCount(const QModelIndex & p) const {
  Q_UNUSED(p);
  return 4;
}

int ListModelBoard::rowCount(const QModelIndex & p) const {
  Q_UNUSED(p);
  return (*board).getNbPieces();
}

bool ListModelBoard::setData(const QModelIndex & i, const QVariant & value, int role) {
  // TODO
}


bool ListModelBoard::update() {
  emit layoutChanged();
}

