#
#   This file is part of Voxigame.
#
#   Copyright (C) 2011 Jean-Marie Favreau <J-Marie.Favreau@u-clermont1.fr>
#                      Université d'Auvergne (France)
#
#   Voxigame is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   Voxigame is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
#

# add the files of the current library
INCLUDE_DIRECTORIES(. core/ frontend/ tests/)
SET(CPP_LIST core/Coord.cxx core/Piece.cxx core/Board.cxx)

ADD_LIBRARY(libvoxigame STATIC ${CPP_LIST})


FIND_PACKAGE(Qt4)

# create the binaries
IF(QT4_FOUND)
  ADD_EXECUTABLE(example tests/example.cpp)
  TARGET_LINK_LIBRARIES(example libvoxigame ${QT_QTCORE_LIBRARY})
ENDIF(QT4_FOUND)

# GUI

IF(QT4_FOUND AND QT_QTOPENGL_FOUND)
  SET(QT_USE_QTXML 1)
  SET(QT_USE_QTOPENGL 1)
  INCLUDE(${QT_USE_FILE})

  INCLUDE_DIRECTORIES(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})
  SET(VOXIGAME_UIS frontend/mainwindow.ui)
  SET(VOXIGAME_SRC frontend/voxigame.cxx frontend/main.cxx)

  QT4_WRAP_UI(UIHeaders ${VOXIGAME_UIS})
  QT4_AUTOMOC(${VOXIGAME_SRC})
  QT4_WRAP_CPP(${VOXIGAME_SRC})
  ADD_EXECUTABLE(voxigame ${VOXIGAME_SRC} ${UIHeaders})
  TARGET_LINK_LIBRARIES(voxigame ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTOPENGL_LIBRARY} qglviewer-qt4)
ENDIF(QT4_FOUND AND QT_QTOPENGL_FOUND)
