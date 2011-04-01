#ifndef VOXIGAME_H
#define VOXIGAME_H

#include <QMainWindow>
#include "ui_mainwindow.h"

class Voxigame : public QMainWindow
{
  Q_OBJECT
public:
  Voxigame();

  virtual ~Voxigame();
private:
  Ui::MainWindow ui;
};

#endif
