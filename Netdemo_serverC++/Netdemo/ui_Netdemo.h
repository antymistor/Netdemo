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
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QFontDatabase>
#include <QGridLayout>
#include <QtCharts/QChart>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QTimer>
#include <QDateTime>
QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE


class Ui_Netdemo
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;
	
	QWidget *main_widget, *left_widget, *right_widget;
	QGridLayout *main_layout, *left_layout, *right_layout;
	QLineEdit *localaddress, *filepath;
	QLabel *label1, *label2, *label3 ,*label4;
	QComboBox *combo;
	QPushButton *start,*send;
	QTextEdit *textsent;
	QLineEdit *texttosent;

	//left
	QTabWidget *leftQtab;
	//Chart part
	QChart *chart;
	QChartView *chartView;
	QLineSeries *line1, *line2;
	
	//Text Part
	QWidget *Text_B;
	QGridLayout *Text_B_Layout;
	QLineEdit *toaddress;
	QTextEdit *text;
	QPushButton *stop,*copy,*clear;

	//video part
	QWidget *video;
	QGridLayout *video_Layout;
	QLabel *videolabel;
	QPushButton *stop2, *copy2, *Cal2;
	QLineEdit *rtsp;
	void setupUi(QMainWindow *NetdemoClass); // setupUi

};

QT_END_NAMESPACE

#endif // UI_NETDEMO_H
