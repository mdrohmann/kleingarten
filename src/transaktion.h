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

#ifndef TRANSAKTION_CLASS_H
#define TRANSAKTION_CLASS_H

#include "ui_transaktion.h"
#include <QSqlRecord>
#include <QSqlQueryModel>

class TransaktionDialog : public QDialog
{
    Q_OBJECT

public:
    TransaktionDialog( const QSqlQueryModel * kontenModel, QWidget * parent = 0 ) :
	QDialog(parent)// , rec()
    {
	ui.setupUi(this);

	ui.kontoCombo->setModel(const_cast<QSqlQueryModel*>(kontenModel));
	ui.kontoCombo->setModelColumn(0);

/*         connect(this, SIGNAL(accepted()), this, SLOT(trAccepted())); */
    };

    QString getCurrentText()
    {
	return ui.kontoCombo->currentText();
    };

    ~TransaktionDialog() { qDebug("kill the dialog"); };

/* private slots: */
/*     void trAccepted()
    {
    }                    */

private:
    Ui::TransactionDialog ui;
};

#endif /* _TRANSAKTION_CLASS_H */
