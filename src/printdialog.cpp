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

#include "printdialog.h"

void PrintDialog::setupPrinter()
{
    thePrinter = new QPrinter(QPrinter::HighResolution);
    thePrinter->setPageSize(QPrinter::A4);
    thePrinter->setDocName("Kleingarten");
    thePrinter->setCreator("Kleingarten");
    thePrinter->setOutputFormat(QPrinter::PdfFormat);
    thePrinter->setFullPage(true);
    thePrinter->setColorMode(QPrinter::Color);
    thePrinter->setOutputFileName(previewFileName);
    /*         thePrinter->setOutputToFile(true); */
    if(ui.printSelection_allRadio->isChecked())
        thePrinter->setFromTo(0, 0);
    else
    {
        thePrinter->setFromTo(ui.fromSpinBox->value(), ui.toSpinBox->value());
        qDebug() << "from" << ui.fromSpinBox->value() << "to" << ui.toSpinBox->value();
    }
}

PrintDialog::PrintDialog( QSqlQueryModel* kontenModel, QSqlQueryModel* kategorienModel, QWidget * parent /* = 0 */ ):
    QDialog(parent), theFont("Times New Roman", 10),
    currentY(0), foot(), head(), main()
{
    ui.setupUi(this);
    ui.kontenCombo->setModel(kontenModel);
    setKontoname(tr("Kasse"));
    ui.kategorienCombo->setModel(kategorienModel);
    qDebug() <<ui.kategorienCombo->currentText();
    setKategoriename(ui.kategorienCombo->currentText());
    /*     ui.comboBox->setModel(); */
    previewFileName = QDir::homePath().append(
            QDir::separator()).append(
            ".kleingarten").append(
                QDir::separator()).append(
                "preview.pdf");

    connect(ui.printButton, SIGNAL(clicked()), this, SLOT(print()));
    connect(ui.previewButton, SIGNAL(clicked()), this, SLOT(preview()));
    connect(ui.kontenCombo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setKontoname(const QString &)));
    connect(ui.kategorienCombo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setKategoriename(const QString &)));
    connect(ui.kontenRadio, SIGNAL(toggled(bool)), this, SLOT(setKonto(bool)));
}

void PrintDialog::drawTopLine()
{
    QFont oldFont = thePainter->font();
    QFont headerFont(oldFont);
    headerFont.setBold(true);
    headerFont.setItalic(true);
    thePainter->setFont(headerFont);
    QRect rect;
    int cx=0;
    for(int i = 0; i< columnData.width.count(); ++i)
    {
        thePainter->drawText(cx + columnData.span.x(), 0,
                columnData.width.value(i), lineHeight,
                Qt::AlignVCenter | Qt::AlignLeft, columnData.header.value(i),
                &rect);
        cx += columnData.width.value(i);
    }
    currentY += rect.height() + 2 * columnData.span.y();
    thePainter->setFont(oldFont);
    drawDoubleLine();
    cx = accumulatedWidth(columnData.expandIndex);
    if (type == Konto || saldo.toDouble() != 0.0)
    {
        thePainter->drawText(cx + columnData.span.x(), currentY + columnData.span.y(),
                columnData.width.value(columnData.expandIndex) - 2 * columnData.span.x(),
                lineHeight,
                Qt::AlignTop| Qt::AlignRight, tr("Übertrag"),
                &rect);
        drawSaldo();
        currentY += rect.height() + 2 * columnData.span.y();
    }
}

void PrintDialog::drawBottomLine(bool isLastLine)
{
    drawDoubleLine();
    int cx = accumulatedWidth(columnData.expandIndex);
    QRect rect;
    QString text;
    if (isLastLine)
        text = tr("Gesamt");
    else
        text = tr("Übertrag");
    thePainter->drawText(cx + columnData.span.x(), currentY + columnData.span.y(),
            columnData.width.value(columnData.expandIndex) - 2 * columnData.span.x(),
            lineHeight,
            Qt::AlignTop| Qt::AlignRight, text,
            &rect);
    drawSaldo();
    currentY += rect.height() + 2 * columnData.span.y();
}

void PrintDialog::drawDoubleLine()
{
    thePainter->drawLine(0, currentY, main.width(), currentY);
    currentY += DOUBLE_LINE_SPAN;
    thePainter->drawLine(0, currentY, main.width(), currentY);
}

/* signSaldo dreht in Konten das Vorzeichen um. Dies ist nämlich eine für
 * Ausdrücke intuitivere Darstellung */
QVariant PrintDialog::signSaldo(const QVariant & s)
{
    if (type==Konto)
    {
        return QVariant( s.toDouble() * -1 );
    }
    else
    {
        return QVariant( qAbs(s.toDouble()) );
    }
}

void PrintDialog::drawSaldo()
{
    int offsetX;
    if (type == Konto)
    {
        offsetX = accumulatedWidth(SALDO_INDEX);
        thePainter->drawText(offsetX + columnData.span.x(), currentY + columnData.span.y(),
                columnData.width.value(SALDO_INDEX) - 2 * columnData.span.x(),
                lineHeight,
                Qt::AlignTop| Qt::AlignRight, 
                variantToString( signSaldo(saldo) ));
    }
    else 
    {
        int cd_index = SALDO_INDEX - (saldo.toDouble() < 0.0 ?
                                                            1 : 2);
        offsetX = accumulatedWidth(cd_index);
        thePainter->drawText(offsetX + columnData.span.x(), currentY + columnData.span.y(),
                columnData.width.value(cd_index) - 2 * columnData.span.x(),
                lineHeight,
                Qt::AlignTop| Qt::AlignRight, 
                variantToString( signSaldo(saldo) ));
    }
    /*     offsetX += columnData.width.value(ci);
           ++ci;
           thePainter->drawText(offsetX + columnData.span.x(), currentY + columnData.span.y(),
           columnData.width.value(ci) - 2 * columnData.span.x(),
           lineHeight,
           Qt::AlignTop| Qt::AlignRight, variantToString(saldo.soll));                   */
}

void PrintDialog::drawHead()
{
    drawInRect(Head);

    QFont oldFont = thePainter->font();
    QFont headFont(oldFont);
    headFont.setPointSize(12);
    thePainter->setFont(headFont);
    QSqlQuery * query = headSqlStatement();
    query->first();
    QString h(query->value(0).toString());
    QString temp = query->value(1).toString();
    if(type == Konto && !temp.isEmpty())
        h.append(" (").append(temp).append(QChar(')'));
    bool hide_season = query->value(type == Konto ? 2 : 1).toBool();
    if( !hide_season ) {
        h.append(QChar(' ')).append(QString::number(saison));
    }
    thePainter->drawText(0, 0, head.width(), head.height(),
            Qt::AlignCenter, h);
    thePainter->setFont(oldFont);
    delete query;

}

void PrintDialog::drawFoot(const QString & f)
{
    drawInRect(Foot);

    /*     QFont oldFont = thePainter->font();
           QFont headFont(oldFont);
           headFont.setPointSize(12);
           thePainter->setFont(headFont);         */
    thePainter->drawText(0, 0, foot.width(), foot.height(), Qt::AlignCenter, f);
    /*     thePainter->setFont(oldFont);    */

}

int PrintDialog::accumulatedWidth(int c)
{
    int x = 0;
    for (int i=0; i<c; ++i)
        x += columnData.width.value(i);
    return x;
}

bool PrintDialog::belegLessThen(const QVariantList &qvl1, const QVariantList &qvl2)
{
    QString s1 = qvl1.value(2).toString();
    QString s2 = qvl2.value(2).toString();
    QRegExp rx("([0-9]+)(\\D*)");
    rx.indexIn(s1);
    QString s11 = rx.cap(1);
    QString s12 = rx.cap(2);
    rx.indexIn(s2);
    QString s21 = rx.cap(1);
    QString s22 = rx.cap(2);
    if(s11.toInt() == s21.toInt())
        return s12 < s22;
    else
        return s11.toInt() < s21.toInt();
}

bool PrintDialog::auszugLessThen(const QVariantList &qvl1, const QVariantList &qvl2)
{
    QString s1 = qvl1.value(3).toString();
    QString s2 = qvl2.value(3).toString();
    QRegExp rx("(\\d+)[-\\.\\/]*(\\d*)");
    rx.indexIn(s1);
    QString s11 = rx.cap(1);
    QString s12 = rx.cap(2);
    rx.indexIn(s2);
    QString s21 = rx.cap(1);
    QString s22 = rx.cap(2);
    bool ret;
    if(s11.toInt() == s21.toInt())
        ret = s12.toInt() < s22.toInt();
    else
        ret = s11.toInt() < s21.toInt();
    return ret;
}

bool PrintDialog::addLine(QVariantList columnItems)
{
    int spaceLeftY = main.height() - currentY - lineHeight;

    int expandColumnX = accumulatedWidth(columnData.expandIndex);

    QRect initRect(expandColumnX + columnData.span.x(), currentY + columnData.span.y(),
            columnData.width.value(columnData.expandIndex) - 2 * columnData.span.x(), 1000);
    int flags = Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap;
    QRect rect;
    rect = theMetrics->boundingRect(initRect, flags,
            variantToString(columnItems.value(columnData.expandIndex)));

    if (rect.height() > spaceLeftY)
        return false;

    thePainter->drawLine(0, currentY, main.width(), currentY);
    currentY += columnData.span.y();

    thePainter->drawText(initRect, flags,
            variantToString(columnItems.value(columnData.expandIndex)), &rect);
    flags = Qt::AlignVCenter | Qt::AlignRight;

    int currentX = 0;
    rect.setY(currentY);
    QPen oldPen = thePainter->pen();
    for(int i=0; i<columnItems.count(); ++i)
    {
        rect.setX(currentX + columnData.span.x());
        if (i != columnData.expandIndex)
        {
            rect.setWidth(columnData.width.value(i) - 2 * columnData.span.x());
            if(columnItems.value(i).type() == QVariant::Double 
                    && columnItems.value(i).toDouble() < 0) 
            {
                QPen oldPen = thePainter->pen();
                QPen pen(oldPen);
                pen.setColor(QColor(Qt::darkRed));
                thePainter->setPen(pen); 
            }
            thePainter->drawText(rect, flags, variantToString(columnItems.value(i)));
            thePainter->setPen(oldPen);
        }
        currentX += columnData.width.value(i); 
    }
    currentY += rect.height() + columnData.span.y();
    /*     qDebug() << rect.width() << rect.height() << "vs. " << columnData.width.value(columnData.expandIndex); */
    /*         qDebug() << rect.width() << rect.height() << "vs. " << rect2.width() << rect2.height() << "vs. " << rect2.width() << rect2.height() << columnData.width.value(columnData.expandIndex); */
    return true;
}

void PrintDialog::clearColumnData()
{
    columnData.width.clear();
    columnData.header.clear();
    columnData.type.clear();
}

void PrintDialog::createPage(bool preview)
{
    setupPrinter();
    currentY = 0;
    clearColumnData();
    if (preview)
        thePrinter->setOutputFileName(previewFileName);
    else
        thePrinter->setOutputFileName(QString());
    int pageCount = 1;
#ifdef DEBUG
    qDebug() << saldoSqlStatement();
#endif
    QSqlQuery qu(saldoSqlStatement());
    qu.first();
    saldo = ( type == Konto ?
            QVariant(qu.value(0).toDouble() - qu.value(1).toDouble()) :
            0 );
#ifdef DEBUG
    qDebug() << saldo.toString();
#endif
    typifyVariant(saldo, SALDO_INDEX);
    QSqlQuery * query = sqlStatement();

    initMetrics();

    thePainter = new QPainter(thePrinter);
    thePainter->setFont(theFont);

    thePainter->setRenderHint(QPainter::Antialiasing, true);

    qDebug() << thePainter->fontInfo().family() << thePainter->fontInfo().pointSize();
    QRect windowRect = thePainter->window();

    int cc = query->record().count();
    qDebug() << cc;

    QList<QVariantList> lines;
    while (query->next())
    {
        QVariantList line;
        for (int i=0; i< cc; ++i)
        {
            QVariant v = query->value(i);
            qDebug() << i << ": " << v << ", ";
            typifyVariant(v, i);
            line.append(v);
        }
        qDebug() << endl;
        lines.append(line);
    }

    /* sort algorithms */

    if (type == Konto && (! kontoname.contains("Sparbuch")) ) {
        if (kontoname == tr("Kasse"))
            qStableSort(lines.begin(), lines.end(), belegLessThen);
        else 
            qStableSort(lines.begin(), lines.end(), auszugLessThen);
    }

    /* draw the pages */

    setupPage(pageCount);
    drawTopLine();
    QList<QVariantList>::iterator it;
    QString lastString;
    for (it = lines.begin(); it != lines.end(); ++it)
    {
        /*             qDebug() << ls.at(j); */
        QVariantList line = *it;
        double oldSaldo = saldo.toDouble();
        saldo = saldo.toDouble() + line.value(5).toDouble() - line.value(6).toDouble();
        typifyVariant(saldo, SALDO_INDEX);
        line.append( signSaldo(saldo) );
        if(! addLine(line))
        {
            double tempSaldo = saldo.toDouble();
            saldo = oldSaldo;
            drawBottomLine();
            drawVLines();
            thePrinter->newPage();
            setupPage(++pageCount);
            currentY = 0;
            drawTopLine();
            saldo = tempSaldo;
            addLine(line);
        }
    }

    delete query;

    drawBottomLine(true);
    drawVLines();

    delete thePainter;
    delete theMetrics;
    delete thePrinter;
    QStringList args;
    if (preview)
    {
        args << previewFileName;
        QProcess::startDetached("kpdf", args);
    }
}

void PrintDialog::setupPage(int pageNumber)
{
    drawHead();
    drawFoot(QString::number(pageNumber).prepend(tr("Seite ")));
    drawInRect(Main);
}

void PrintDialog::drawVLines()
{
    int cx=0;
    for (int i = 0; i <= columnData.width.count(); ++i)
    {
        thePainter->drawLine(cx, 0, cx, currentY);
        cx += columnData.width.value(i);
    }
    thePainter->drawLine(cx, 0, cx, currentY);
}

void PrintDialog::drawInRect(PrintRect printRect)
{
    int h = thePrinter->height();
    int w = thePrinter->width();
    switch(printRect)
    {
        case Head:
            thePainter->setWindow( -head.x(), -head.y(), w, h );
            break;
        case Foot:
            thePainter->setWindow( -foot.x(), -foot.y(), w, h );
            break;
        case Main:
            thePainter->setWindow( -main.x(), -main.y(), w, h );
            break;
        default:
            return; 
    }
}

void PrintDialog::typifyVariant(QVariant & v, int index)
{
    switch(columnData.type.value(index))
    {
        case QVariant::Double:
            v = QVariant(v.toDouble());
            break;
        case QVariant::Date:
            v = QVariant(QDate::fromString(v.toString(), Qt::ISODate));
            break;
        default:
            return;
    }
}

QString PrintDialog::variantToString(const QVariant &v)
{
    QString s;
    double d;
    switch(v.type())
    {
        case QVariant::Double:
            d = v.toDouble();
            if (d == 0)
                s = QString(" ");
            else
                s = QLocale::system().toString(d, 'f', 2);
            break;
        case QVariant::Date:
            s = v.toDate().toString("dd.MM.yy");
            break;
        default:
            s = v.toString();
    }
    return s;
}

void PrintDialog::initMetrics()
{
    /*     qDebug() << thePainter->fontInfo().family() << thePainter->fontInfo().pointSize(); */
    qDebug() << theFont.family() << theFont.pointSize();
    int height = thePrinter->height();
    int width = thePrinter->width();
    main = QRect(LEFT_MARGIN, TOP_MARGIN, width - LEFT_MARGIN - RIGHT_MARGIN, height - TOP_MARGIN - BOTTOM_MARGIN);
    head = QRect(LEFT_MARGIN, TOP_HEAD_MARGIN, width - LEFT_MARGIN - RIGHT_MARGIN, HEAD_HEIGHT);
    foot = QRect(LEFT_MARGIN, height - BOTTOM_FOOT_MARGIN - FOOT_HEIGHT, width - LEFT_MARGIN - RIGHT_MARGIN, HEAD_HEIGHT);

    /*     theMetrics = thePainter->fontMetrics(); */
    theMetrics = new QFontMetrics(theFont, thePrinter);

    QRect mRect = theMetrics->boundingRect(QChar('M'));
    columnData.span.setX((int)(mRect.width() * SPAN_RATIOX));
    columnData.span.setY((int)(mRect.height() * SPAN_RATIOY));
    columnData.width.append( qMax(theMetrics->width("00.00.00"), theMetrics->width("B-Datum")) + 2 * columnData.span.x() );
    columnData.header.append( "B-Datum" );
    columnData.type.append(QVariant::Date);
    columnData.width.append( qMax(theMetrics->width("00.00.00"), theMetrics->width("L-Datum")) + 2 * columnData.span.x() );
    columnData.header.append( "L-Datum" );
    columnData.type.append(QVariant::Date);
    columnData.width.append( qMax(theMetrics->width("999a"), theMetrics->width("Beleg")) + 2 * columnData.span.x() );
    columnData.header.append( "Beleg" );
    columnData.type.append(QVariant::String);
    columnData.width.append( qMax(theMetrics->width("999.9"), theMetrics->width("Auszug")) + 2 * columnData.span.x() );
    columnData.header.append( "Auszug" );
    columnData.type.append(QVariant::String);
    columnData.width.append( 0 );
    columnData.header.append( "Beschreibung" );
    columnData.type.append(QVariant::String);
    int soll_haben_width = theMetrics->width(type == Konto ? "9.999,99": "99.999,99");
    columnData.width.append( qMax(soll_haben_width, theMetrics->width("Haben"))
            + 2 * columnData.span.x() );
    columnData.header.append( "Haben" );
    columnData.type.append(QVariant::Double);
    columnData.width.append( qMax(soll_haben_width, theMetrics->width("Soll"))
            + 2 * columnData.span.x() );
    columnData.header.append( "Soll" );
    columnData.type.append(QVariant::Double);
    columnData.expandIndex = columnData.width.indexOf(0);
    if (type == Konto)
    {
        columnData.header.append( "Saldo" );
        columnData.type.append(QVariant::Double);
        columnData.width.append( qMax(theMetrics->width("99.999,99"),
                    theMetrics->width("Saldo")) + 2 * columnData.span.x() );
    }
    lineHeight = theMetrics->height() + 2 * columnData.span.y();

    int expandColumnWidth = main.width();
    for(int i=0; i<columnData.width.count(); ++i)
        expandColumnWidth -= columnData.width.value(i);
    columnData.width.replace(columnData.expandIndex, expandColumnWidth);

    /*     for (int i=0; i<columnData.width.count(); ++i)
           qDebug() << columnData.width.value(i) << columnData.span.x();
           qDebug() << QFontDatabase::standardSizes();                          */
}

QSqlQuery * PrintDialog::headSqlStatement()
{
    QSqlDriver * driver = QSqlDatabase::database().driver();
    bool prepared = driver->hasFeature(QSqlDriver::PreparedQueries);
    QSqlRecord sRec;
    QSqlRecord wRec;
    QString s;
    if (type == Konto)
    {
        sRec.append(QSqlField(tr("kopfzeile")));
        sRec.append(QSqlField(tr("ktn")));
        sRec.append(QSqlField(tr("season_hide")));
        wRec.append(QSqlField(tr("kontoname")));
        wRec.setValue(0, kontoname);
        s = driver->sqlStatement(QSqlDriver::SelectStatement, "konten", sRec, prepared);
        s.append(QChar(' ')).append(driver->sqlStatement(QSqlDriver::WhereStatement, "konten", wRec, prepared));
    }
    else
    {
        sRec.append(QSqlField(tr("kopfzeile")));
        sRec.append(QSqlField(tr("season_hide")));
        wRec.append(QSqlField(tr("kategoriename")));
        wRec.setValue(0, kategoriename);
        s = driver->sqlStatement(QSqlDriver::SelectStatement, "kategorien", sRec, false);
        s.append(QChar(' ')).append(driver->sqlStatement(QSqlDriver::WhereStatement, "kategorien", wRec, prepared));
    }
    QSqlQuery * query;
    if(prepared)
    {
        query = new QSqlQuery();
        if(query->lastQuery() != s)
            query->prepare(s);

        query->addBindValue(wRec.value(0));
        query->exec();
    }
    else
    {
        query = new QSqlQuery(s);
    }
    return query;
}

QSqlQuery * PrintDialog::sqlStatement()
{
    QSqlDriver * driver = QSqlDatabase::database().driver();
    bool prepared = driver->hasFeature(QSqlDriver::PreparedQueries);
    QSqlRecord sRec;
    QSqlRecord wRec;
    sRec.append(QSqlField(tr("datum")));
    sRec.append(QSqlField(tr("rdatum")));
    sRec.append(QSqlField(tr("beleg")));
    sRec.append(QSqlField(tr("auszug")));
    sRec.append(QSqlField(tr("beschreibung")));
    if (type == Konto)
    {
        sRec.append(QSqlField(tr("soll")));
        sRec.append(QSqlField(tr("haben")));
    }
    else
    {
        sRec.append(QSqlField(tr("soll")));
        sRec.append(QSqlField(tr("haben")));
    }
    QString s;
    s = driver->sqlStatement(QSqlDriver::SelectStatement, "buchungen", sRec, prepared);
    if (type == Konto)
    {
        wRec.append(QSqlField(tr("kontoname")));
        wRec.setValue(0, kontoname);
    } else {
        wRec.append(QSqlField(tr("kategoriename")));
        wRec.setValue(0, kategoriename);
    }
    wRec.append(QSqlField(tr("saison")));
    wRec.setValue(1, QVariant(saison));
    s.append(QChar(' ')).append(driver->sqlStatement(QSqlDriver::WhereStatement, "buchungen", wRec, prepared));
    s.append(" ORDER BY datum, id");
    qDebug() << s;
    QSqlQuery * query;
    if(prepared)
    {
        query = new QSqlQuery();
        if(query->lastQuery() != s)
            query->prepare(s);

        query->addBindValue(wRec.value(0));
        query->addBindValue(wRec.value(1));
        query->exec();
    }
    else
    {
        query = new QSqlQuery(s);
    }
    return query;
}

QString PrintDialog::saldoSqlStatement()
{
    QString temp = (type == Konto ? 
                        QString("kontoname=\"").append(kontoname).append("\"") :
                        QString("kategoriename=\"").append(kategoriename).append("\"") );
    QString s = QString("SELECT sum(haben), sum(soll) FROM buchungen WHERE ")
        .append(temp).append(" AND saison < ").append(QString::number(saison));
#ifdef DEBUG
    qDebug() << s;
#endif
    return s;
}
