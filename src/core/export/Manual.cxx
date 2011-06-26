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
#include <QGraphicsTextItem>

#include "core/export/Manual.hxx"


Manual::Manual(const Board & b) : board(b), substep(false), nbcolumns(2),
				  level(0), maxLevel(10), id(0),
				  author("Unknown"), date(QDate::currentDate()), pageSize(210, 297),
				  innermargin(15.), outermargin(7.), bottommargin(5.), topmargin(5.),
				  columnmargin(5.), footerwidth(20), headererwidth(15.),
				  blackpen(Qt::black, .5){

}

void Manual::addFooter(QGraphicsScene & page, unsigned int nb) const {
  // first draw line
  bool even = nb % 2 == 0;
  QLineF line(even ? outermargin : innermargin, pageSize.height() - footerwidth,
	      pageSize.width() - (even ?  innermargin : outermargin), pageSize.height() - footerwidth);
  page.addLine(line, QPen(Qt::black, 1));

  // text
  QGraphicsTextItem * text = new QGraphicsTextItem;
  QFont font("DejaVu Sans", 4);
  (*text).setFont(font);
  (*text).setPlainText(QString::fromUtf8("Voxigame — ") + QString("#%1 (").arg(id) +
		       (level != 0 ? QString("%1 / %2").arg(level).arg(maxLevel) : QString("- / %2").arg(maxLevel)) + ")");

  QGraphicsTextItem * npage = new QGraphicsTextItem;
  (*npage).setFont(font);
  (*npage).setPlainText(QString("page %1").arg(nb));

  if (even) {
    (*npage).setPos(outermargin, pageSize.height() - footerwidth);
    page.addItem(npage);

    (*text).setPos(pageSize.width() - (*text).boundingRect().width() - innermargin, pageSize.height() - footerwidth);
    page.addItem(text);
  }
  else {
    (*text).setPos(innermargin, pageSize.height() - footerwidth);
    page.addItem(text);

    (*npage).setPos(pageSize.width() - (*npage).boundingRect().width() - outermargin, pageSize.height() - footerwidth);
    page.addItem(npage);
  }
}

QSharedPointer<QGraphicsScene> Manual::createFirstPage() const {
  QSharedPointer<QGraphicsScene> first = QSharedPointer<QGraphicsScene>(new QGraphicsScene(QRectF(0, 0, pageSize.width(), pageSize.height())));

  // draw title
  QFont titleFont("DejaVu Sans", 16, QFont::Bold);
  QGraphicsTextItem * title = new QGraphicsTextItem;
  (*title).setPos(innermargin, headererwidth);
  (*title).setPlainText("Voxigame");
  (*title).setFont(titleFont);

  (*first).addItem(title);
  QRectF btitle = (*title).sceneBoundingRect();

  QLineF linetitle(innermargin, btitle.bottom(),
		   pageSize.width() - outermargin, btitle.bottom());
  (*first).addLine(linetitle, QPen(Qt::black, 5));

  linetitle = QLineF(btitle.right(), btitle.top(),
		     btitle.right(), btitle.bottom());
  (*first).addLine(linetitle, QPen(Qt::black, 1));


  // information about the game
  QGraphicsTextItem * text = new QGraphicsTextItem;
  QFont font("DejaVu Sans", 4);
  (*text).setFont(font);

  // draw level and size
  (*text).setPos(btitle.right(), btitle.top());
  (*text).setPlainText("Puzzle: " + (id != 0 ? QString("#%1").arg(id) : "-")
		       + "\nLevel: " + (level != 0 ? QString("%1 / %2").arg(level).arg(maxLevel) : QString("- / %2").arg(maxLevel))
		       + "\nSize: " + QString::fromUtf8("%1 × %2 × %3").arg(board.getSizeX()).arg(board.getSizeY()).arg(board.getSizeZ()));
  (*first).addItem(text);

  // write author and creation date
  QGraphicsTextItem * text2 = new QGraphicsTextItem;
  (*text2).setFont(font);

  (*text2).setPlainText("Author: " + author +
		       "\nCreation: " + date.toString("d.M.yyyy"));
  (*text2).setPos(innermargin, pageSize.height() - footerwidth - (*text).boundingRect().height());
  (*first).addItem(text2);

  // TODO

  addFooter(*first, 1);
  return first;
}

void Manual::generate() {
  if (!pages.isEmpty())
    pages.clear();

  pages.push_back(createFirstPage());
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

  for(QVector<QSharedPointer<QGraphicsScene> >::iterator page = pages.begin(); page != pages.end(); ++page) {
    if (page != pages.begin())
      printer.newPage();
    (**page).render(&pdfPainter);
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
    // see bug http://bugreports.qt.nokia.com/browse/QTBUG-7091 : cannot generate real A4 pages
    gen.setSize(pageSize);
    gen.setResolution(90);
    gen.setTitle("Voxigame manual");
    QPainter svgPainter;
    if (!svgPainter.begin(&gen))
      return false;
    (**page).render(&svgPainter);
    svgPainter.end();
  }
  return true;
}
