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
 ***************************************************************************/

#ifndef KLEINGARTENDELEGATE_H
#define KLEINGARTENDELEGATE_H

#include "defines.h"
#include <QItemDelegate>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <limits.h>
#include "kleingartenmodel.h"
#include "transaktion.h"

/**
	@author Martin Drohmann <mdrohmann@gmx.de>
*/
class KleingartenDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit KleingartenDelegate(QObject * parent = 0)
	:QItemDelegate(parent)
    {
    };

    ~KleingartenDelegate() {};

 
    QWidget *createEditor(QWidget *parent,
	    const QStyleOptionViewItem &option,
	    const QModelIndex &index) const;


    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    int getTransactionBuddyData(QString * kontoname, const QSqlQueryModel * model) const;

signals:
    void statusFixupChange( const QString & fixupString ) const;

private slots:
    QString fixupCombo( const QString & currentText ) const;

private:
    mutable QModelIndex lastIndex;
/*     enum DelegateColumn { DoubleAmount, Konto, Kategorie, Other };
    DelegateColumn editorCol;                                         */
    enum Type { Konto, Kategorie };
    mutable Type comboType;

    mutable QMap<int, QString> shortcutMap;

};
#endif
