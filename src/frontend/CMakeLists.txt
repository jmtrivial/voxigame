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


FIND_PACKAGE(QGLViewer REQUIRED)
INCLUDE_DIRECTORIES(${QGLVIEWER_INCLUDE_DIR})

SET(VOXIGAME_HDRS
  Voxigame.hxx
  InteractiveBoard.hxx
  ListModelBoard.hxx
  )
SET(VOXIGAME_SRCS
  Voxigame.cxx
  InteractiveBoard.cxx
  PieceProperties.cxx
  main.cxx
  ListModelBoard.cxx
  )
SET(VOXIGAME_UIS
  MainWindow.ui
  about.ui
  )

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

ADD_EXECUTABLE(${VOXIGAME_EXE}
  ${VOXIGAME_SRCS} ${VOXIGAME_UIS} ${VOXIGAME_HDRS}
  )

TARGET_LINK_LIBRARIES(${VOXIGAME_EXE}
  ${VOXIGAME_CORE_LIB} ${QT_LIBRARIES} ${QGLVIEWER_LIBRARY} Qt5::Core Qt5::Xml Qt5::OpenGL

  )

  TARGET_COMPILE_OPTIONS(${VOXIGAME_EXE} PRIVATE -fPIC)
