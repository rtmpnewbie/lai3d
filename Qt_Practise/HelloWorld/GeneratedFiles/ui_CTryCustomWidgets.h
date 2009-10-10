/********************************************************************************
** Form generated from reading ui file 'CTryCustomWidgets.ui'
**
** Created: Tue Mar 4 00:55:59 2008
**      by: Qt User Interface Compiler version 4.3.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CTRYCUSTOMWIDGETS_H
#define UI_CTRYCUSTOMWIDGETS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCalendarWidget>
#include <QtGui/QLCDNumber>
#include <QtGui/QWidget>
#include "analogclock.h"

class Ui_CTryCustomWidgetsClass
{
public:
    QCalendarWidget *calendarWidget;
    AnalogClock *analogClock;
    QLCDNumber *lcdNumber;

    void setupUi(QWidget *CTryCustomWidgetsClass)
    {
    if (CTryCustomWidgetsClass->objectName().isEmpty())
        CTryCustomWidgetsClass->setObjectName(QString::fromUtf8("CTryCustomWidgetsClass"));
    QSize size(516, 322);
    size = size.expandedTo(CTryCustomWidgetsClass->minimumSizeHint());
    CTryCustomWidgetsClass->resize(size);
    calendarWidget = new QCalendarWidget(CTryCustomWidgetsClass);
    calendarWidget->setObjectName(QString::fromUtf8("calendarWidget"));
    calendarWidget->setGeometry(QRect(0, 0, 344, 155));
    analogClock = new AnalogClock(CTryCustomWidgetsClass);
    analogClock->setObjectName(QString::fromUtf8("analogClock"));
    analogClock->setGeometry(QRect(380, 20, 100, 100));
    lcdNumber = new QLCDNumber(CTryCustomWidgetsClass);
    lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
    lcdNumber->setGeometry(QRect(40, 180, 391, 121));
    lcdNumber->setFrameShape(QFrame::Box);
    lcdNumber->setFrameShadow(QFrame::Raised);
    lcdNumber->setSmallDecimalPoint(true);
    lcdNumber->setSegmentStyle(QLCDNumber::Filled);
    lcdNumber->setProperty("intValue", QVariant(12345));

    retranslateUi(CTryCustomWidgetsClass);

    QMetaObject::connectSlotsByName(CTryCustomWidgetsClass);
    } // setupUi

    void retranslateUi(QWidget *CTryCustomWidgetsClass)
    {
    CTryCustomWidgetsClass->setWindowTitle(QApplication::translate("CTryCustomWidgetsClass", "CTryCustomWidgets", 0, QApplication::UnicodeUTF8));
    analogClock->setToolTip(QApplication::translate("CTryCustomWidgetsClass", "The current time", 0, QApplication::UnicodeUTF8));
    analogClock->setWhatsThis(QApplication::translate("CTryCustomWidgetsClass", "The analog clock widget displays the current time.", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CTryCustomWidgetsClass);
    } // retranslateUi

};

namespace Ui {
    class CTryCustomWidgetsClass: public Ui_CTryCustomWidgetsClass {};
} // namespace Ui

#endif // UI_CTRYCUSTOMWIDGETS_H
