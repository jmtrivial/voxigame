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
  )
SET(VOXIGAME_SRCS
  Voxigame.cxx
  main.cxx
  )
SET(VOXIGAME_UIS
  MainWindow.ui
  about.ui
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
