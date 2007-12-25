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
#ifndef KLEINGARTENMODEL_H
#define KLEINGARTENMODEL_H

#include "defines.h"
#include "transaktion.h"

#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>

/**
	@author Martin Drohmann <mdrohmann@gmx.de>
*/
class KleingartenModel : public QSqlTableModel
{
    Q_OBJECT
public:
    KleingartenModel(QObject * parent);
    QModelIndex index(int row, int column,
            const QModelIndex & parent = QModelIndex()
            ) const;
    QVariant data( const QModelIndex & index,
            int role = Qt::DisplayRole 
            ) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    void clear();
    bool isEditable(const QModelIndex & index) const;
    bool needTrBuddyName(const int row) const;
    QSqlQueryModel * relationModel(int column) const;
    void setRelationModel(int index, QSqlQueryModel * childModel);
    int getMaxSaison();
    double getSelectedSaldo();
    ~KleingartenModel();
    
public slots:
    void revertAll();
    void setKontoFilter(const QString & kontoname);
    void setKategorieFilter(const QString & kategoriename);
    void setSaison(const QString & saison);
    bool submit();
    bool submitAll();
    bool submitWithSelect();
    void setAltSort(bool enabled);

signals:
    void newSelect();
    void updateSaldo(const QString & saldo);

protected:
    bool insertRowIntoTable ( const QSqlRecord & values );
    bool updateRowInTable( int row, const QSqlRecord & values );
    QSqlRecord primaryValues(int row);

private:
    bool select();
    bool altSortEnabled;
    static const int viewableColumnCount = 9;
    QString sqlStatement();
    QString saldoSqlStatement();
    void updateLRec();
    void clearInsertBuffer();
    bool readyToInsertRow();
    bool commitInsertBuffer();
    bool exec(const QString &stmt, bool prepStatement,
	    const QSqlRecord & rec, const QSqlRecord & whereValues = QSqlRecord());

    QString kontoname;
    QString kategoriename;
    QSqlRecord lRec;
    QSqlRecord mRec;
    int saison;

    int insertIndex;
    int editRowIndex;
    int maxSaison;
    int maxId;
    enum RowState { None, Insert, Update, Commited };
    struct ModifiedRow
    {
        ModifiedRow(
                const RowState s = None,
                const QSqlRecord &r = QSqlRecord(),
                const bool t = false
                ):
            state(s), rec(r), tr(t) {};
        ModifiedRow(const ModifiedRow & other):
            state(other.state), rec(other.rec), tr(other.tr) {};
        RowState state;
        QSqlRecord rec;
        bool tr; /* transaction */
    };

    struct TransactionCache
    {
        QSqlRecord rec;
        bool active;
    };
    TransactionCache trRow; 

    void updateTransactionBuddy(
            QSqlRecord * buddyTarget,
            const QSqlRecord & buddySource
            );

    QMap <int, int> trMap;
    QList <int> rowToIdMap;
    QMap <int, int> idToRowMap;

    typedef QMap<int, ModifiedRow> RecordCache;
    RecordCache cache;
/*     QSqlRecord editBuffer; */
    QSqlRecord insertBuffer;
    QSqlRecord primaryIndex;
    QSqlQuery editQuery;
    double selectedSaldo;
    mutable QSqlQueryModel * relations[2];
    QString * relQueries[2];
    QString buddyKonto;
};

#endif
