/********************************************************************************
** Form generated from reading UI file 'outputwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OUTPUTWIDGET_H
#define UI_OUTPUTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OutputWidget
{
public:
    QVBoxLayout *verticalLayout;
    QToolButton *btnTest;
    QRadioButton *rbBlockOccupancy;
    QCheckBox *chkBlinking;
    QRadioButton *rb1On;
    QRadioButton *rb1Off;
    QRadioButton *rb2On;
    QRadioButton *rb2Off;
    QCheckBox *chk4WayPort;
    QRadioButton *rb1SoftReset;
    QRadioButton *rb1HardReset;
    QRadioButton *rb2SoftReset;
    QRadioButton *rb2HardReset;

    void setupUi(QWidget *OutputWidget)
    {
        if (OutputWidget->objectName().isEmpty())
            OutputWidget->setObjectName(QLatin1String("OutputWidget"));
        OutputWidget->resize(30, 342);
        OutputWidget->setMaximumSize(QSize(50, 16777215));
        QFont font;
        font.setFamily(QLatin1String("Vemana2000"));
        font.setPointSize(7);
        OutputWidget->setFont(font);
        verticalLayout = new QVBoxLayout(OutputWidget);
        verticalLayout->setSpacing(2);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QLatin1String("verticalLayout"));
        btnTest = new QToolButton(OutputWidget);
        btnTest->setObjectName(QLatin1String("btnTest"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnTest->sizePolicy().hasHeightForWidth());
        btnTest->setSizePolicy(sizePolicy);
        btnTest->setMaximumSize(QSize(30, 16777215));
        QFont font1;
        font1.setFamily(QLatin1String("Ubuntu"));
        btnTest->setFont(font1);

        verticalLayout->addWidget(btnTest);

        rbBlockOccupancy = new QRadioButton(OutputWidget);
        rbBlockOccupancy->setObjectName(QLatin1String("rbBlockOccupancy"));
        rbBlockOccupancy->setMaximumSize(QSize(30, 16777215));

        verticalLayout->addWidget(rbBlockOccupancy);

        chkBlinking = new QCheckBox(OutputWidget);
        chkBlinking->setObjectName(QLatin1String("chkBlinking"));

        verticalLayout->addWidget(chkBlinking);

        rb1On = new QRadioButton(OutputWidget);
        rb1On->setObjectName(QLatin1String("rb1On"));

        verticalLayout->addWidget(rb1On);

        rb1Off = new QRadioButton(OutputWidget);
        rb1Off->setObjectName(QLatin1String("rb1Off"));

        verticalLayout->addWidget(rb1Off);

        rb2On = new QRadioButton(OutputWidget);
        rb2On->setObjectName(QLatin1String("rb2On"));

        verticalLayout->addWidget(rb2On);

        rb2Off = new QRadioButton(OutputWidget);
        rb2Off->setObjectName(QLatin1String("rb2Off"));

        verticalLayout->addWidget(rb2Off);

        chk4WayPort = new QCheckBox(OutputWidget);
        chk4WayPort->setObjectName(QLatin1String("chk4WayPort"));

        verticalLayout->addWidget(chk4WayPort);

        rb1SoftReset = new QRadioButton(OutputWidget);
        rb1SoftReset->setObjectName(QLatin1String("rb1SoftReset"));

        verticalLayout->addWidget(rb1SoftReset);

        rb1HardReset = new QRadioButton(OutputWidget);
        rb1HardReset->setObjectName(QLatin1String("rb1HardReset"));

        verticalLayout->addWidget(rb1HardReset);

        rb2SoftReset = new QRadioButton(OutputWidget);
        rb2SoftReset->setObjectName(QLatin1String("rb2SoftReset"));

        verticalLayout->addWidget(rb2SoftReset);

        rb2HardReset = new QRadioButton(OutputWidget);
        rb2HardReset->setObjectName(QLatin1String("rb2HardReset"));

        verticalLayout->addWidget(rb2HardReset);


        retranslateUi(OutputWidget);

        QMetaObject::connectSlotsByName(OutputWidget);
    } // setupUi

    void retranslateUi(QWidget *OutputWidget)
    {
        OutputWidget->setWindowTitle(QApplication::translate("OutputWidget", "Form", nullptr));
#ifndef QT_NO_TOOLTIP
        btnTest->setToolTip(QApplication::translate("OutputWidget", "<html><head/><body><p>Generate test message.</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        btnTest->setText(QApplication::translate("OutputWidget", "On", nullptr));
#ifndef QT_NO_TOOLTIP
        rbBlockOccupancy->setToolTip(QApplication::translate("OutputWidget", "<html><head/><body><p>Block occupancy.</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        rbBlockOccupancy->setText(QString());
#ifndef QT_NO_TOOLTIP
        chkBlinking->setToolTip(QApplication::translate("OutputWidget", "<html><head/><body><p>Check to make output blink at blink rate.</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        chkBlinking->setText(QString());
#ifndef QT_NO_TOOLTIP
        rb1On->setToolTip(QApplication::translate("OutputWidget", "<html><head/><body><p>Push button on at power up.</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        rb1On->setText(QString());
#ifndef QT_NO_TOOLTIP
        rb1Off->setToolTip(QApplication::translate("OutputWidget", "<html><head/><body><p>Push button off at power up.</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        rb1Off->setText(QString());
#ifndef QT_NO_TOOLTIP
        rb2On->setToolTip(QApplication::translate("OutputWidget", "<html><head/><body><p>Second output on at power up.</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        rb2On->setText(QString());
#ifndef QT_NO_TOOLTIP
        rb2Off->setToolTip(QApplication::translate("OutputWidget", "<html><head/><body><p>Second output off at power up. </p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        rb2Off->setText(QString());
#ifndef QT_NO_TOOLTIP
        chk4WayPort->setToolTip(QApplication::translate("OutputWidget", "<html><head/><body><p>Check for 4 way output.</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        chk4WayPort->setText(QString());
#ifndef QT_NO_TOOLTIP
        rb1SoftReset->setToolTip(QApplication::translate("OutputWidget", "<html><head/><body><p>Software pulse output.</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        rb1SoftReset->setText(QString());
#ifndef QT_NO_TOOLTIP
        rb1HardReset->setToolTip(QApplication::translate("OutputWidget", "<html><head/><body><p>Hardware controlled pulse output. </p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        rb1HardReset->setText(QString());
#ifndef QT_NO_TOOLTIP
        rb2SoftReset->setToolTip(QApplication::translate("OutputWidget", "<html><head/><body><p>Paired software controlled pulse output.</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        rb2SoftReset->setText(QString());
#ifndef QT_NO_TOOLTIP
        rb2HardReset->setToolTip(QApplication::translate("OutputWidget", "<html><head/><body><p>Paired hardware controlled pulse output. </p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        rb2HardReset->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class OutputWidget: public Ui_OutputWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OUTPUTWIDGET_H
