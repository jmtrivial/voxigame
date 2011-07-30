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

#include <QFileDialog>
#include <QMessageBox>
#include "Voxigame.hxx"
#include "ui_about.h"


// See: http://doc.qt.nokia.com/4.7-snapshot/mainwindows-recentfiles-mainwindow-cpp.html
Voxigame::Voxigame() : board(getBoardFromSettings()), filename(), modified(false) {
  ui.setupUi(this);
  connect(ui.actionAbout_Voxigame, SIGNAL(triggered()), SLOT(aboutMessage()));
  connect(ui.actionOpen, SIGNAL(triggered()), SLOT(loadBoard()));
  connect(ui.actionSave, SIGNAL(triggered()), SLOT(saveBoard()));
  connect(ui.actionSave_as, SIGNAL(triggered()), SLOT(saveAsBoard()));
  connect(ui.actionQuit, SIGNAL(triggered()), SLOT(quit()));
  connect(ui.actionClose, SIGNAL(triggered()), SLOT(closeBoard()));
}

Voxigame::~Voxigame() {

}


void Voxigame::changeEvent(QEvent *e) {
  QWidget::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    ui.retranslateUi(this);
    break;
  default:
    break;
  }
 }

void Voxigame::aboutMessage() {
  QDialog dialog;
  Ui::About about;

  about.setupUi(&dialog);
  dialog.exec();

}


void Voxigame::loadBoard() {
  if (modified) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "The current board has been modified. All the modification will be lost by opening a new one. Open anyway?", "",
				  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No)
      return;
  }

  QString fname = QFileDialog::getOpenFileName(this, "Load board", "", "Voxigame files (*.xml)");

  if (!fname.isEmpty()) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if(board.load(fname)) {
      filename = fname;
      modified = false;
      QApplication::restoreOverrideCursor();
      statusBar()->showMessage("File loaded", 2000);
    }
    else {
      QApplication::restoreOverrideCursor();
      QMessageBox::warning(this, "Recent Files", QString("Cannot load file %1").arg(fname));
      statusBar()->showMessage("Abort loading", 2000);
    }
  }
}

bool Voxigame::saveBoard() {
  if (filename.isEmpty())
    return saveAsBoard();
  else {
    return saveFile(filename);
  }
}

bool Voxigame::saveAsBoard() {
  QString fname = QFileDialog::getOpenFileName(this, "Save board", "", "Voxigame files (*.xml)");
  if (fname.isEmpty())
    return false;

  return saveFile(fname);
}

void Voxigame::quit() {
  if (modified) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "The board has been modified. Save before exit?", "",
				  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (reply == QMessageBox::Yes) {
      saveBoard();
      close();
    }
    else if (reply == QMessageBox::No)
      close();
  }
  else
    close();
}

void Voxigame::closeBoard() {
  if (modified) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "The board has been modified. Close anyway?", "",
				  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No)
      return;
  }

  board.setBoard(getBoardFromSettings());
}

bool Voxigame::saveFile(const QString & fileName) {

  QApplication::setOverrideCursor(Qt::WaitCursor);
  if(board.save(fileName)) {
    filename = fileName;
    modified = false;
    QApplication::restoreOverrideCursor();
    statusBar()->showMessage("File saved", 2000);
    return true;
  }
  else {
    QApplication::restoreOverrideCursor();
    QMessageBox::warning(this, "Recent Files", QString("Cannot write file %1").arg(fileName));
    statusBar()->showMessage("Abort saving", 2000);
    return false;
  }

}

Board Voxigame::getBoardFromSettings() {
  Board b;
  // TODO

  return b;
}
