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


#ifndef VOXIGAME_H
#define VOXIGAME_H

#include <QMainWindow>
#include "ui_MainWindow.h"
#include "core/Board.hxx"

class Voxigame : public QMainWindow
{
  Q_OBJECT
public:
  Voxigame();

  virtual ~Voxigame();

  void changeEvent(QEvent *e);

private:
  /** main interface */
  Ui::MainWindow ui;

  /** the manipulated board */
  Board board;

  /** filename of the currently modified board */
  QString filename;

  /** true if the board as been modified since the creation */
  bool modified;

  bool saveFile(const QString & file);

  Board getBoardFromSettings();

private slots:
  /** display the about message */
  void aboutMessage();

  /** load a board */
  void loadBoard();

  /** save the current board */
  bool saveBoard();

  /** save the current board in a new file */
  bool saveAsBoard();

  /** quit the application */
  void quit();

  /** close the current manipulated board */
  void closeBoard();
};

#endif
