# set project's name
  PROJECT( think )
  
  cmake_minimum_required(VERSION 2.8)
  
  # with SET() command you can change variables or define new ones
  # here we define THINK_SRCS variable that contains a list of all .cpp files
  # note that we don't need \ at the end of line
  SET( THINK_SRCS
    main.cpp
    mainwindow.cpp
    categorywindow.cpp
    notewindow.cpp
    preferencewindow.cpp
  )
  
  # another list, this time it includes all header files that should be treated with moc
  SET( THINK_MOC_HDRS
    mainwindow.h
    categorywindow.h
    notewindow.h
    preferencewindow.h
  )
  
    SET( THINK_RCS
        rc.qrc
  )
    SET( THINK_UIS
          mainwindow.ui
          categorywindow.ui
          notewindow.ui
          preferencewindow.ui
  )
  
  set(EXECUTABLE_OUTPUT_PATH ${THINK_SOURCE_DIR})
  
  # and finally an resource file
  
  # enable warnings
  ADD_DEFINITIONS( -Wall )
  
  # by default only QtCore and QtGui modules are enabled
  # other modules must be enabled like this:
  
  # this command finds Qt4 libraries and sets all required variables
  # note that it's Qt4, not QT4 or qt4
  FIND_PACKAGE( Qt4 REQUIRED )
  
  # add some useful macros and variables
  # (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
  INCLUDE( ${QT_USE_FILE} )
  
  # this command will generate rules that will run rcc on all files from THINK_RCS
  # in result THINK_RC_SRCS variable will contain paths to files produced by rcc
  QT4_ADD_RESOURCES( THINK_RC_SRCS ${THINK_RCS} )
  
  # this will run uic on .ui files:
  QT4_WRAP_UI( THINK_UI_HDRS ${THINK_UIS} )
  
  # and finally this will run moc:
  QT4_WRAP_CPP( THINK_MOC_SRCS ${THINK_MOC_HDRS} )
  
  # we need this to be able to include headers produced by uic in our code
  # (CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES() works just like INCLUDEPATH from qmake)
  INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )
  
  # here we instruct CMake to build "THINK" executable from all of the source files
  ADD_EXECUTABLE( THINK ${THINK_SRCS} ${THINK_MOC_SRCS} ${THINK_RC_SRCS} ${THINK_UI_HDRS} )
  
  # last thing we have to do is to tell CMake what libraries our executable needs,
  # luckily FIND_PACKAGE prepared QT_LIBRARIES variable for us:
  TARGET_LINK_LIBRARIES( THINK ${QT_LIBRARIES})
  
  install(
    TARGETS
        THINK
    RUNTIME DESTINATION
        ${CMAKE_INSTALL_PREFIX}/bin
)

