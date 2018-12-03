/********************************************************************************
** Form generated from reading UI file 'osgreadlas.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OSGREADLAS_H
#define UI_OSGREADLAS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OSGReadLasClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *OSGReadLasClass)
    {
        if (OSGReadLasClass->objectName().isEmpty())
            OSGReadLasClass->setObjectName(QStringLiteral("OSGReadLasClass"));
        OSGReadLasClass->resize(600, 400);
        menuBar = new QMenuBar(OSGReadLasClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        OSGReadLasClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(OSGReadLasClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        OSGReadLasClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(OSGReadLasClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        OSGReadLasClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(OSGReadLasClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        OSGReadLasClass->setStatusBar(statusBar);

        retranslateUi(OSGReadLasClass);

        QMetaObject::connectSlotsByName(OSGReadLasClass);
    } // setupUi

    void retranslateUi(QMainWindow *OSGReadLasClass)
    {
        OSGReadLasClass->setWindowTitle(QApplication::translate("OSGReadLasClass", "OSGReadLas", 0));
    } // retranslateUi

};

namespace Ui {
    class OSGReadLasClass: public Ui_OSGReadLasClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OSGREADLAS_H
