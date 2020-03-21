#pragma once
#ifndef _NetThread_
#define _NetThread_
#include<QThread>
#include<QVector>
#include"Netdemo.h"
class NetThread: public QThread
{
	Q_OBJECT
public:
	NetThread(Netdemo *ptr):Par(ptr) {}
private:
	Netdemo *Par;
	char RecvBuf[100];//发送数据的缓冲区
	int BufLen = 100; //缓冲区大小
protected:
	void run(); // 新线程入口

};
DWORD WINAPI netthread2(void *parent);
#endif 