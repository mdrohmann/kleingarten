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

#include <QtGui>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlIndex>
#include <QSqlField>
#include <QSqlError>
#include "kleingartenmodel.h"
#include "kleingartendelegate.h"

QModelIndex KleingartenModel::index(int row, int column,
        const QModelIndex & parent) const
{
    if (!parent.isValid())
	return QSqlTableModel::index(row,column,parent);
    if (row != 0 || column != 0)
	return QModelIndex();
    if (parent.row() >= rowCount() || parent.row() < 0 || parent.column() < 0)
	return QModelIndex();
    return createIndex(0,0,1);
}

void KleingartenModel::updateLRec()
{
    if (lRec.isEmpty())
	lRec = query().record();
    if (mRec.isEmpty())
	mRec = lRec;
    for (int i=0; i<lRec.count(); ++i)
    {
	mRec.setValue(i, QVariant());
	lRec.setValue(i, QVariant());
	/* Remark: The generated flag of insertBuffer and editBuffer is misused, as we
	 * interprete a positive value as a user edited column. In the beginning the user
	 * hasn't done anything, so all flags are false. */
	lRec.setGenerated(i, false);
	mRec.setGenerated(i, false);
    }
    int year = (saison == 0) ? QDate::currentDate().year() : saison;
    lRec.setValue(0, QDate(year, 1, 1).toString(Qt::ISODate));
    lRec.setValue(1, QDate(year, 1, 1).toString(Qt::ISODate));
    lRec.setGenerated(0, false);
    lRec.setGenerated(1, false);
}

bool KleingartenModel::isEditable(const QModelIndex & index) const
{
    if ( index.column() == lRec.indexOf("kontoname") 
            && !kontoname.isEmpty()
            && data(index).toString() == kontoname 
       )
	return false;
    if ( index.column() == lRec.indexOf("kategoriename") 
            && !kategoriename.isEmpty()
       )
	return false;
    if ( index.column() == lRec.indexOf("kategoriename") 
            && trMap.contains(rowToIdMap.value( index.row() ))
       )
	return false;
    return true;
}

bool KleingartenModel::needTrBuddyName(const int row) const
{
    return (!trMap.contains( rowToIdMap[row] ));
}

bool KleingartenModel::readyToInsertRow()
{
    for (int i = 0; i < viewableColumnCount; ++i)
    {
	qDebug() << i << " " << insertBuffer.isGenerated(i);
	if (! insertBuffer.isGenerated(i))
	    return false;
    }
    return true;
}

void KleingartenModel::clearInsertBuffer()
{
    insertBuffer = lRec;
    if(!kontoname.isEmpty())
    {
	insertBuffer.setValue(7, kontoname);
	insertBuffer.setGenerated(7, true);
    }
    if(!kategoriename.isEmpty())
    {
	insertBuffer.setValue(8, kategoriename);
	insertBuffer.setGenerated(8, true);
    }
}

KleingartenModel::KleingartenModel(QObject * parent)
    : QSqlTableModel(parent), altSortEnabled(false)
{
    relations[0] = relations[1] = 0;
    relQueries[0] = new QString(
            "SELECT kontoname, shortcut, season_hide \
                    FROM konten \
                    ORDER BY kontoname"
            );
    relQueries[1] = new QString(
            "SELECT kategoriename, shortcut, season_hide \ 
                    FROM kategorien \
                    ORDER BY kategoriename"
            );
    kontoname="Kasse";
    QSqlQuery q("SELECT max(saison) FROM buchungen");
    q.first();
    maxSaison = q.value(0).toInt();
    saison = maxSaison;

/*     qDebug() << "prepared Queries: " << QSqlDatabase::database().driver()->hasFeature(QSqlDriver::PreparedQueries); */

    select();

/*     qDebug() << "primary Index: " << QSqlDatabase::database().primaryIndex("buchungen").fieldName(0); */
    primaryIndex = QSqlDatabase::database().primaryIndex("buchungen");

#ifdef DEBUG
    for (int i = 0; i < insertBuffer.count(); ++i)
    {
        qDebug() << insertBuffer.field(i).type() << " " << insertBuffer.value(i).toString();
    }
#endif

    insertIndex  = 0;
    editRowIndex = -1;
}

KleingartenModel::~KleingartenModel()
{
    delete relQueries[0];
    delete relQueries[1];
    // TODO: Ask the user if she wants to commit changes.
    clear();
}

void KleingartenModel::revertAll()
{
    if (editRowIndex != -1) {
	int oldIndex = editRowIndex;
	editRowIndex = -1;
	emit dataChanged(createIndex(oldIndex, 0),
		createIndex(oldIndex, columnCount()));
    }
    clearInsertBuffer();
    insertIndex = 0;
}

bool KleingartenModel::select()
{
    /* TODO: Check if there are changes that might be discarded, and ask the user if he really wants this. */
//    revertAll();

    clear();

    QSqlQuery saldoQuery(saldoSqlStatement());
    saldoQuery.first();
    selectedSaldo = saldoQuery.value(0).toDouble() 
        - saldoQuery.value(1).toDouble();

    QSqlQuery maxIdQuery("SELECT max(id) FROM buchungen");
    maxIdQuery.first();
    maxId = maxIdQuery.value(0).toInt();
    emit updateSaldo(QString::number(selectedSaldo, 'f', 2));
    this->setQuery(QSqlQuery("SELECT * FROM buchungen WHERE id=1"));
    QSqlQuery qu(sqlStatement());
    this->setQuery(qu);
    while(canFetchMore()) 
    {
        fetchMore();
    } 

    trMap.clear();
    idToRowMap.clear();
    rowToIdMap.clear();
    rowToIdMap.append(0);
    for (int row = 0; row < QSqlTableModel::rowCount(); ++row)
    {
        int id = QSqlTableModel::data(createIndex(row,10)).toInt();
        int tp = QSqlTableModel::data(createIndex(row,9)).toInt();
        rowToIdMap.append(id); 
        if (tp > 0)
        {
            trMap.insert(id, tp);
            idToRowMap.insert(id, row + 1);
        }
    }

    this->updateLRec();
    this->clearInsertBuffer();
    editRowIndex=-1;
    insertIndex = 0;
    emit newSelect();

    return qu.isActive();
}

QString KleingartenModel::sqlStatement() 
{
    QString w;
    QString s("SELECT \
	    datum, rdatum, beleg,auszug,beschreibung, \
	    haben,soll, \
	    kontoname,kategoriename,tp,id FROM buchungen");
    if(!kontoname.isEmpty())
	w.append(QString("kontoname=\"%1\"").arg(kontoname)); 
    if(!kategoriename.isEmpty())
    {
        if(!w.isEmpty())
            w.append(" AND ");
        w.append(QString("kategoriename=\"%1\"").arg(kategoriename));
    }
    if(!saison==0)
    {
        if(!w.isEmpty())
            w.append(" AND ");
        w.append(QString("saison=%1").arg(saison));
    }
    if( !w.isEmpty() )
	s.append(" WHERE (").append(w).append(") OR (tp is NOT NULL AND id \
	    	IN (SELECT tp FROM buchungen WHERE ").append(w).append("))");
    
    if (altSortEnabled)
	s.append(" ORDER BY id DESC");
    else
	s.append(" ORDER BY datum DESC, rdatum DESC, id DESC");
    
//    qDebug() << "sqlStatement: " << s;
    return s;
}

QString KleingartenModel::saldoSqlStatement()
{
    QString w;
    QString s("SELECT SUM(haben), SUM(soll) FROM buchungen");
    if(!kontoname.isEmpty())
	w.append(QString("kontoname=\"%1\"").arg(kontoname));
    if(!kategoriename.isEmpty())
    {
	if(!w.isEmpty())
	    w.append(" AND ");
	w.append(QString("kategoriename=\"%1\"").arg(kategoriename));
    }
    if(!saison==0)
    {
	if(!w.isEmpty())
	    w.append(" AND ");
	w.append(QString("saison<=%1").arg(saison));
    }
    if(!w.isEmpty())
	s.append(" WHERE ").append(w);
/*     qDebug() << "saldoSqlStatement: " << s; */
    return s;
}

int KleingartenModel::rowCount(const QModelIndex &parent) const
{
    return QSqlTableModel::rowCount(parent) + 1;
}

int KleingartenModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return QSqlTableModel::columnCount(parent);
    }
    return viewableColumnCount;
}

void KleingartenModel::setRelationModel(int index, QSqlQueryModel * childModel)
{
    relations[index]=childModel;
}

QSqlQueryModel * KleingartenModel::relationModel(int column) const
{
    int mapped_col=column-7;
    if (mapped_col<0 || mapped_col > 1)
	return 0;
    if (!relations[mapped_col])
    {
        relations[mapped_col] = new QSqlQueryModel(
                const_cast<KleingartenModel *>(this)
                );

        relations[mapped_col]->setQuery(*relQueries[mapped_col]);
    }

    return relations[mapped_col];
}

QVariant KleingartenModel::data( const QModelIndex & index, int role ) const
{
    QVariant v;
    /* return the first Row, where user can insert buchungen */
    if (index.row() == insertIndex)
    {
        if (role == Qt::EditRole || role == Qt::DisplayRole)
            v = insertBuffer.value(index.column());
    }
    /* return potentially edited rows */
    else
    {
        v = QSqlQueryModel::data(createIndex(index.row() - 1, index.column()),
                role);
        if (role == Qt::EditRole || role == Qt::DisplayRole)
        {
            QSqlRecord rtemp = cache.value( index.row() ).rec;
            QVariant vtemp = rtemp.value( index.column() );
            if(vtemp.type() != QVariant::Invalid 
                    && rtemp.isGenerated(index.column())
              ) {
                v = vtemp;
            }
        }
    }

    if (index.column()==1 || index.column()==0)
    {
        if ( role == Qt::EditRole )
            v = QDate::fromString(v.toString(), Qt::ISODate);
        if ( role == Qt::DisplayRole)
            v = QDate::fromString(v.toString(), Qt::ISODate)
                .toString("dd.MM.yyyy");
    }

    if (index.column()==5 || index.column()==6)
    {
        switch (role)
        {
            case Qt::EditRole:
                v=v.toDouble();
                break;
            case Qt::DisplayRole:
                if (v.toDouble() == 0)
                    v=QString("");
                else
                    v = QString::number(v.toDouble(), 'f', 2);
                break;
            case Qt::TextAlignmentRole:
                v = QVariant((Qt::AlignRight | Qt::AlignVCenter));
        }
    }

    if ((index.column()==2 || index.column()==3) 
            && role == Qt::TextAlignmentRole
       ) {
        v = QVariant((Qt::AlignRight | Qt::AlignVCenter));
    }

    if (role == Qt::TextColorRole  
            && data(createIndex(index.row(),7),Qt::DisplayRole).toString() 
               != kontoname 
            && data(createIndex(index.row(),8),Qt::DisplayRole).toString() 
               == tr("Transaktion")) {
        v=QColor(Qt::darkCyan);
    }

    /*     if(role == Qt::EditRole )
           qDebug() << v.type();    */

    return v; 
}

bool KleingartenModel::setData(const QModelIndex & index,
        const QVariant & value,
        int role)
{
    if (index.internalId() == 1)
    {
        buddyKonto = value.toString();
        /*         qDebug() << "buddyKonto: " << buddyKonto; */
        return true;
    }

    /* if nothing interesting happens, we simply send our arguments to the parent class */
    if (index.row() != insertIndex && role != Qt::EditRole)
	return QSqlTableModel::setData(createIndex(index.row()-1,
		    index.column()),value,role);

    if (index.column() >= lRec.count() || index.row() >= rowCount())
	return false;

    bool isOk=true;

    /* are we in a new row, and the last edited row's changes haven't been commited yet? */
    if (editRowIndex != index.row())
	if (editRowIndex != -1)
	    submitAll();

    /* first row? i.e. editing a new insertion */
    if (index.row() == insertIndex)
    {
        insertBuffer.setValue(index.column(), value);
        insertBuffer.setGenerated(index.column(), true);
        if (index.column() == 0 && (!insertBuffer.isGenerated(1)))
        {
            insertBuffer.setValue(1, value);
            insertBuffer.setGenerated(1, true);
            emit dataChanged(createIndex(insertIndex,0),
                    createIndex(insertIndex,1));
        }
        else
            emit dataChanged(index,index);
        if (readyToInsertRow())
        {
            return commitInsertBuffer();
        }
    }
    else /* we modify an existing table entry */
    {
        ModifiedRow & mr = cache[ index.row() ];
        if(mr.state == None)
            mr.rec = mRec;
        mr.rec.setValue(index.column(), value);
        mr.rec.setGenerated(index.column(), true);
        mr.state = Update;

        editRowIndex = index.row();
        emit dataChanged(index,index);

        int id = rowToIdMap[ index.row() ];

        /* do we work with a transaction? */
        if (trMap.contains(id))
        {
            int row = idToRowMap[ trMap[id] ];
            /*             qDebug("in setData: transaktion!!!! row: %i, id: %i, tp: %i, crow: %i", row, id, trMap[id], index.row()); */
            /*             qDebug("id->row: %i", idToRowMap[id]); */
            ModifiedRow & tmr = cache[ row ];
            if(tmr.state == None)
                tmr.rec = mRec;
            updateTransactionBuddy(&tmr.rec, mr.rec);
            tmr.state = Update;
            /*             qDebug() << "haben: " << mr.rec.value(5).toString() << "soll: " << mr.rec.value(6).toString(); */
            /*             qDebug() << "haben: " << tmr.rec.value(5).toString() << "soll: " << tmr.rec.value(6).toString(); */
            tmr.state = Update;
            emit dataChanged(createIndex(row, 0), createIndex(row,6));
        }
    }

    return isOk;
}

bool KleingartenModel::commitInsertBuffer()
{
    qDebug() << "ready to insert row!";
    if (insertBuffer.value(8).toString() == tr("Transaktion"))
    {
        insertBuffer.setValue(9, QVariant(maxId + 2));
        insertBuffer.setGenerated(9, true);
        insertBuffer.setValue(10, QVariant(maxId + 1));
        insertBuffer.setGenerated(10, true);

        if (insertRowIntoTable(insertBuffer))
        {
            QSqlRecord buddyBuffer = insertBuffer;
            buddyBuffer.setValue(9, QVariant(maxId + 1));
            buddyBuffer.setValue(10, QVariant(maxId + 2));
            buddyBuffer.setValue(6, insertBuffer.value(5));
            buddyBuffer.setValue(5, insertBuffer.value(6));
            buddyBuffer.setValue(7, QVariant(buddyKonto));
            buddyKonto = QString();
            insertRowIntoTable(buddyBuffer);
        }
    }
    else
	insertRowIntoTable(insertBuffer);
    clearInsertBuffer();
    return select();
}

void KleingartenModel::updateTransactionBuddy(QSqlRecord * buddyTarget,
        const QSqlRecord & buddySource)
{
    for (int i=0; i < 5; ++i)
    {
        if(buddySource.value(i).isValid())
        {
            buddyTarget->setValue(i, buddySource.value(i));
            buddyTarget->setGenerated(i, true);
        }
    }
    if(buddySource.value(6).isValid())
    {
        buddyTarget->setValue(5, buddySource.value(6));
        buddyTarget->setGenerated(5, true);
    }
    if(buddySource.value(5).isValid())
    {
        buddyTarget->setValue(6, buddySource.value(5));
        buddyTarget->setGenerated(6, true);
    }
}

QSqlRecord KleingartenModel::primaryValues(int row)
{
    QSqlRecord record;
    if(!query().seek(row)) {
        return record;
    }
    record = primaryIndex;
/*     qDebug() << record.fieldName(0) << " " << record.count(); */
    for (int i=0; i< record.count(); ++i)
        record.setValue(i, query().value(lRec.indexOf(record.fieldName(i))));
	
    return record;
}

bool KleingartenModel::updateRowInTable( int row, const QSqlRecord & values )
{
    qDebug("in updateRowInTable(int row=%i)",row);
    QSqlRecord rec(values);
    QSqlDriver * driver = QSqlDatabase::database().driver();
    emit beforeUpdate(row,rec);

    const QSqlRecord whereValues = primaryValues(row-1);
    bool prepStatement = driver->hasFeature(QSqlDriver::PreparedQueries);
    QString stmt = driver->sqlStatement(QSqlDriver::UpdateStatement,
            "buchungen",
            rec,
            prepStatement);
    QString where = driver->sqlStatement(QSqlDriver::WhereStatement,
            "buchungen",
            whereValues,
            prepStatement);

    if (stmt.isEmpty() || where.isEmpty() || row < 0 || row >= rowCount()) {
        return false;
    }
    stmt.append(QChar(' ')).append(where);

/*     qDebug() << "updateStatement : " << where; */

    return exec(stmt, prepStatement, rec, whereValues);
}

bool KleingartenModel::insertRowIntoTable ( const QSqlRecord & values ) 
{
    qDebug() << "in insertRowIntoTable()";
    QSqlRecord rec(values);
    rec.append(QSqlField(QString("saison"), QVariant::Int));
    rec.setValue("saison", saison);

    QSqlDriver * driver = QSqlDatabase::database().driver();
    emit beforeInsert(rec);

    bool prepStatement = driver->hasFeature(QSqlDriver::PreparedQueries); 
    QString stmt = driver->sqlStatement(QSqlDriver::InsertStatement,
            "buchungen",
            rec,
            prepStatement);
/*     qDebug() << "the insert Statement: \n" << stmt; */
    
    return exec(stmt, prepStatement, rec);
}

bool KleingartenModel::exec(const QString &stmt, bool prepStatement,
	const QSqlRecord & rec, const QSqlRecord & whereValues /* = QSqlRecord() */)
{
    if (stmt.isEmpty())
	return false;

    QSqlDatabase db = QSqlDatabase::database();
    if (editQuery.driver() != db.driver())
	editQuery = QSqlQuery(db);

    if (prepStatement) {
        if(editQuery.lastQuery() != stmt) {
            if(!editQuery.prepare(stmt)) {
                qDebug() << "no prepare: " << editQuery.lastError().text();
                return false;
            }
        }
        int i;
        for (i=0; i< rec.count(); ++i) {
            /*             qDebug() << rec.value(i).toString() << QString::fromUtf8(" wird eingefügt "); */
            if (rec.isGenerated(i) && rec.value(i).type() != QVariant::Invalid)
                editQuery.addBindValue(rec.value(i));
        }
        for (i = 0; i < whereValues.count(); ++i) {
            if (whereValues.isGenerated(i))
                editQuery.addBindValue(whereValues.value(i));
        }

        if (!editQuery.exec()) {
            qDebug() << "no exec: " << editQuery.lastError().text();
            return false;
        }
    }

    qDebug() << QString("executed: %1 (%2)").arg(stmt).arg(editQuery.numRowsAffected());

    return true;
}

void KleingartenModel::setAltSort(bool enabled)
{
    if (altSortEnabled != enabled)
    {
        altSortEnabled = enabled;
        submitWithSelect();
    }
    else
        altSortEnabled = enabled;
}

bool KleingartenModel::submit()
{
    qDebug() << "in submit()";
    return submitAll();
}

bool KleingartenModel::submitWithSelect()
{
    bool isOk = submitAll(); 
    if(isOk)
	isOk = select();
    return isOk;
}

bool KleingartenModel::submitAll()
{
    qDebug() << "in submitAll()";
    bool isOk=true;
    if(editRowIndex != -1)
    {
        int id = rowToIdMap[ editRowIndex ];
        /* do we have a transaction? */
        if (data( createIndex(editRowIndex, 8), Qt::DisplayRole).toString() 
                == tr("Transaktion"))
        {
            /* transaction buddy's row exists already? */
            if (trMap.contains(id))
            {
                database().transaction();
                updateRowInTable(editRowIndex, cache[ editRowIndex ].rec);
                updateRowInTable(idToRowMap[trMap[id]],
                        cache[ idToRowMap[ trMap[id] ] ].rec);
                /*                 qDebug() << editRowIndex << " " << idToRowMap[ trMap[id] ];  */
                isOk = database().commit();
            }
            /* we must add the transaction */
            else
            {
                ModifiedRow & mRow = cache[ editRowIndex ];
                QSqlRecord & rec1 = mRow.rec;
                QSqlRecord rec2 = rec1;
                for (int i=0; i<9; ++i)
                {
                    rec2.setValue(i, data( createIndex(editRowIndex, i),
                                Qt::EditRole ));
                    rec2.setGenerated(i,true);
                }
                /* set the new record's tp column to the buddy's id */
                rec2.setValue(9, QVariant( rowToIdMap[ editRowIndex ] ));
                rec2.setGenerated(9, true);
                /* set the new record's id column explicitly... */
                rec2.setValue(10, QVariant(maxId + 1));
                rec2.setGenerated(10, true);
                /* ...and attach the buddy through the tp column */
                rec1.setValue(9, QVariant(maxId + 1));
                rec1.setGenerated(9, true);

                /* swap "haben" and "soll" */
                rec2.setValue(5, data( createIndex(editRowIndex, 6),
                            Qt::EditRole ));
                rec2.setValue(6, data( createIndex(editRowIndex, 5),
                            Qt::EditRole ));
                /* set the kontoname */
                rec2.setValue(7, buddyKonto);

                /*                 for (int i=0; i<9; ++i)
                                   {
                                   qDebug() << "i = " << i << " <-> " << rec2.value(i).toString();
                                   }                                                                      */
                database().transaction();
                updateRowInTable(editRowIndex, rec1);
                insertRowIntoTable(rec2);
                database().commit();
                isOk = select();
            }
        }
        /* no transaction */
        else
        {
            updateRowInTable(editRowIndex, cache[ editRowIndex ].rec);
        }
    }
    editRowIndex = - 1;

    /* commit the insertBuffer if the latter is ready */
    if (readyToInsertRow() && isOk)
	isOk = commitInsertBuffer();
    return isOk;
}

void KleingartenModel::setKontoFilter(const QString & kontoname)
{
    qDebug() << "in setKontoFilter mit " << kontoname;
    this->kontoname=kontoname;
    submitWithSelect();
}

void KleingartenModel::setKategorieFilter(const QString & kategoriename)
{
    qDebug() << "in setKategorieFilter mit " << kategoriename;
    this->kategoriename=kategoriename;
    submitWithSelect();
}

void KleingartenModel::setSaison(const QString & saison)
{
    qDebug() << "in setSaison mit " << saison;
    this->saison=saison.toInt();
    select();
}

/*! returns the last saison for which an item has been booked when model initialised
 * The function return the year of the last saison for which an item has been booked
 * when a \a KleingartenModel has been initialised. NB: This value does not change
 * while working with the model.
 */
int KleingartenModel::getMaxSaison()
{
    return maxSaison;
}

void KleingartenModel::clear()
{
    cache.clear();
    editQuery.clear();
    clearInsertBuffer();
/*     lRec.clear(); */
    QSqlTableModel::clear();
}

double KleingartenModel::getSelectedSaldo()
{
    return selectedSaldo;
}
