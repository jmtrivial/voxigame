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
SET(CORE_CPP_LIST core/Coord.cxx core/Piece.cxx core/Board.cxx core/Pattern.cxx)

ADD_LIBRARY(libvoxigame STATIC ${CORE_CPP_LIST})


FIND_PACKAGE(Qt4 REQUIRED)


# GUI
IF(QT_QTOPENGL_FOUND)
  SET(QT_USE_QTOPENGL 1)
  SET(QT_USE_QTXML 1)
  INCLUDE(${QT_USE_FILE})

  INCLUDE_DIRECTORIES(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})
  SET(VOXIGAME_UIS frontend/mainwindow.ui frontend/about.ui)
  SET(VOXIGAME_SRC frontend/voxigame.cxx frontend/main.cxx)
  SET(VOXIGAME_MOC_HDRS frontend/voxigame.hxx)

  FIND_PACKAGE(QGLViewer REQUIRED)
  INCLUDE_DIRECTORIES(${QGLVIEWER_INCLUDE_DIR})

  QT4_WRAP_UI(UIHeaders ${VOXIGAME_UIS})
  QT4_WRAP_CPP(VOXIGAME_MOC_SRCS ${VOXIGAME_MOC_HDRS})
  ADD_EXECUTABLE(voxigame ${VOXIGAME_SRC} ${UIHeaders} ${VOXIGAME_MOC_SRCS})
  TARGET_LINK_LIBRARIES(voxigame libvoxigame ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTOPENGL_LIBRARY} ${QT_QTXML_LIBRARY} ${QGLVIEWER_LIBRARY})
ENDIF(QT_QTOPENGL_FOUND)


# testing
ENABLE_TESTING()
SET(QT_USE_QTTEST true)
SET(QT_USE_QTXML true)
SET(TEST_LIBRARIES ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES})
INCLUDE(${QT_USE_FILE})

SET(TEST_MOC_TBOARD_HDRS tests/testBoard.hxx)
SET(TEST_MOC_TPIECE_HDRS tests/testPiece.hxx)
SET(TEST_MOC_TCOORD_HDRS tests/testCoord.hxx)

QT4_WRAP_CPP(TEST_MOC_TBOARD_SRCS ${TEST_MOC_TBOARD_HDRS})
QT4_WRAP_CPP(TEST_MOC_TPIECE_SRCS ${TEST_MOC_TPIECE_HDRS})
QT4_WRAP_CPP(TEST_MOC_TCOORD_SRCS ${TEST_MOC_TCOORD_HDRS})

ADD_EXECUTABLE(testCoord tests/testCoord.cxx ${TEST_MOC_TCOORD_SRCS})
TARGET_LINK_LIBRARIES(testCoord libvoxigame ${QT_LIBRARIES} ${QT_QTTEST_LIBRARY})
ADD_TEST(testCoord testCoord)

ADD_EXECUTABLE(testPiece tests/testPiece.cxx ${TEST_MOC_TPIECE_SRCS})
TARGET_LINK_LIBRARIES(testPiece libvoxigame ${QT_LIBRARIES} ${QT_QTTEST_LIBRARY})
ADD_TEST(testPiece testPiece)

ADD_EXECUTABLE(testBoard tests/testBoard.cxx ${TEST_MOC_TBOARD_SRCS})
TARGET_LINK_LIBRARIES(testBoard libvoxigame ${QT_LIBRARIES} ${QT_QTTEST_LIBRARY})
ADD_TEST(testBoard testBoard)

