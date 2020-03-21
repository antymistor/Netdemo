/********************************************************************************
** Form generated from reading UI file 'Netdemo.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NETDEMO_H
#define UI_NETDEMO_H

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

class Ui_NetdemoClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *NetdemoClass)
    {
        if (NetdemoClass->objectName().isEmpty())
            NetdemoClass->setObjectName(QStringLiteral("NetdemoClass"));
        NetdemoClass->resize(600, 400);
        menuBar = new QMenuBar(NetdemoClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        NetdemoClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(NetdemoClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        NetdemoClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(NetdemoClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        NetdemoClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(NetdemoClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        NetdemoClass->setStatusBar(statusBar);

        retranslateUi(NetdemoClass);

        QMetaObject::connectSlotsByName(NetdemoClass);
    } // setupUi

    void retranslateUi(QMainWindow *NetdemoClass)
    {
        NetdemoClass->setWindowTitle(QApplication::translate("NetdemoClass", "Netdemo", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class NetdemoClass: public Ui_NetdemoClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NETDEMO_H
