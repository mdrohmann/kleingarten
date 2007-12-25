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

#include "kleingartendelegate.h"
#include <QInputDialog>
#include <QtGui>

int KleingartenDelegate::getTransactionBuddyData(QString * kontoname, const QSqlQueryModel * model) const
{
    TransaktionDialog trDialog(model);
    int ret = trDialog.exec();
    *kontoname = trDialog.getCurrentText();
    qDebug() << "in getTransactionBuddyData" << *kontoname << ret << QDialog::Accepted;
    return ret;
}

QWidget * KleingartenDelegate::createEditor(QWidget *parent,
	const QStyleOptionViewItem &option,
	const QModelIndex &index) const
{
    if (!index.isValid())
	return false;

    QVariant::Type t = index.data(Qt::EditRole).type();
    if (t == QVariant::Double)
    {
	QDoubleSpinBox *sb = new QDoubleSpinBox(parent);
	sb->setFrame(false);
	sb->setDecimals(2);
	sb->setMaximum(LONG_MAX);
	sb->selectAll();
	sb->installEventFilter(const_cast<KleingartenDelegate*>(this));
	return sb;
    }
    const KleingartenModel *sqlModel = qobject_cast<const KleingartenModel *>(index.model());
    QSqlQueryModel *childModel = sqlModel ? sqlModel->relationModel(index.column()) : 0;
    if (!sqlModel->isEditable(index))
    {
	QLineEdit *line = new QLineEdit(parent);
	line->setText(index.data(Qt::DisplayRole).toString());
	line->setInputMask(index.data(Qt::DisplayRole).toString().replace(QRegExp("([AaNnXx90Dd#HhBb><\\\\!])"), "\\\\1"));
	line->installEventFilter(const_cast<KleingartenDelegate*>(this));
	return line;
    }

    if (!childModel)
	return QItemDelegate::createEditor(parent, option, index);

    QComboBox *combo = new QComboBox(parent);
    combo->setModel(childModel);
    combo->setEditable(true);
    combo->setInsertPolicy(QComboBox::NoInsert);
    combo->setModelColumn(0);
    combo->installEventFilter(const_cast<KleingartenDelegate *>(this));
    connect(combo, SIGNAL(editTextChanged( const QString & )),
		this, SLOT(fixupCombo( const QString & )));
    if (index.column() == 7)
	comboType = Konto;
    else
	comboType = Kategorie;

    shortcutMap.clear();
    int shortcutIndex = childModel->record().indexOf("shortcut");
    for( int i=0; i < childModel->rowCount(); ++i)
    {
	int shortcut = childModel->data(childModel->index(i,shortcutIndex)).toInt();
	if (shortcut > 0)
	    shortcutMap.insert(shortcut, childModel->data(childModel->index(i, 0)).toString());
    }

    return combo;
}


void KleingartenDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    const KleingartenModel *sqlModel = qobject_cast<const KleingartenModel *>(index.model());

    QComboBox *combo = qobject_cast<QComboBox *>(editor);
    if (!sqlModel || !combo) {
	QItemDelegate::setEditorData(editor, index);
	QVariant::Type t = index.data(Qt::EditRole).type();
	if (t == QVariant::Double)
	{
	    QDoubleSpinBox *dsp = qobject_cast<QDoubleSpinBox *>(editor);
	    if (dsp)
		dsp->selectAll();
	}
	return;
    }
    combo->setCurrentIndex(combo->findText(sqlModel->data(index).toString()));
}


void KleingartenDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (!index.isValid())
	return;

    KleingartenModel *sqlModel = qobject_cast<KleingartenModel *>(model);
    QSqlQueryModel *childModel = sqlModel ? sqlModel->relationModel(index.column()) : 0;
    QComboBox *combo = qobject_cast<QComboBox *>(editor);
    if (!sqlModel || !childModel || !combo) {
	QItemDelegate::setModelData(editor, model, index);
	return;
    }

    if (!sqlModel->isEditable(index))
    {
	return;
    }

    int currentItem = combo->currentIndex();
    qDebug("currentItem: %i", combo->currentIndex());
    /*      sqlModel->setData(index,
	    childModel->data(childModel->index(currentItem, 0), Qt::DisplayRole),
	    Qt::DisplayRole);*/

    combo->setEditText(fixupCombo(combo->currentText()));

    if (combo->currentText() != combo->itemData(currentItem, Qt::EditRole).toString())
    {
	currentItem = combo->findText(combo->currentText());
    }
    if (currentItem == -1)
	currentItem = 0;

    if (index.column() == 8 
	    && childModel->data(childModel->index(currentItem, 0)) == tr("Transaktion") 
	    && sqlModel->needTrBuddyName(index.row())
	    && lastIndex != index)
    {
	int retOk;
/*         QString text = QInputDialog::getText(0, tr("Kontoname"),
		tr("Kontoname: "), QLineEdit::Normal,
		QString(), &retOk);                                 */
	lastIndex = index;
	QString kontoname;
	retOk = getTransactionBuddyData(&kontoname, sqlModel->relationModel(7));
	if (retOk == QDialog::Rejected)
	    return;
	sqlModel->setData(sqlModel->index(0,0,index), QVariant(kontoname), Qt::DisplayRole);

	sqlModel->setData(index,
		childModel->data(childModel->index(currentItem, 0), Qt::EditRole),
		Qt::EditRole);
	sqlModel->submitAll();
    }
    else
    {
	sqlModel->setData(index,
		childModel->data(childModel->index(currentItem, 0), Qt::EditRole),
		Qt::EditRole);
    }
}

QString KleingartenDelegate::fixupCombo( const QString & currentText ) const
{
    QString fixedString(currentText); 
    bool convOk;
    int shortcut = currentText.toInt(&convOk);
    if (convOk)
    {
	QMap<int,QString>::const_iterator nextMatch = shortcutMap.lowerBound(shortcut);
	if (nextMatch != shortcutMap.constEnd())
	{
	    fixedString = nextMatch.value();
	    emit statusFixupChange(QString::number(nextMatch.key()).append(" ").append(fixedString));
	}
	else
	{
	    emit statusFixupChange(" ");
	}
    }
    else
	emit statusFixupChange(QString());
    return fixedString;
}

