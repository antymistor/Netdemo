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
	char RecvBuf[100];//�������ݵĻ�����
	int BufLen = 100; //��������С
protected:
	void run(); // ���߳����

};
DWORD WINAPI netthread2(void *parent);
#endif 