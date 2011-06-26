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

#include <QPainter>
#include <QPrinter>
#include <QSvgGenerator>
#include "core/export/Manual.hxx"


Manual::Manual(const Board & b) : board(b), substep(false), nbcolumns(2) {

}

void Manual::createFirstPage() {
  // TODO
}

void Manual::generate() {
  // TODO
}

bool Manual::toPDF(const QString & filename) {
  if (pages.size() == 0)
    generate();

  QPrinter printer;
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setOutputFileName(filename);
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setPaperSize(QPrinter::A4);
  printer.setOrientation(QPrinter::Portrait);
  printer.setFullPage(true);

  QPainter pdfPainter;
  if (!pdfPainter.begin(&printer))
    return false;

  // see http://forum.qtfr.org/viewtopic.php?id=8179
  for(QVector<QSharedPointer<QGraphicsScene> >::iterator page = pages.begin(); page != pages.end(); ++page) {
    (**page).render(&pdfPainter);
    printer.newPage();
  }

  pdfPainter.end();
  return true;
}

bool Manual::toSVG(const QString & prefix, const QString & suffix) {
  if (pages.size() == 0)
    generate();

  short int i = 0;
  for(QVector<QSharedPointer<QGraphicsScene> >::iterator page = pages.begin(); page != pages.end(); ++page, ++i) {
    QSvgGenerator gen;
    QString filename = prefix + QString("%1").arg(i, 4, 10, QChar('0'))+ suffix;
    gen.setFileName(filename);
    // see http://bugreports.qt.nokia.com/browse/QTBUG-7091
    gen.setSize(QSize(595, 841));
    gen.setViewBox(QRect(0, 0, 595, 841));
    gen.setResolution(72);
    gen.setTitle("Voxigame manual");
    QPainter svgPainter;
    if (!svgPainter.begin(&gen))
      return false;
    (**page).render(&svgPainter);
    svgPainter.end();
  }
  return true;
}
