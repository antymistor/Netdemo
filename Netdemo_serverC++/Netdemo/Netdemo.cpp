#include "Netdemo.h"
#include "Mstring.h"
#include <direct.h>
#include <io.h>
#include <algorithm>
#include <WINSOCK2.H>
#include <Windows.h>
#include <tchar.h>

//#include <QUdpsocket>
#pragma comment(lib,"WS2_32.lib")
#include"NetThread.h"
#include"Camerathread.h"
#include"HKIPcamera.h"
#include <ctime>
#include <QDebug>
Netdemo::Netdemo(QWidget *parent): QMainWindow(parent)
{
	hMutex = CreateMutex(NULL, FALSE, _T("threadall"));
	charttimer = new QTimer(this);
	ui.setupUi(this);
	connect(charttimer,SIGNAL(timeout()),this,SLOT(RealtimeDataSlot()));
	charttimer->start(100);
	connect(ui.start, SIGNAL(clicked()), this, SLOT(start_deal()));
	connect(ui.send, SIGNAL(clicked()), this, SLOT(send_deal()));
    
	connect(ui.stop, SIGNAL(clicked()), this, SLOT(stop_deal()));
	connect(ui.copy, SIGNAL(clicked()), this, SLOT(copy_deal()));
	connect(ui.clear, SIGNAL(clicked()), this, SLOT(clear_deal()));

	connect(ui.stop2, SIGNAL(clicked()), this, SLOT(stop2_deal()));
	connect(ui.copy2, SIGNAL(clicked()), this, SLOT(copy2_deal()));
	connect(ui.Cal2, SIGNAL(clicked()), this, SLOT(Cal2_deal()));
}

void Netdemo::RealtimeDataSlot() {
	WaitForSingleObject(hMutex, INFINITE);
	static float Max, Min;
	for (unsigned temp = 0; temp!=time1_list.size();++temp) {
		while (Point1L.size() > 100){
			Point1L.pop_front();
			datatemp.pop_front();
		}
		while (Point2L.size() > 100) {
			Point2L.pop_front();
			datatemp.pop_front();
		}
		datatemp.push_back(data1_list[temp]);
		datatemp.push_back(data2_list[temp]);
		Point1L.push_back(QPointF(time1_list[temp],data1_list[temp]));
		Point2L.push_back(QPointF(time1_list[temp],data2_list[temp]));
		ui.text->insertPlainText(QString("%1: %2 %3\r\n").arg(time1_list[temp]).arg(data1_list[temp]).arg(data2_list[temp]));
		ui.text->moveCursor(QTextCursor::End);
	}
	for (unsigned temp = 0; temp != time2_list.size(); ++temp) {
		ui.text->insertPlainText( QString("%1:").arg(time2_list[temp])+ QString::fromStdString(orginrec_list[temp])+QString("\r\n") );
		ui.text->moveCursor(QTextCursor::End);
	}

	time1_list.clear();
	data1_list.clear();
	data2_list.clear();
	time2_list.clear();
	orginrec_list.clear();
	Max = *std::max_element(datatemp.begin(),datatemp.end());
	Min = *std::min_element(datatemp.begin(), datatemp.end());
	ui.line1->replace(Point1L);
	ui.line1->setPen(QPen(Qt::red, 3, Qt::DotLine));
	ui.line2->replace(Point2L);
	ui.line2->setPen(QPen(Qt::blue, 3, Qt::DotLine));
	if (Point1L.size() > 1) {
		ui.chart->axisY()->setRange(Min-1, Max+1);
		if (Point1L.back().x() > 1)
		{ui.chart->axisX()->setRange(Point1L.front().x(), Point1L.back().x());}
	}
	ReleaseMutex(hMutex);
}
void Netdemo::start_deal() {
	start_state = (start_state + 1) % 2;
	if (start_state) {
		ui.chart->axisX()->setRange(0, 1);
		Point1L.clear();
		Point2L.clear();
		time1_list.clear();
		data1_list.clear();
		data2_list.clear();

		if (ui.combo->currentText() == "UDP") {
			int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (err != 0)
			{
				MessageBox(NULL, _T("WSAStartup Error"), _T("Error"), MB_OK);
				start_state = 0; return;
			};
			Mstring str(ui.localaddress->text().toStdString());
			ServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			ServerAddr.sin_family = AF_INET;
			ServerAddr.sin_port = htons(stoi(str.split(':')[1]));
			ServerAddr.sin_addr.s_addr = inet_addr(str.split(':')[0].data());
			if (bind(ServerSocket, (SOCKADDR *)&ServerAddr, sizeof(ServerAddr)))
			{
				MessageBox(NULL, _T("Connect Error"), _T("Error"), MB_OK);
				WSACleanup(); closesocket(ServerSocket); start_state = 0; return;
			}
			Mstring str2(ui.toaddress->text().toStdString());
			ClientAddr.sin_family = AF_INET;
			ClientAddr.sin_port = htons(stoi(str2.split(':')[1]));
			ClientAddr.sin_addr.S_un.S_addr = inet_addr(str2.split(':')[0].data());
			sendto(ServerSocket, " ", strlen(" ") + 1, 0, (SOCKADDR*)&ClientAddr, sizeof(SOCKADDR));
			starttime = clock();
			/*NetThread *thread1;
			thread1 = new NetThread(this);
			thread1->start();*/
			CreateThread(NULL, 0, netthread2, this, 0, 0);
		}
		else if(ui.combo->currentText() == "TCP"){
			int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (err != 0)
			{MessageBox(NULL, _T("WSAStartup Error"), _T("Error"), MB_OK);
			start_state = 0; return;
			};
			Mstring str(ui.localaddress->text().toStdString());
			BaseSocket = socket(AF_INET, SOCK_STREAM, 0);
			ServerAddr.sin_family = AF_INET;
			ServerAddr.sin_port = htons(stoi(str.split(':')[1]));
			ServerAddr.sin_addr.s_addr = inet_addr(str.split(':')[0].data());
			if (bind(BaseSocket, (SOCKADDR *)&ServerAddr, sizeof(SOCKADDR))==SOCKET_ERROR)
			{
				MessageBox(NULL, _T("Connect Error"), _T("Error"), MB_OK);
			    WSACleanup(); closesocket(BaseSocket); start_state = 0; return;
			}
			if (listen(BaseSocket, SOMAXCONN) < 0) {
				MessageBox(NULL, _T("Listen Error"), _T("Error"), MB_OK);
				WSACleanup(); closesocket(BaseSocket); start_state = 0; return;
			}
			qDebug() << "listening!!!";
			int len = sizeof(SOCKADDR);
			ServerSocket = accept(BaseSocket, (SOCKADDR *)&ClientAddr, &len);
			if (ServerSocket == SOCKET_ERROR) {
				MessageBox(NULL, _T("Link Error"), _T("Error"), MB_OK);
				WSACleanup(); closesocket(BaseSocket); start_state = 0; return;
			}
			starttime = clock();
			NetThread *thread1;
			thread1 = new NetThread(this);

			thread1->start();
		}
	
		ui.start->setStyleSheet("QPushButton{background:rgb(109,223,35);\
			                     border-top-right-radius: 10px;\
                                 border-bottom-right-radius: 10px;\
                                 height:24px;}\
                                 QPushButton:hover{ background:green; }");
	}
	else {
		closesocket(ServerSocket);
		if (ui.combo->currentText() == "TCP") { closesocket(BaseSocket); }
		WSACleanup();
		ui.start->setStyleSheet("QPushButton{background:rgb(223,109,35);\
			                     border-top-right-radius: 10px;\
                                 border-bottom-right-radius: 10px;\
                                 height:24px;}\
                                 QPushButton:hover{ background:red; }");
	}
}

void Netdemo::send_deal() {
	ui.textsent->insertPlainText(QString("%1:").arg((float)(clock()-starttime)/1000) + ":" + ui.texttosent->text() + "\r\n");
	sendto(ServerSocket, ui.texttosent->text().toStdString().c_str(),
	strlen(ui.texttosent->text().toStdString().c_str()) + 1, 0, (SOCKADDR*)&ClientAddr, sizeof(SOCKADDR));
}

void Netdemo::stop_deal() {
	stop_state = (stop_state + 1) % 2;
	if (stop_state == 0) {
		ui.stop->setText(QChar(0xf04b));//play
	}
	else
	{
		ui.stop->setText(QChar(0xf04d));//stop
	}
}
void Netdemo::copy_deal() {
	std::string str = GetProgramDir() + "//" + ui.filepath->text().toStdString();
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyMMdd-hhmmss");
	if (0 !=_access(str.c_str(), 0))
	{
		mkdir(str.c_str());   
	}
	Mstring(ui.text->toPlainText().toStdString()).save(str+"//"+ current_date.toStdString()+".txt");
}
void Netdemo::clear_deal() {
	ui.text->clear();
}

void Netdemo::stop2_deal() {
	stop2_state = (stop2_state + 1) % 2;
	if (stop2_state == 1) {
		/*CameraThread *thread2;
		thread2 = new CameraThread(this);
		thread2->start();*/
		CreateThread(NULL, 0, camerathread2, this, 0, 0);
	}
}
void Netdemo::copy2_deal() {
	if (stop2_state == 1) {
		framesaveflag = 1;
	}
}
void Netdemo::Cal2_deal() {
	Cal2_state = (Cal2_state + 1) % 2;
	if (Cal2_state == 1) {
		ui.Cal2->setText(QChar(0xf005));
	}
	else {
		ui.Cal2->setText(QChar(0xf006));
   }
}
