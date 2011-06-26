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

#include <QApplication>
#include <QStringList>
#include <QTextStream>
#include <QFile>

#include "core/export/Manual.hxx"
#include "core/Board.hxx"

int main(int argc, char** argv)
{

  QTextStream out(stdout);
  QTextStream err(stderr);

  QApplication app(argc, argv);
  QStringList args;

  args = app.arguments();

  if (args.size() <= 1) {
    out << "Parameters required. See help (--help)" << endl;
    return 1;
  }
  if (args.contains("--help") ||
      args.contains("-h")) {
    out << "Usage: vg2pdf [parameters] INPUT OUTPUT" << endl;
    out << endl;
    out << " Parameters:" << endl;
    out << "  -h, --help       Print this help message" << endl;
    return 0;
  }


  QString input;
  QString output;

  // load parameters
  for(unsigned int i = 1; i != (unsigned int) args.size(); ++i) {
    const QString & s = args[i];
    if (s[0] == '-') {
      if ((s == "-h") || (s == "--help"))
	continue;
    }
    else {
      if (input == "")
	input = s;
      else if (output == "")
	output = s;
      else {
	err << "Error: unknown parameter (" << s << ")" << endl;
	err << "Abort." << endl;
	return 1;
      }
    }
  }

  // load file
  QFile ifile(input);
  if (!ifile.exists()) {
    err << "Error: input file not readable (" << input << ")" << endl;
    err << "Abort." << endl;
    return 2;
  }
  out << "Loading file (" << input << ")" << endl;
  Board board;

  if (!board.load(ifile)) {
    err << "Error: cannot load input file (" << input << ")" << endl;
    err << "Abort." << endl;
    return 3;
  }

  Manual manual(board);

  out << "Save file (" << output << ")" << endl;
  return manual.toPDF(output) ? 0 : 4;
}
