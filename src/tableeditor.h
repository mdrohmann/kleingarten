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
#ifndef TABLEEDITOR_H
#define TABLEEDITOR_H
#include "defines.h"

#include <QDoubleSpinBox>
#include <QDialog>
#include <QStackedLayout>
#include <QTableView>
#include <QItemEditorCreatorBase>
#include <QLabel>
#include <limits.h>
#include "kleingartenmodel.h"
#include "kleingartendelegate.h"
#include "printdialog.h"

class ItemEditorDouble : public QItemEditorCreatorBase
{
public:
    ItemEditorDouble() {};
    QWidget * createWidget ( QWidget * parent ) const
    {
        QDoubleSpinBox *sb = new QDoubleSpinBox(parent);
        sb->setFrame(false);
	sb->setDecimals(2);
	sb->setMaximum(LONG_MAX);
	return sb;
    };
    QByteArray valuePropertyName () const
    {
	return "value";
    };
    ~ItemEditorDouble() {};
};


class TableEditor : public QDialog
{
    Q_OBJECT
public:
    TableEditor(QWidget *parent = 0);
    ~TableEditor();

private slots:
    void currentTabChanged(int currentItem);
    void resizeViewColumns();
    void kontenFilterActivated(int checkState);
    void kategorienFilterActivated(int checkState);
    void addSaison();

private:
    void createContextMenu();
    PrintDialog * printDialog;
    QTableView * mainView;
    QTableView * kontenView;
    QTableView * kategorienView;
    QPushButton *submitButton;
    QPushButton *revertButton;
    QPushButton *quitButton;
    KleingartenModel *mainModel;
    QSqlTableModel *kontenModel;
    QSqlTableModel *kategorienModel;
    QStackedLayout *stackedLayout;
    QComboBox * saisonCombo;
    QComboBox * kontenCombo;
    QComboBox * kategorienCombo;
    QLabel * statusSumLabel;
    int currentSaison;
    void resizeColumn(int column, int width);
};

#endif
