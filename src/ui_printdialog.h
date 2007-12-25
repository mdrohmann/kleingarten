/********************************************************************************
** Form generated from reading ui file 'printdialog.ui'
**
** Created: Tue Dec 25 19:16:40 2007
**      by: Qt User Interface Compiler version 4.3.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PRINTDIALOG_H
#define UI_PRINTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

class Ui_printDialog
{
public:
    QWidget *layoutWidget1;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *previewButton;
    QPushButton *printButton;
    QPushButton *cancelButton;
    QGroupBox *groupBox_2;
    QWidget *layoutWidget2;
    QGridLayout *gridLayout;
    QSpinBox *toSpinBox;
    QSpinBox *fromSpinBox;
    QLabel *label;
    QRadioButton *printSelection_allRadio;
    QRadioButton *printSelection_fromRadio;
    QGroupBox *printTypeGroup;
    QWidget *layoutWidget3;
    QGridLayout *gridLayout1;
    QComboBox *kategorienCombo;
    QComboBox *kontenCombo;
    QRadioButton *kategorienRadio;
    QRadioButton *kontenRadio;

    void setupUi(QDialog *printDialog)
    {
    if (printDialog->objectName().isEmpty())
        printDialog->setObjectName(QString::fromUtf8("printDialog"));
    printDialog->resize(616, 207);
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(1));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(printDialog->sizePolicy().hasHeightForWidth());
    printDialog->setSizePolicy(sizePolicy);
    printDialog->setMinimumSize(QSize(610, 200));
    printDialog->setModal(true);
    layoutWidget1 = new QWidget(printDialog);
    layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
    layoutWidget1->setGeometry(QRect(20, 150, 581, 33));
    hboxLayout = new QHBoxLayout(layoutWidget1);
#ifndef Q_OS_MAC
    hboxLayout->setSpacing(6);
#endif
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    spacerItem = new QSpacerItem(120, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    previewButton = new QPushButton(layoutWidget1);
    previewButton->setObjectName(QString::fromUtf8("previewButton"));

    hboxLayout->addWidget(previewButton);

    printButton = new QPushButton(layoutWidget1);
    printButton->setObjectName(QString::fromUtf8("printButton"));

    hboxLayout->addWidget(printButton);

    cancelButton = new QPushButton(layoutWidget1);
    cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

    hboxLayout->addWidget(cancelButton);

    groupBox_2 = new QGroupBox(printDialog);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    groupBox_2->setGeometry(QRect(400, 20, 201, 121));
    layoutWidget2 = new QWidget(groupBox_2);
    layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
    layoutWidget2->setGeometry(QRect(10, 20, 181, 91));
    gridLayout = new QGridLayout(layoutWidget2);
#ifndef Q_OS_MAC
    gridLayout->setSpacing(6);
#endif
    gridLayout->setMargin(0);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);
    toSpinBox = new QSpinBox(layoutWidget2);
    toSpinBox->setObjectName(QString::fromUtf8("toSpinBox"));
    toSpinBox->setEnabled(false);
    toSpinBox->setMinimum(1);
    toSpinBox->setValue(1);

    gridLayout->addWidget(toSpinBox, 1, 3, 1, 1);

    fromSpinBox = new QSpinBox(layoutWidget2);
    fromSpinBox->setObjectName(QString::fromUtf8("fromSpinBox"));
    fromSpinBox->setEnabled(false);
    fromSpinBox->setMinimum(1);
    fromSpinBox->setValue(1);

    gridLayout->addWidget(fromSpinBox, 1, 1, 1, 1);

    label = new QLabel(layoutWidget2);
    label->setObjectName(QString::fromUtf8("label"));
    label->setEnabled(false);

    gridLayout->addWidget(label, 1, 2, 1, 1);

    printSelection_allRadio = new QRadioButton(layoutWidget2);
    printSelection_allRadio->setObjectName(QString::fromUtf8("printSelection_allRadio"));
    printSelection_allRadio->setChecked(true);

    gridLayout->addWidget(printSelection_allRadio, 0, 0, 1, 2);

    printSelection_fromRadio = new QRadioButton(layoutWidget2);
    printSelection_fromRadio->setObjectName(QString::fromUtf8("printSelection_fromRadio"));

    gridLayout->addWidget(printSelection_fromRadio, 1, 0, 1, 1);

    printTypeGroup = new QGroupBox(printDialog);
    printTypeGroup->setObjectName(QString::fromUtf8("printTypeGroup"));
    printTypeGroup->setGeometry(QRect(20, 20, 351, 121));
    printTypeGroup->setCheckable(false);
    layoutWidget3 = new QWidget(printTypeGroup);
    layoutWidget3->setObjectName(QString::fromUtf8("layoutWidget3"));
    layoutWidget3->setGeometry(QRect(11, 21, 331, 91));
    gridLayout1 = new QGridLayout(layoutWidget3);
#ifndef Q_OS_MAC
    gridLayout1->setSpacing(6);
#endif
    gridLayout1->setMargin(0);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    gridLayout1->setContentsMargins(0, 0, 0, 0);
    kategorienCombo = new QComboBox(layoutWidget3);
    kategorienCombo->setObjectName(QString::fromUtf8("kategorienCombo"));
    kategorienCombo->setEnabled(false);
    QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy1.setHorizontalStretch(1);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(kategorienCombo->sizePolicy().hasHeightForWidth());
    kategorienCombo->setSizePolicy(sizePolicy1);
    kategorienCombo->setEditable(true);
    kategorienCombo->setInsertPolicy(QComboBox::NoInsert);

    gridLayout1->addWidget(kategorienCombo, 1, 1, 1, 1);

    kontenCombo = new QComboBox(layoutWidget3);
    kontenCombo->setObjectName(QString::fromUtf8("kontenCombo"));
    sizePolicy1.setHeightForWidth(kontenCombo->sizePolicy().hasHeightForWidth());
    kontenCombo->setSizePolicy(sizePolicy1);
    kontenCombo->setEditable(true);
    kontenCombo->setInsertPolicy(QComboBox::NoInsert);

    gridLayout1->addWidget(kontenCombo, 0, 1, 1, 1);

    kategorienRadio = new QRadioButton(layoutWidget3);
    kategorienRadio->setObjectName(QString::fromUtf8("kategorienRadio"));
    QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(kategorienRadio->sizePolicy().hasHeightForWidth());
    kategorienRadio->setSizePolicy(sizePolicy2);

    gridLayout1->addWidget(kategorienRadio, 1, 0, 1, 1);

    kontenRadio = new QRadioButton(layoutWidget3);
    kontenRadio->setObjectName(QString::fromUtf8("kontenRadio"));
    sizePolicy2.setHeightForWidth(kontenRadio->sizePolicy().hasHeightForWidth());
    kontenRadio->setSizePolicy(sizePolicy2);
    kontenRadio->setChecked(true);

    gridLayout1->addWidget(kontenRadio, 0, 0, 1, 1);

    label->setBuddy(toSpinBox);
    QWidget::setTabOrder(kontenRadio, kontenCombo);
    QWidget::setTabOrder(kontenCombo, kategorienRadio);
    QWidget::setTabOrder(kategorienRadio, kategorienCombo);
    QWidget::setTabOrder(kategorienCombo, printSelection_allRadio);
    QWidget::setTabOrder(printSelection_allRadio, printSelection_fromRadio);
    QWidget::setTabOrder(printSelection_fromRadio, fromSpinBox);
    QWidget::setTabOrder(fromSpinBox, toSpinBox);
    QWidget::setTabOrder(toSpinBox, previewButton);
    QWidget::setTabOrder(previewButton, printButton);
    QWidget::setTabOrder(printButton, cancelButton);

    retranslateUi(printDialog);
    QObject::connect(cancelButton, SIGNAL(clicked()), printDialog, SLOT(reject()));
    QObject::connect(kontenRadio, SIGNAL(toggled(bool)), kontenCombo, SLOT(setEnabled(bool)));
    QObject::connect(kategorienRadio, SIGNAL(toggled(bool)), kategorienCombo, SLOT(setEnabled(bool)));
    QObject::connect(printSelection_fromRadio, SIGNAL(toggled(bool)), fromSpinBox, SLOT(setEnabled(bool)));
    QObject::connect(printSelection_fromRadio, SIGNAL(toggled(bool)), toSpinBox, SLOT(setEnabled(bool)));
    QObject::connect(printSelection_fromRadio, SIGNAL(toggled(bool)), label, SLOT(setEnabled(bool)));

    QMetaObject::connectSlotsByName(printDialog);
    } // setupUi

    void retranslateUi(QDialog *printDialog)
    {
    printDialog->setWindowTitle(QApplication::translate("printDialog", "Drucken/Vorschau...", 0, QApplication::UnicodeUTF8));
    previewButton->setText(QApplication::translate("printDialog", "Vorschau...", 0, QApplication::UnicodeUTF8));
    printButton->setText(QApplication::translate("printDialog", "Drucken...", 0, QApplication::UnicodeUTF8));
    cancelButton->setText(QApplication::translate("printDialog", "Abbrechen", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("printDialog", "Druckbereich", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("printDialog", "bis", 0, QApplication::UnicodeUTF8));
    printSelection_allRadio->setText(QApplication::translate("printDialog", "Alles", 0, QApplication::UnicodeUTF8));
    printSelection_fromRadio->setText(QApplication::translate("printDialog", "Von", 0, QApplication::UnicodeUTF8));
    printTypeGroup->setTitle(QApplication::translate("printDialog", "W\303\244hle Ausdruck", 0, QApplication::UnicodeUTF8));
    kategorienRadio->setText(QApplication::translate("printDialog", "Kategorien", 0, QApplication::UnicodeUTF8));
    kontenRadio->setText(QApplication::translate("printDialog", "Konten", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(printDialog);
    } // retranslateUi

};

namespace Ui {
    class printDialog: public Ui_printDialog {};
} // namespace Ui

#endif // UI_PRINTDIALOG_H
