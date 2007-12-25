# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./src
# Das Target ist eine Anwendung:  ../bin/garten

HEADERS += dbconnection.h \
           tableeditor.h \
           kleingartenmodel.h \
           defines.h \
	   kleingartendelegate.h \
	   printdialog.h \
	   transaktion.h
# FORMS   += transaktion.ui
FORMS   += printdialog.ui \
           transaktion.ui
SOURCES += garten.cpp \
           tableeditor.cpp \
           kleingartenmodel.cpp \
	   kleingartendelegate.cpp \
	   printdialog.cpp
QT = sql gui
TARGET = ../bin/garten
CONFIG += debug \
warn_on
TEMPLATE = app
