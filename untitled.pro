QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += icu
LIBS += -licu


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    NgramModel.cpp \
    loaddialog.cpp \
    main.cpp \
    mainwindow.cpp \
    ngram.cpp \
    thread.cpp \
    word_tokenizer.cpp

HEADERS += \
    NGramModel.h \
    loaddialog.h \
    mainwindow.h \
    ngram.h \
    ngram_hasher.h \
    thread.h \
    vector_hasher.h \
    word_tokenizer.h

FORMS += \
    loaddialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    CMakeLists
