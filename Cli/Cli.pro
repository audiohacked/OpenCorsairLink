SOURCES += cli.cpp
INCLUDEPATH += . ../CorsairLinkLib
LIBS += -L../CorsairLinkLib -lCorsairLink -lhidapi-hidraw
TARGET = OpenCorsairLink

