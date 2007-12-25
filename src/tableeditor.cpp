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
#include <QtGui>
#include <QtSql>

#include "tableeditor.h"
#define DEBUG

TableEditor::TableEditor(QWidget *parent)
: QDialog(parent)
{
    mainModel = new KleingartenModel(this);
    mainModel->setHeaderData(0, Qt::Horizontal, tr("Datum"));
    mainModel->setHeaderData(1, Qt::Horizontal, tr("RDatum"));
    mainModel->setHeaderData(2, Qt::Horizontal, tr("Beleg"));
    mainModel->setHeaderData(3, Qt::Horizontal, tr("Auszug"));
    mainModel->setHeaderData(4, Qt::Horizontal, tr("Beschreibung"));
    mainModel->setHeaderData(5, Qt::Horizontal, tr("Soll"));
    mainModel->setHeaderData(6, Qt::Horizontal, tr("Haben"));
    mainModel->setHeaderData(7, Qt::Horizontal, tr("Konto"));
    mainModel->setHeaderData(8, Qt::Horizontal, tr("Kategorie"));
    mainModel->setHeaderData(9, Qt::Horizontal, tr("Saison"));

    kontenModel = new QSqlTableModel(this);
    kontenModel->setTable("konten");
    kontenModel->select();
    kontenModel->setHeaderData(0, Qt::Horizontal, tr("Konto"));
    kontenModel->setHeaderData(1, Qt::Horizontal, tr("Ktn"));
    kontenModel->setHeaderData(2, Qt::Horizontal, tr("BLZ"));
    kontenModel->setHeaderData(3, Qt::Horizontal, tr("Beschreibung"));
    kontenModel->setHeaderData(4, Qt::Horizontal, tr("Shortcut"));
    kontenModel->setHeaderData(5, Qt::Horizontal, tr("Verstecke Saison"));

    kategorienModel = new QSqlTableModel(this);
    kategorienModel->setTable("kategorien");
    kategorienModel->select();
    kategorienModel->setHeaderData(0, Qt::Horizontal, tr("Kategorie"));
    kategorienModel->setHeaderData(1, Qt::Horizontal, tr("Beschreibung"));
    kategorienModel->setHeaderData(2, Qt::Horizontal, tr("Shortcut"));
    kategorienModel->setHeaderData(3, Qt::Horizontal, tr("Verstecke Saison"));

    mainModel->setRelationModel(0, kontenModel);
    mainModel->setRelationModel(1, kategorienModel);

/*     mainModel->setRelationModel(0,kontenModel);
    mainModel->setRelationModel(1,kategorienModel);    */

    mainView = new QTableView;
    mainView->setModel(mainModel);
    mainView->setColumnWidth(0, 75);
    mainView->setColumnWidth(1, 75);
    mainView->setColumnWidth(2, 60);
    mainView->setColumnWidth(3, 60);
    mainView->setColumnWidth(4, 250);
    mainView->setColumnWidth(5, 60);
    mainView->setColumnWidth(6, 60);
    mainView->setColumnWidth(7, 140);
    mainView->setColumnWidth(8, 160);
    connect(mainModel, SIGNAL(newSelect()), this, SLOT(resizeViewColumns()));
    kontenView = new QTableView;
    kontenView->setModel(kontenModel);
    kontenView->setColumnWidth(0, 140);
    kontenView->setColumnWidth(1, 70);
    kontenView->setColumnWidth(2, 70);
    kontenView->setColumnWidth(3, 240);
    kontenView->setColumnWidth(4, 50);
    kontenView->setColumnWidth(5, 50);
    kategorienView = new QTableView;
    kategorienView->setModel(kategorienModel);
    kategorienView->setColumnWidth(0, 240);
    kategorienView->setColumnWidth(1, 240);
    kategorienView->setColumnWidth(2, 50);
    kategorienView->setColumnWidth(3, 50);

    /* init the print/preview Dialog */
    printDialog = new PrintDialog(kontenModel, kategorienModel, this);

    KleingartenDelegate * kgd = new KleingartenDelegate();
    mainView->setItemDelegate(kgd);

    createContextMenu();

/*     now we create the layout */
/*     first the tab widget */
    QTabWidget * tw = new QTabWidget();
    tw->addTab(kontenView, tr("K&onten"));
    tw->addTab(mainView, tr("&Buchungen"));
    tw->addTab(kategorienView, tr("K&ategorien"));
    tw->setCurrentIndex(1);

/*     now the buttons for printing and preview */
    QPushButton * printButton = new QPushButton(QString(tr("Drucken...")));
/*    QPushButton * deleteButton = new QPushButton(QString(tr("Lösche..."))); */
    connect(printButton, SIGNAL(clicked()), printDialog, SLOT(show()));
    
    QHBoxLayout * printLayout = new QHBoxLayout();
    printLayout->setMargin(0);
    printLayout->addWidget(printButton);
/*     printLayout->addWidget(previewButton);  */
    printLayout->addStretch(1);

/*     and the filters for the main view */
    QCheckBox * kontenFilterCheck = new QCheckBox(QString(tr("Konto:")));
    kontenFilterCheck->setCheckState(Qt::Checked);
    kontenCombo = new QComboBox();
    kontenCombo->setModel(mainModel->relationModel(7));
    kontenCombo->setCurrentIndex(kontenCombo->findText("Kasse"));
    connect(kontenCombo, SIGNAL(currentIndexChanged(const QString &)),
            printDialog, SLOT(setKontoname(const QString &))
            );
    connect(kontenFilterCheck, SIGNAL(stateChanged(int)),
            this,              SLOT(kontenFilterActivated(int))
            );
    connect(kontenFilterCheck, SIGNAL(toggled(bool)),
            kontenCombo,       SLOT(setEnabled(bool))
            );
    connect(kontenFilterCheck, SIGNAL(toggled(bool)),
            printDialog,       SLOT(setKonto(bool))
            );

    QCheckBox * kategorienFilterCheck = new QCheckBox(
            QString(tr("Kategorie:"))
            );

    kategorienFilterCheck->setCheckState(Qt::Unchecked);
    kategorienCombo = new QComboBox();
    kategorienCombo->setModel(mainModel->relationModel(8));
    kategorienCombo->setCurrentIndex(0);
    kategorienCombo->setEnabled(false);
    connect(kategorienCombo, SIGNAL(currentIndexChanged(const QString &)),
            printDialog,     SLOT(setKategoriename(const QString &))
            );
    connect(kategorienFilterCheck, SIGNAL(stateChanged(int)),
            this,                  SLOT(kategorienFilterActivated(int))
            );
    connect(kategorienFilterCheck, SIGNAL(toggled(bool)),
            kategorienCombo,       SLOT(setEnabled(bool))
            );
    connect(kategorienFilterCheck, SIGNAL(toggled(bool)),
            printDialog, SLOT(setKategorie(bool))
            );

    saisonCombo = new QComboBox();
    currentSaison = mainModel->getMaxSaison();
    QStringList saisons;
    for (int i=2002; i<=currentSaison; ++i)
	saisons << QString::number(i);
    saisonCombo->addItems(saisons);
    saisonCombo->setCurrentIndex(saisonCombo->count()-1);
    printDialog->setSaison(currentSaison);
    connect(saisonCombo, SIGNAL(currentIndexChanged(const QString &)),
            printDialog, SLOT(setSaison(const QString &))
            );

    QPushButton * newSaisonButton = new QPushButton(
            QString(tr("Neue Saison..."))
            );

    connect(newSaisonButton, SIGNAL(clicked()), this, SLOT(addSaison()));

    QPushButton * refreshButton = new QPushButton(tr("Aktualisieren"));
    connect(refreshButton, SIGNAL(clicked()),
            mainModel,     SLOT(submitWithSelect())
            );

    QCheckBox * altSortCheck = new QCheckBox(tr("Alt. Sortierung"));
    connect(altSortCheck, SIGNAL(toggled(bool)),
            mainModel,    SLOT(setAltSort(bool))
            );
   
/*  the buchungen layout */
    QHBoxLayout * buchungenLayout = new QHBoxLayout;
    buchungenLayout->setMargin(0);
    buchungenLayout->addWidget(kontenFilterCheck);
    buchungenLayout->addWidget(kontenCombo);
    buchungenLayout->addWidget(kategorienFilterCheck);
    buchungenLayout->addWidget(kategorienCombo);
    buchungenLayout->addWidget(saisonCombo);
    buchungenLayout->addWidget(newSaisonButton);
    buchungenLayout->addWidget(refreshButton);
    buchungenLayout->addWidget(altSortCheck);
    buchungenLayout->addStretch(1);

/*     stack the print and filter layout */
    QWidget * printWidget = new QWidget();
    printWidget->setLayout(printLayout);
    QWidget * buchungenWidget = new QWidget();
    buchungenWidget->setLayout(buchungenLayout);
    buchungenWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    stackedLayout = new QStackedLayout();
    stackedLayout->addWidget(printWidget);
    stackedLayout->addWidget(buchungenWidget);
    stackedLayout->setMargin(0);
    stackedLayout->setCurrentIndex(1);

    connect(tw, SIGNAL(currentChanged(int)), 
            this, SLOT(currentTabChanged(int))
            );
    connect(kontenCombo, SIGNAL(currentIndexChanged(const QString&)),
            mainModel,   SLOT(setKontoFilter(const QString &))
            );
    connect(kategorienCombo, SIGNAL(currentIndexChanged(const QString&)),
            mainModel,       SLOT(setKategorieFilter(const QString &))
            );
    connect(saisonCombo, SIGNAL(currentIndexChanged(const QString&)),
            mainModel,   SLOT(setSaison(const QString &))
            );

/*     a status line */
    QLabel * statusFixupLabel = new QLabel();
    statusFixupLabel->setText("");
    statusFixupLabel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    connect(kgd,              SIGNAL(statusFixupChange(const QString &)),
            statusFixupLabel, SLOT(setText(const QString &))
            );

    statusSumLabel = new QLabel();
    statusSumLabel->setText(QString::number(mainModel->getSelectedSaldo(),
                            'f',2)
            );
    statusSumLabel->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    connect(mainModel,      SIGNAL(updateSaldo(const QString & )), 
            statusSumLabel, SLOT(setText(const QString &))
            );

    QHBoxLayout * statusLineLayout = new QHBoxLayout();
    statusLineLayout->addWidget(statusFixupLabel);
    statusLineLayout->addStretch(1);
    statusLineLayout->addWidget(statusSumLabel);
    statusLineLayout->addSpacing(20);

/*     add all together */
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(stackedLayout);
    mainLayout->addWidget(tw,1);
    mainLayout->addLayout(statusLineLayout);
/*    mainLayout->addLayout(buttonLayout);*/ 
    setLayout(mainLayout);

    setWindowTitle(tr("Kleingarten v0.5"));
}

void TableEditor::createContextMenu()
{
    QAction * deleteAct = new QAction(tr("Lösche Zeile"), this);
    mainView->addAction(deleteAct);
    mainView->setContextMenuPolicy(Qt::ActionsContextMenu);
    kontenView->setContextMenuPolicy(Qt::ActionsContextMenu);
    kategorienView->setContextMenuPolicy(Qt::ActionsContextMenu);

    QAction * previewAct = new QAction(tr("Vorschau"), this);
    QAction * printAct = new QAction(tr("Ducken"), this);
    kontenView->addAction(previewAct);
    kontenView->addAction(printAct);
    kategorienView->addAction(previewAct);
    kategorienView->addAction(printAct);
}

void TableEditor::currentTabChanged(int currentItem)
{
    switch(currentItem)
    {
	case 1:
	    stackedLayout->setCurrentIndex(1);
	    break;
	case 0:
	    stackedLayout->setCurrentIndex(0);
	    printDialog->setType(PrintDialog::Konto);
	    break;
	case 2:
	    stackedLayout->setCurrentIndex(0);
	    printDialog->setType(PrintDialog::Kategorie);
	    break;
    }
}

void TableEditor::resizeViewColumns()
{
    qDebug() << "in resizeViewColumns() ";
    resizeColumn(0, 75);
    resizeColumn(1, 75);
    resizeColumn(2, 60);
    resizeColumn(3, 60);
    resizeColumn(4, 250);
    resizeColumn(5, 60);
    resizeColumn(6, 60);
    resizeColumn(7, 140);
    resizeColumn(8, 160);
}

void TableEditor::resizeColumn(int column, int width)
{
    if(mainView->columnWidth(column) != width)
	mainView->setColumnWidth(column, width);
}

void TableEditor::kontenFilterActivated(int checkState)
{
    qDebug() << "kontenFilterActivated mit " << checkState 
        << " Referenz: Qt::Unchecked = " << Qt::Unchecked;
    if (checkState == Qt::Unchecked)
	mainModel->setKontoFilter("");
    else if (checkState == Qt::Checked)
	mainModel->setKontoFilter(kontenCombo->currentText());
}

void TableEditor::kategorienFilterActivated(int checkState)
{
    if (checkState == Qt::Unchecked)
	mainModel->setKategorieFilter("");
    else if (checkState == Qt::Checked)
	mainModel->setKategorieFilter(kategorienCombo->currentText());
}

void TableEditor::addSaison()
{
    if( QMessageBox::question(
		this,
        tr("Neue Saison..."),
		tr("Soll tatsächlich die Sasion %1 \n"
			"hinzugefügt werden?")
                .arg(QString::number(currentSaison + 1)),
                tr("&Ja"), tr("&Nein"),
                QString(), 0, 1) == 0 )
    {
	currentSaison = currentSaison + 1;
	saisonCombo->addItem(QString::number(currentSaison));
    }
}

TableEditor::~TableEditor()
{
/*     delete mainView;
    delete submitButton;
    delete revertButton;
    delete quitButton;
    delete mainModel;
    delete kontenModel;
    delete kategorienModel;
    delete stackedLayout;
    delete saisonCombo;
    delete kontenCombo;
    delete kategorienCombo;
    delete statusSumLabel;     */
}
