#include"ui_Netdemo.h"
void Ui_Netdemo::setupUi(QMainWindow *Qttest2Class) {
	//int fontId = QFontDatabase::addApplicationFont("D:\\c++test\\Qttest2\\Qttest2\\Qttest2\\font-awesome-4.7.0\\font-awesome-4.7.0\\fonts\\fontawesome-webfont.ttf");
	int fontId = QFontDatabase::addApplicationFont("fontawesome-webfont.ttf");
	Qttest2Class->setWindowIcon(QIcon("virus2.ico"));
	QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
	QFont font;
	font.setFamily(fontFamilies.at(0));
	font.setPointSize(11);
	//base set
	if (Qttest2Class->objectName().isEmpty())
		Qttest2Class->setObjectName(QStringLiteral("Qttest2Class"));
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication::setStyle("Fusion");
	Qttest2Class->setWindowOpacity(0.9);
	Qttest2Class->setAttribute(Qt::WA_TranslucentBackground);
	Qttest2Class->setBaseSize(900, 560);
	main_widget = new QWidget(Qttest2Class);
	main_widget->setObjectName(QStringLiteral("main_widget"));
	main_layout = new QGridLayout(main_widget);
	main_layout->setObjectName(QStringLiteral("main_layout"));
	left_widget = new QWidget();
	left_widget->setObjectName(QStringLiteral("left_widget"));
	left_layout = new QGridLayout(left_widget);
	left_layout->setObjectName(QStringLiteral("left_layout"));
	right_widget = new QWidget();
	right_widget->setObjectName(QStringLiteral("right_widget"));
	right_layout = new QGridLayout(right_widget);
	right_layout->setObjectName(QStringLiteral("right_layout"));
	main_layout->addWidget(left_widget, 0, 0, 10, 6);
	main_layout->addWidget(right_widget, 0, 7, 10, 2);
	Qttest2Class->setCentralWidget(main_widget);
	Qttest2Class->setBaseSize(900, 560);
	//rightset
	localaddress = new QLineEdit();
	filepath = new QLineEdit();
	localaddress->setText("192.168.1.190:6000");
	localaddress->setAlignment(Qt::AlignCenter);
	filepath->setText("FileGotten");
	filepath->setAlignment(Qt::AlignCenter);
	localaddress->setObjectName(QStringLiteral("inputtext"));
	filepath->setObjectName(QStringLiteral("inputtext"));
	label1 = new QLabel();
	label1->setText("Local IP Address : Port");
	label2 = new QLabel();
	label2->setText("File Save Path");
	label3 = new QLabel();
	label3->setText("Communication Protocol");
	combo = new QComboBox();
	QStringList combolist;
	combolist.append("UDP");
	combolist.append("TCP");
	combo->addItems(combolist);
	start = new QPushButton();
	start->setObjectName(QStringLiteral("inputtext"));
	start->setStyleSheet("QPushButton{background:rgb(223,109,35);\
			                  border-top-right-radius: 10px;\
                              border-bottom-right-radius: 10px;\
                              height:24px;}\
                              QPushButton:hover{ background:red; }");
	label4 = new QLabel();
	label4->setText("Command");
	label4->setObjectName(QStringLiteral("datasent"));
	textsent = new QTextEdit();
	texttosent = new QLineEdit("Hello Netdemo!");
	send = new QPushButton(); send->setFont(font); send->setText(QChar(0xf1d8));   //send;
	send->setObjectName(QStringLiteral("send"));
	send->setStyleSheet("QPushButton{background:rgb(51,51,51);\
                             border-bottom-right-radius: 10px;\
                             color:white;\
                             height:24px;}\
                             QPushButton:hover{ background:black; }\
                             QPushButton:pressed{ background:rgb(170,170,170); }");
	right_layout->addWidget(localaddress, 1, 0, 1, 5);
	right_layout->addWidget(filepath, 3, 0, 1, 5);
	right_layout->addWidget(combo, 5, 0, 1, 4);
	right_layout->addWidget(start, 5, 4, 1, 1);
	right_layout->addWidget(label1, 0, 0, 1, 5);
	right_layout->addWidget(label2, 2, 0, 1, 5);
	right_layout->addWidget(label3, 4, 0, 1, 5);
	right_layout->addWidget(label4, 6, 0, 1, 5);
	right_layout->addWidget(textsent, 7, 0, 8, 5);
	right_layout->addWidget(texttosent, 18, 0, 1, 4);
	right_layout->addWidget(send, 18, 4, 1, 1);
	right_widget->setStyleSheet("QLineEdit#inputtext{width:160px; border-radius:10px; padding:2px 4px; font-family:Arial; height:20px;}\
			                         QLineEdit{color:white;width:160px;font-size:17px;font-family:Arial;border-bottom-left-radius:10px;min-height:25px;background:rgb(51,51,51);}\
			                         QComboBox{width:160px;font-family:Arial;font-style:italic;border-radius:10px;padding:2px;}\
		                             QLabel{color:rgb(225,255,255,255);font-size:17px;font-weight:normal;font-family:Arial;}\
                                     QLabel#datasent{margin-top:50px;color:rgb(225,255,255,255);font-size:17px;font-weight:normal;font-family:Arial;}\
				                     QTextEdit{background-color:rgb(51,51,51);color:white;font-size:17px;font-family:Arial;border-top-left-radius:10px;border-top-right-radius:10px;}\
			                         QScrollBar{background:black; width:15; }\
			                         QScrollBar::handle:vertical{background: white; min-height:20px; border-radius:5px; }\
			                         QScrollBar::sub-line:vertical{ background:black; }");
	//left
	leftQtab = new QTabWidget();
	left_layout->addWidget(leftQtab);
	//Chart part

	chart = new QChart();
	chartView = new QChartView(chart);
	line1 = new QLineSeries();
	line2 = new QLineSeries();
	//chart->setTitle("Feedback");
	chart->setTitleFont(QFont("Î¢ÈíÑÅºÚ", 10));
	chart->addSeries(line1);
	chart->addSeries(line2);
	line1->setName("Data-1");
	line2->setName("Data-2");
	chart->legend()->setAlignment(Qt::AlignTop);
	chart->setTheme(QChart::ChartThemeDark);
	chart->setBackgroundVisible(false);
	chartView->setStyleSheet("QGraphicsView{ background-color:black;min-width:700px;min-height:300px;}");
	chart->createDefaultAxes();
	chart->axisX()->setTitleText("time/s");
	chart->axisY()->setTitleText("Val");
	chart->axisX()->setRange(0,1);
	chart->axisY()->setRange(-1, 1);
	chart->axisX()->setGridLineVisible(false);
	chart->axisY()->setGridLineVisible(false);
	//chartView->setRenderHint(QPainter::Antialiasing);
	
	leftQtab->addTab(chartView, "Chart");

	//Textpart
	Text_B = new QWidget();
	Text_B_Layout = new QGridLayout();
	text = new QTextEdit();
	toaddress = new QLineEdit();
	toaddress->setText("192.168.1.199:5000");
	toaddress->setStyleSheet("color:white;width:160px;font-size:17px;font-family:Arial;border-radius: 10px;min-height:25px;background:rgb(51,51,51);text-align:center;");
	toaddress->setAlignment(Qt::AlignCenter);
	stop = new QPushButton(); stop->setFont(font); stop->setText(QChar(0xf04d));    //stop
	copy = new QPushButton(); copy->setFont(font); copy->setText(QChar(0xf0c5));    //copy
	clear = new QPushButton(); clear->setFont(font); clear->setText(QChar(0xf068));  //minus
	Text_B->setLayout(Text_B_Layout);
	Text_B_Layout->addWidget(text, 0, 0, 4, 8);
	Text_B_Layout->addWidget(stop, 9, 6, 1, 1);
	Text_B_Layout->addWidget(copy, 9, 7, 1, 1);
	Text_B_Layout->addWidget(clear, 9, 5, 1, 1);
	Text_B_Layout->addWidget(toaddress, 9, 0, 1, 5);
	Text_B->setStyleSheet("QPushButton{background:#333333;color:white;border-radius: 15px;font-size:17px;font-weight:bold;min-width:80px;height:30px; }\
	                           QPushButton:hover{background:#000000;}\
	                           QPushButton:pressed{background:#aaaaaa;}");
	leftQtab->addTab(Text_B, "Text");
	text->setStyleSheet("QTextEdit{background-color:rgb(0,0,0);color:white;font-size:17px;font-family:Arial;}\
                            QScrollBar{background: black;width:15px; }\
			                QScrollBar::handle:vertical{background: white; min-height:20px; border-radius:5px; }\
			                QScrollBar::sub-line:vertical{background:black; }");
	//video part
	video = new QWidget();
	leftQtab->addTab(video, "Video");
	video_Layout = new QGridLayout();
	video->setLayout(video_Layout);
	videolabel = new QLabel();
	stop2 = new QPushButton(); stop2->setFont(font); stop2->setText(QChar(0xf04b));    //play
	copy2 = new QPushButton(); copy2->setFont(font); copy2->setText(QChar(0xf0c5));    //copy
	Cal2 = new QPushButton(); Cal2->setFont(font); Cal2->setText(QChar(0xf006));    //star
	rtsp = new QLineEdit();
	rtsp->setText("admin:a1234567:192.168.1.10");
	rtsp->setAlignment(Qt::AlignCenter);
	rtsp->setStyleSheet("color:white;width:160px;font-size:17px;font-family:Arial;border-radius: 10px;min-height:25px;background:rgb(51,51,51);text-align:center;");
	video_Layout->addWidget(videolabel, 0, 0, 4, 8);
	video_Layout->addWidget(stop2, 9, 6, 1, 1);
	video_Layout->addWidget(copy2, 9, 7, 1, 1);
	video_Layout->addWidget(Cal2, 9, 5, 1, 1);
	video_Layout->addWidget(rtsp, 9, 0, 1, 5);
	video->setStyleSheet("QPushButton{background:#333333;color:white;border-radius: 15px;font-size:17px;font-weight:bold;min-width:80px;height:30px; }\
	                           QPushButton:hover{background:#000000;}\
	                           QPushButton:pressed{background:#aaaaaa;}");
	leftQtab->setStyleSheet("QTabWidget{background-color:rgb(0,0,0);font-family:Arial;}\
                                 QTabBar::tab { border:none;min-width:100px; min-height:30px;font-size:16px;color:white;background-color:[255,255,255,0];}\
                                 QTabBar::tab:selected{color:#000000;font-weight:bold;background-color:#FFFFFF;}\
			                     QTabBar::tab:hover{background-color:gray;}\
			");
	Qttest2Class->setWindowTitle(QApplication::translate("Qttest2Class", "Netdemo", Q_NULLPTR));
	Qttest2Class->setWindowIcon(QIcon("virus2.ico"));
	QMetaObject::connectSlotsByName(Qttest2Class);
}
