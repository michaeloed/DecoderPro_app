/********************************************************************************
** Form generated from reading UI file 'boosterinputwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BOOSTERINPUTWIDGET_H
#define UI_BOOSTERINPUTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BoosterInputWidget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lblfiller;
    QLineEdit *edStatus;
    QRadioButton *rbBlockDetectionActiveLow;
    QLabel *lblfiller_2;
    QLabel *lblfiller_3;
    QLabel *lblfiller_4;
    QLabel *lblfiller_5;
    QLabel *lblfiller_6;
    QLabel *lblfiller_7;
    QLabel *lblfiller_8;
    QLabel *lblfiller_10;
    QLabel *lblfiller_9;

    void setupUi(QWidget *BoosterInputWidget)
    {
        if (BoosterInputWidget->objectName().isEmpty())
            BoosterInputWidget->setObjectName(QLatin1String("BoosterInputWidget"));
        BoosterInputWidget->resize(20, 317);
        verticalLayout = new QVBoxLayout(BoosterInputWidget);
        verticalLayout->setSpacing(2);
        verticalLayout->setObjectName(QLatin1String("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lblfiller = new QLabel(BoosterInputWidget);
        lblfiller->setObjectName(QLatin1String("lblfiller"));
        lblfiller->setMaximumSize(QSize(16777215, 20));

        verticalLayout->addWidget(lblfiller);

        edStatus = new QLineEdit(BoosterInputWidget);
        edStatus->setObjectName(QLatin1String("edStatus"));
        edStatus->setMaximumSize(QSize(20, 20));
        edStatus->setReadOnly(true);

        verticalLayout->addWidget(edStatus);

        rbBlockDetectionActiveLow = new QRadioButton(BoosterInputWidget);
        rbBlockDetectionActiveLow->setObjectName(QLatin1String("rbBlockDetectionActiveLow"));
        rbBlockDetectionActiveLow->setChecked(true);

        verticalLayout->addWidget(rbBlockDetectionActiveLow);

        lblfiller_2 = new QLabel(BoosterInputWidget);
        lblfiller_2->setObjectName(QLatin1String("lblfiller_2"));
        lblfiller_2->setMaximumSize(QSize(16777215, 20));

        verticalLayout->addWidget(lblfiller_2);

        lblfiller_3 = new QLabel(BoosterInputWidget);
        lblfiller_3->setObjectName(QLatin1String("lblfiller_3"));
        lblfiller_3->setMaximumSize(QSize(16777215, 20));

        verticalLayout->addWidget(lblfiller_3);

        lblfiller_4 = new QLabel(BoosterInputWidget);
        lblfiller_4->setObjectName(QLatin1String("lblfiller_4"));
        lblfiller_4->setMaximumSize(QSize(16777215, 20));

        verticalLayout->addWidget(lblfiller_4);

        lblfiller_5 = new QLabel(BoosterInputWidget);
        lblfiller_5->setObjectName(QLatin1String("lblfiller_5"));
        lblfiller_5->setMaximumSize(QSize(16777215, 20));

        verticalLayout->addWidget(lblfiller_5);

        lblfiller_6 = new QLabel(BoosterInputWidget);
        lblfiller_6->setObjectName(QLatin1String("lblfiller_6"));
        lblfiller_6->setMaximumSize(QSize(16777215, 20));

        verticalLayout->addWidget(lblfiller_6);

        lblfiller_7 = new QLabel(BoosterInputWidget);
        lblfiller_7->setObjectName(QLatin1String("lblfiller_7"));
        lblfiller_7->setMaximumSize(QSize(16777215, 20));

        verticalLayout->addWidget(lblfiller_7);

        lblfiller_8 = new QLabel(BoosterInputWidget);
        lblfiller_8->setObjectName(QLatin1String("lblfiller_8"));
        lblfiller_8->setMaximumSize(QSize(16777215, 20));

        verticalLayout->addWidget(lblfiller_8);

        lblfiller_10 = new QLabel(BoosterInputWidget);
        lblfiller_10->setObjectName(QLatin1String("lblfiller_10"));
        lblfiller_10->setMaximumSize(QSize(16777215, 20));

        verticalLayout->addWidget(lblfiller_10);

        lblfiller_9 = new QLabel(BoosterInputWidget);
        lblfiller_9->setObjectName(QLatin1String("lblfiller_9"));
        lblfiller_9->setMaximumSize(QSize(16777215, 20));

        verticalLayout->addWidget(lblfiller_9);


        retranslateUi(BoosterInputWidget);

        QMetaObject::connectSlotsByName(BoosterInputWidget);
    } // setupUi

    void retranslateUi(QWidget *BoosterInputWidget)
    {
        BoosterInputWidget->setWindowTitle(QApplication::translate("BoosterInputWidget", "Form", nullptr));
        lblfiller->setText(QString());
#ifndef QT_NO_TOOLTIP
        rbBlockDetectionActiveLow->setToolTip(QApplication::translate("BoosterInputWidget", "<html><head/><body><p>Block detection active high.  Generate an OPC_INPUT_REP (0xB2) message.</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        rbBlockDetectionActiveLow->setText(QString());
        lblfiller_2->setText(QString());
        lblfiller_3->setText(QString());
        lblfiller_4->setText(QString());
        lblfiller_5->setText(QString());
        lblfiller_6->setText(QString());
        lblfiller_7->setText(QString());
        lblfiller_8->setText(QString());
        lblfiller_10->setText(QString());
        lblfiller_9->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class BoosterInputWidget: public Ui_BoosterInputWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOOSTERINPUTWIDGET_H
