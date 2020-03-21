#pragma once
#ifndef __Netdemo__
#define __Netdemo__

#include <QtWidgets/QMainWindow>
#include "ui_Netdemo.h"
#include <string>
#include <Qstring>
#include <WINSOCK2.H>
#pragma comment(lib,"WS2_32.lib")
//#include "Camerathread.h"
#include <opencv2/opencv.hpp>
using namespace cv;

class Netdemo : public QMainWindow
{
	Q_OBJECT
public:
	HANDLE hMutex = NULL;
	explicit Netdemo(QWidget *parent = Q_NULLPTR);
	unsigned start_state = 0;
	unsigned stop_state =1;
	unsigned copy_state = 0;
	unsigned data_type = 0;
	unsigned thread_flag = 0;
	unsigned printprogress = 0;
	unsigned printprogress2 = 0;
	unsigned stop2_state =0;
	unsigned framesaveflag = 0;
	unsigned Cal2_state = 0;
	long starttime;
	//SOCKETPART
	WSADATA wsaData;
	SOCKET ServerSocket;
	SOCKET BaseSocket;
	sockaddr_in ServerAddr;//服务器地址
	sockaddr_in ClientAddr;//客户端地址
	int ClientAddrSize = sizeof(ClientAddr);
	QList<QPointF> Point1L;
	QList<QPointF> Point2L;       //used to draw
	QVector<float> time1_list;   
	QVector<float> data1_list;
	QVector<float> data2_list;   //used to save the data to draw

	QList<float> datatemp;       //used to get Max and Min

	QVector<float> time2_list;
	QVector<std::string> orginrec_list;
	QTimer *charttimer;
	//Mat picget;
	Ui_Netdemo ui;

private slots:
    void RealtimeDataSlot();
//udp/tcp set
	void start_deal();
//do send
	void send_deal();
//text part
	void stop_deal();
	void copy_deal();
	void clear_deal();
//video part
	void stop2_deal();
	void copy2_deal();
	void Cal2_deal();

};
#endif
