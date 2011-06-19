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

OPTION(BUILD_WITH_FRONTEND "Build the Qt front-end for Voxigame" ON)
OPTION(BUILD_WITH_TESTS    "Build the unit tests"                ON)


FIND_PACKAGE(Qt4 REQUIRED)

SET(VOXIGAME_CORE_LIB VoxigameCore)
SET(VOXIGAME_EXE voxigame)

# add the files of the current library
SET(VOXIGAME_CORE_SRCS
  core/Coord.cxx
  core/Box.cxx
  core/Board.cxx
  core/Piece.cxx
  core/StraightPiece.cxx
  core/LPiece.cxx
  core/GenericPiece.cxx
  core/Pattern.cxx
  core/PieceFactory.cxx
  core/Face.cxx
)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
ADD_LIBRARY(${VOXIGAME_CORE_LIB} ${VOXIGAME_CORE_SRCS})


# GUI
IF(BUILD_WITH_FRONTEND)
  IF(QT_QTOPENGL_FOUND)
    SET(QT_USE_QTOPENGL TRUE)
    SET(QT_USE_QTXML    TRUE)
    INCLUDE(${QT_USE_FILE})
    ADD_DEFINITIONS(${QT_DEFINITIONS})
    INCLUDE_DIRECTORIES(${QT_INCLUDES})

    FIND_PACKAGE(QGLViewer REQUIRED)
    INCLUDE_DIRECTORIES(${QGLVIEWER_INCLUDE_DIR})

    SET(VOXIGAME_HDRS
      frontend/Voxigame.hxx
    )
    SET(VOXIGAME_SRCS
      frontend/Voxigame.cxx
      frontend/main.cxx
    )
    SET(VOXIGAME_UIS
      frontend/MainWindow.ui
      frontend/about.ui
    )

    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
    QT4_WRAP_UI(VOXIGAME_UIS_H ${VOXIGAME_UIS})
    QT4_WRAP_CPP(VOXIGAME_MOC_SRCS ${VOXIGAME_HDRS})

    ADD_EXECUTABLE(${VOXIGAME_EXE}
      ${VOXIGAME_SRCS} ${VOXIGAME_UIS_H} ${VOXIGAME_MOC_SRCS}
    )
    TARGET_LINK_LIBRARIES(${VOXIGAME_EXE}
      ${VOXIGAME_CORE_LIB} ${QT_LIBRARIES} ${QGLVIEWER_LIBRARY}
    )
  ENDIF(QT_QTOPENGL_FOUND)
ENDIF(BUILD_WITH_FRONTEND)


# Testing
IF(BUILD_WITH_TESTS)
  ENABLE_TESTING()
  SET(QT_USE_QTTEST TRUE)
  SET(QT_USE_QTXML TRUE)
  INCLUDE(${QT_USE_FILE})

  INCLUDE_DIRECTORIES(tests)
  ADD_SUBDIRECTORY(tests)
ENDIF(BUILD_WITH_TESTS)
