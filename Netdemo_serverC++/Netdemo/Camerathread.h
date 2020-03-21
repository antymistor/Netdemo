#pragma once
#ifndef _Camerthread_
#define _Camerthread_
#include<QThread>
#include<QVector>
#include"Netdemo.h"
class CameraThread : public QThread
{
	Q_OBJECT
public:
	CameraThread(Netdemo *ptr) :Par(ptr) {}
private:
	Netdemo *Par;
	cv::Mat pic;
protected:
	void run(); // 新线程入口
};
DWORD WINAPI camerathread2(void *parent);
#endif
