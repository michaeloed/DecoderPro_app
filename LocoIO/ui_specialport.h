/********************************************************************************
** Form generated from reading UI file 'specialport.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPECIALPORT_H
#define UI_SPECIALPORT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SpecialPort
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lblInput;
    QRadioButton *rbInput;
    QLabel *lblFiller;
    QRadioButton *rbOutput;
    QLabel *lblOutput;

    void setupUi(QWidget *SpecialPort)
    {
        if (SpecialPort->objectName().isEmpty())
            SpecialPort->setObjectName(QLatin1String("SpecialPort"));
        SpecialPort->resize(22, 405);
        QFont font;
        font.setPointSize(7);
        SpecialPort->setFont(font);
        verticalLayout = new QVBoxLayout(SpecialPort);
        verticalLayout->setSpacing(2);
        verticalLayout->setObjectName(QLatin1String("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lblInput = new QLabel(SpecialPort);
        lblInput->setObjectName(QLatin1String("lblInput"));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(255, 192, 203, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        lblInput->setPalette(palette);
        QFont font1;
        font1.setFamily(QLatin1String("Monospace"));
        font1.setBold(true);
        font1.setItalic(false);
        font1.setWeight(75);
        lblInput->setFont(font1);
        lblInput->setLayoutDirection(Qt::RightToLeft);
        lblInput->setAutoFillBackground(false);
        lblInput->setStyleSheet(QLatin1String("QLabel { background-color : pink; color : black; }"));
        lblInput->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblInput);

        rbInput = new QRadioButton(SpecialPort);
        rbInput->setObjectName(QLatin1String("rbInput"));
        rbInput->setChecked(true);

        verticalLayout->addWidget(rbInput);

        lblFiller = new QLabel(SpecialPort);
        lblFiller->setObjectName(QLatin1String("lblFiller"));

        verticalLayout->addWidget(lblFiller);

        rbOutput = new QRadioButton(SpecialPort);
        rbOutput->setObjectName(QLatin1String("rbOutput"));

        verticalLayout->addWidget(rbOutput);

        lblOutput = new QLabel(SpecialPort);
        lblOutput->setObjectName(QLatin1String("lblOutput"));
        QFont font2;
        font2.setFamily(QLatin1String("Monospace"));
        font2.setBold(true);
        font2.setWeight(75);
        lblOutput->setFont(font2);
        lblOutput->setStyleSheet(QLatin1String("QLabel { background-color : yellow; color : black; }"));
        lblOutput->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblOutput);


        retranslateUi(SpecialPort);

        QMetaObject::connectSlotsByName(SpecialPort);
    } // setupUi

    void retranslateUi(QWidget *SpecialPort)
    {
        SpecialPort->setWindowTitle(QApplication::translate("SpecialPort", "Form", nullptr));
        lblInput->setText(QApplication::translate("SpecialPort", "I\n"
"D\n"
"L\n"
"E\n"
"", nullptr));
        rbInput->setText(QString());
        lblFiller->setText(QString());
        rbOutput->setText(QString());
        lblOutput->setText(QApplication::translate("SpecialPort", "S\n"
"T\n"
"A\n"
"T\n"
"U\n"
"S\n"
"", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SpecialPort: public Ui_SpecialPort {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPECIALPORT_H
