/********************************************************************************
** Form generated from reading ui file 'transaktion.ui'
**
** Created: Tue Dec 25 19:16:40 2007
**      by: Qt User Interface Compiler version 4.3.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TRANSAKTION_H
#define UI_TRANSAKTION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

class Ui_TransactionDialog
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QFrame *line;
    QComboBox *kontoCombo;
    QLabel *kontoLabel;

    void setupUi(QDialog *TransactionDialog)
    {
    if (TransactionDialog->objectName().isEmpty())
        TransactionDialog->setObjectName(QString::fromUtf8("TransactionDialog"));
    TransactionDialog->resize(589, 149);
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(TransactionDialog->sizePolicy().hasHeightForWidth());
    TransactionDialog->setSizePolicy(sizePolicy);
    TransactionDialog->setMinimumSize(QSize(585, 140));
    TransactionDialog->setBaseSize(QSize(585, 140));
    TransactionDialog->setFocusPolicy(Qt::StrongFocus);
    TransactionDialog->setSizeGripEnabled(false);
    TransactionDialog->setModal(true);
    layoutWidget = new QWidget(TransactionDialog);
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    layoutWidget->setGeometry(QRect(30, 90, 519, 33));
    hboxLayout = new QHBoxLayout(layoutWidget);
#ifndef Q_OS_MAC
    hboxLayout->setSpacing(6);
#endif
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    okButton = new QPushButton(layoutWidget);
    okButton->setObjectName(QString::fromUtf8("okButton"));

    hboxLayout->addWidget(okButton);

    cancelButton = new QPushButton(layoutWidget);
    cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

    hboxLayout->addWidget(cancelButton);

    line = new QFrame(TransactionDialog);
    line->setObjectName(QString::fromUtf8("line"));
    line->setGeometry(QRect(30, 70, 511, 20));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    kontoCombo = new QComboBox(TransactionDialog);
    kontoCombo->setObjectName(QString::fromUtf8("kontoCombo"));
    kontoCombo->setGeometry(QRect(28, 45, 521, 22));
    QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(0));
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(kontoCombo->sizePolicy().hasHeightForWidth());
    kontoCombo->setSizePolicy(sizePolicy1);
    kontoCombo->setEditable(true);
    kontoCombo->setInsertPolicy(QComboBox::NoInsert);
    kontoLabel = new QLabel(TransactionDialog);
    kontoLabel->setObjectName(QString::fromUtf8("kontoLabel"));
    kontoLabel->setGeometry(QRect(28, 22, 521, 20));
    QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(5));
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(kontoLabel->sizePolicy().hasHeightForWidth());
    kontoLabel->setSizePolicy(sizePolicy2);
    kontoLabel->setFrameShape(QFrame::StyledPanel);
    kontoLabel->setFrameShadow(QFrame::Raised);
    kontoLabel->setBuddy(kontoCombo);
    QWidget::setTabOrder(kontoCombo, okButton);
    QWidget::setTabOrder(okButton, cancelButton);

    retranslateUi(TransactionDialog);
    QObject::connect(okButton, SIGNAL(clicked()), TransactionDialog, SLOT(accept()));
    QObject::connect(cancelButton, SIGNAL(clicked()), TransactionDialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(TransactionDialog);
    } // setupUi

    void retranslateUi(QDialog *TransactionDialog)
    {
    TransactionDialog->setWindowTitle(QApplication::translate("TransactionDialog", "F\303\274ge Transaktion hinzu...", 0, QApplication::UnicodeUTF8));
    okButton->setText(QApplication::translate("TransactionDialog", "&OK", 0, QApplication::UnicodeUTF8));
    cancelButton->setText(QApplication::translate("TransactionDialog", "Abbreche&n", 0, QApplication::UnicodeUTF8));
    kontoLabel->setText(QApplication::translate("TransactionDialog", "&Kontoname", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(TransactionDialog);
    } // retranslateUi

};

namespace Ui {
    class TransactionDialog: public Ui_TransactionDialog {};
} // namespace Ui

#endif // UI_TRANSAKTION_H
