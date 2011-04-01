#include <QtGui/QApplication>
#include "voxigame.h"


int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  Voxigame *window = new Voxigame;

  window->show();
  return app.exec();
}
