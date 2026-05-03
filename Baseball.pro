QT       += core gui \
    quick
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    admin.cpp \
    adminaddsouvenir.cpp \
    adminchangeprice.cpp \
    admindeletesouvenir.cpp \
    basictripwindow.cpp \
    customtripwindow.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    modcolleges.cpp \
    modsouvenirs.cpp \
    specialalgorithms.cpp \
    summarywindow.cpp \
    tripwindow.cpp

HEADERS += \
    admin.h \
    adminaddsouvenir.h \
    adminchangeprice.h \
    admindeletesouvenir.h \
    basictripwindow.h \
    customtripwindow.h \
    login.h \
    mainwindow.h \
    modcolleges.h \
    modsouvenirs.h \
    specialalgorithms.h \
    summarywindow.h \
    tripwindow.h

FORMS += \
    admin.ui \
    adminaddsouvenir.ui \
    adminchangeprice.ui \
    admindeletesouvenir.ui \
    basictripwindow.ui \
    customtripwindow.ui \
    login.ui \
    mainwindow.ui \
    modcolleges.ui \
    modsouvenirs.ui \
    specialalgorithms.ui \
    summarywindow.ui \
    tripwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    college_tour.sqlite
