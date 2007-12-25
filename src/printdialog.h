/***************************************************************************
 *   Copyright (C) 2006 by Martin Drohmann   *
 *   mdrohmann@gmx.de   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 **************************************************************************/

#ifndef PRINT_DIALOG_H_CLASS
#define PRINT_DIALOG_H_CLASS

#define SPAN_RATIOY 0.5L
#define SPAN_RATIOX 0.4L
#define WINDOW_WIDTH 9917
#define MM_TO_INCH(x) ( (int)(x / 25.4L) )
#define INCH_TO_WIN(x) ( (x * 1200) )
#define MM_TO_WIN(x) ( MM_TO_INCH(INCH_TO_WIN(x)) )
#define INCH_TO_MM(x) ( (x * 25.4L) )
#define WIN_TO_INCH(x) ( (int)(x / 1200) )
#define WIN_TO_MM(x) ( WIN_TO_INCH(INCH_TO_MM(x)) )
#define TOP_MARGIN (MM_TO_WIN(25))
#define BOTTOM_MARGIN (MM_TO_WIN(20))
#define TOP_HEAD_MARGIN (MM_TO_WIN(10))
#define BOTTOM_FOOT_MARGIN (MM_TO_WIN(10))
#define LEFT_MARGIN (MM_TO_WIN(20))
#define RIGHT_MARGIN LEFT_MARGIN
#define HEAD_HEIGHT (MM_TO_WIN(10))
#define FOOT_HEIGHT HEAD_HEIGHT
#define DOUBLE_LINE_SPAN (MM_TO_WIN(1))
#define SALDO_INDEX 7

#define DEBUG

#include <QtGui>
#include <QPrinter>
#include <QPainter>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlQueryModel>

#include "ui_printdialog.h"

class PrintDialog: public QDialog
{
    Q_OBJECT
public:
    enum Type { Konto, Kategorie };
private:
    QString previewFileName;
    QFont theFont;
    enum PrintRect { Foot, Head, Main };
    Type type;
    QString kontoname, kategoriename;
    int saison;

    int lineHeight;

    void clearColumnData();
    void setupPrinter();
    void setupPage(int pageNumber);
    void drawHead();
    void drawFoot(const QString & foot);
    void drawInRect(PrintRect printRect);
    void drawVLines();
    void drawTopLine();
    void drawBottomLine(bool isLastLine = false);
    void createPage(bool preview);
    bool addLine(QVariantList columnItems);
    void drawDoubleLine();
    void drawSaldo();
    QVariant signSaldo(const QVariant & s); 
    int accumulatedWidth(int c);
    void typifyVariant(QVariant & v, int index);
    static bool auszugLessThen(const QVariantList &qvl1, const QVariantList &qvl2);
    static bool belegLessThen(const QVariantList &qvl1, const QVariantList &qvl2);
    QString variantToString(const QVariant &v);
    QSqlQuery * sqlStatement();
    QString saldoSqlStatement();
    QSqlQuery * headSqlStatement();

    void initMetrics();

    QPrinter * thePrinter;
    QPainter * thePainter;
    QFontMetrics * theMetrics;
    int currentY;
    QRect foot, head, main;
    struct ColumnData
    {
        QList<int> width;
        QList<QString> header;
        QPoint span;
        int expandIndex;
        QList<QVariant::Type> type;
    } columnData;
    QVariant saldo;
    Ui::printDialog ui;
public:
    PrintDialog( QSqlQueryModel* kontenModel, QSqlQueryModel* kategorienModel, QWidget* parent=0 );
public slots:
    void setType(Type type) 
    {
        this->type = type;
        if(type == Konto)
        {
            ui.kontenRadio->setChecked(true);
            ui.kategorienRadio->setChecked(false);
        }
        else
        {
            ui.kontenRadio->setChecked(false);
            ui.kategorienRadio->setChecked(true);
        }
    };
    void print() { createPage(false); };
    void preview() { createPage(true); };
    
    void setKonto(bool enabled) { setType( enabled ? Konto : Kategorie ); }
    void setKategorie(bool enabled) { setType( enabled ? Kategorie : Konto ); }
    void setKontoname(const QString & konto) 
    {
        this->kontoname = konto;
        ui.kontenCombo->setCurrentIndex(ui.kontenCombo->findText(konto));
    };
    void setKategoriename(const QString & kategorie) 
    {
        this->kategoriename = kategorie; 
        ui.kategorienCombo->setCurrentIndex(ui.kategorienCombo->findText(kategorie));
    };
    void setSaison(int saison) { this->saison = saison; };
    void setSaison(const QString & saison) { this->saison = saison.toInt(); };
};

#endif /*PRINT_DIALOG_H_CLASS*/
