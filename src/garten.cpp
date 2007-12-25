/***************************************************************************
 *   Copyright (C) 2006 by Martin Drohmann                                 *
 *   mdrohmann@gmx.de                                                      *
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
 ***************************************************************************/
// #include "KleingartenWindow.h"

#include <QtGui>
#include <QtSql>

#include "dbconnection.h"
#include "tableeditor.h"
#include "printdialog.h"


int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QLocale::setDefault(QLocale::German);

    QApplication app(argc, argv);
    if (!createConnection())
	return 1;

    QSqlQuery query("SELECT datum, auszug, beleg, beschreibung, haben, soll FROM buchungen WHERE kontoname=\"Kasse\" AND saison=2005");

    while (query.next())
    {
	QStringList temp;
	for(int i=0; i< query.record().count(); ++i)
	{
	    temp << query.value(i).toString() << query.value(i).typeName() << QString::number(query.record().field(i).typeID());
	}
/*         qDebug() << temp; */
    }
    TableEditor table;
    table.showMaximized();

    return table.exec();
}
