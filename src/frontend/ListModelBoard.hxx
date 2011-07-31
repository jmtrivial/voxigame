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

#ifndef LIST_MODEL_BOARD_H
#define LIST_MODEL_BOARD_H

#include "InteractiveBoard.hxx"
#include <QAbstractListModel>

class Q_GUI_EXPORT ListModelBoard : public QAbstractTableModel {
Q_OBJECT
private:
  const InteractiveBoard * board;
  Q_DISABLE_COPY(ListModelBoard);
public:
  explicit ListModelBoard(QObject * parent = 0);
  ListModelBoard(const InteractiveBoard & b, QObject * p = 0);

  void setBoard(const InteractiveBoard & b);

  const InteractiveBoard & getBoard() const;

  QVariant data(const QModelIndex & index, int role) const;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  int rowCount(const QModelIndex & parent = QModelIndex()) const;

  int columnCount(const QModelIndex & parent = QModelIndex()) const;

  Qt::ItemFlags flags(const QModelIndex & index) const;

  bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

 public slots:
  bool update();
};

#endif // LIST_MODEL_BOARD_H
