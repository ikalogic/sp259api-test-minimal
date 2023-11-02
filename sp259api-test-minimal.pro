TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp


win32 {
    DEFINES += OS_WIN
    DEFINES += __WINDOWS_TYPES__
    win32-msvc* {
        LIBS += -L$$PWD/dependencies/win/ -lsp259_api
    }
    equals(QMAKE_CXX, g++) {
        LIBS += $$PWD/dependencies/win/sp259_api.dll
    }

}

macx: LIBS += -L$$PWD/dependencies/mac/ -lsp259_api.1.0.0
unix: LIBS += -L$$PWD/dependencies/linux/ -lsp259_api

HEADERS += \
    ihwapi_common_types.h \
    sp259api.h \
    sp259api_types.h
