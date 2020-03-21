#include"Camerathread.h"
#include <ctime>
#include "Mstring.h"
#include "HKIPcamera.h"
#include <tchar.h>
#include <QDebug>
#include <direct.h>
#include <io.h>

Size DIM(1280, 720);
cv::Matx33d K( 911.1546799130407, 0.0, 636.1179417117924,
			   0.0, 909.46867680695, 362.22498374331553,
			   0.0, 0.0, 1.0);    //   摄像机内参数矩阵
cv::Vec4d D(-0.20518436786315236, 0.13086850373804892, -0.38920896106289166, 0.3717694153970601);


Size DIM2(640, 450);
cv::Matx33d K2(454.6079430869957, 0.0, 318.3465274330906,
	           0.0, 566.2354983838005, 226.9368868881296,
	           0.0, 0.0, 1.0);    //   摄像机内参数矩阵
cv::Vec4d D2(-0.20234958495037134, 0.1432242196055454, -0.4347283951611514, 0.4076209075432136);


void CameraThread::run() {
	
	auto loginfo = Mstring(Par->ui.rtsp->text().toStdString()).split(':');

	auto ip = loginfo[2]; 
	auto name = loginfo[0];
	auto pw = loginfo[1];

	cv::Mat Rgb;
	cv::Mat mapx2(DIM2, CV_32FC1);
	cv::Mat mapy2(DIM2, CV_32FC1);
	cv::Mat R = Mat::eye(3, 3, CV_32F);
	//cv::Mat newCameraMatrix(3, 3, CV_32FC1, Scalar::all(0));
	QImage Img;
	cv::Mat pic2;
	if (init(ip.data(), name.data(), pw.data()) < 0)
	{
		qDebug() << "fail";
		MessageBox(NULL, _T("Camera Not Connected"), _T("Error"), MB_OK);
		Par->ui.stop2->setText(QChar(0xf04b));//play
		Par->stop2_state = 0;
	}
	else {
		Par->ui.stop2->setText(QChar(0xf04d));//stop
		Par->stop2_state = 1;
	}
	while (Par->stop2_state)
	{
		Sleep(10);
		WaitForSingleObject(Par->hMutex, INFINITE);
		pic = getframe();
		cv::resize(pic, pic2, DIM2);
		cv::cvtColor(pic2,Rgb, CV_BGR2RGB);
		if (Par->Cal2_state == 1) {
			fisheye::initUndistortRectifyMap(K2, D2, R, K2, DIM2, CV_32FC1, mapx2, mapy2);
			cv::remap(Rgb, Rgb, mapx2, mapy2, INTER_LINEAR);
			//Sleep(20);
		 }
		else{
		//Sleep(20);
		}

		Img = QImage((const uchar*)(Rgb.data), Rgb.cols, Rgb.rows, Rgb.cols * Rgb.channels(), QImage::Format_RGB888);
		Par->ui.videolabel->setPixmap(QPixmap::fromImage(Img));
		if (Par->framesaveflag == 1) {

			std::string str = GetProgramDir()+"//" + Par->ui.filepath->text().toStdString();
			QDateTime current_date_time = QDateTime::currentDateTime();
			QString current_date = current_date_time.toString("yyMMdd-hhmmss");
			if (0 != _access(str.c_str(), 0))
			{mkdir(str.c_str());}
			cv::imwrite(str + "//" + current_date.toStdString() + ".jpg",pic);
			static cv::Mat mapx(pic.size(), CV_32FC1);
			static cv::Mat mapy(pic.size(), CV_32FC1);
			fisheye::initUndistortRectifyMap(K, D, R, K, pic.size(), CV_32FC1, mapx, mapy);
			cv::remap(pic, pic2, mapx, mapy, INTER_LINEAR);
			cv::imwrite(str + "//" + current_date.toStdString() + "d.jpg", pic2);
			Par->framesaveflag = 0;
	   }
		ReleaseMutex(Par->hMutex);
	}

		Par->ui.stop2->setText(QChar(0xf04b));
		Par->stop2_state = 0;
		release();
	
}
DWORD WINAPI camerathread2(void *parent) {
	Netdemo *Par;
	Par = (Netdemo *)parent;
	auto loginfo = Mstring(Par->ui.rtsp->text().toStdString()).split(':');

	auto ip = loginfo[2];
	auto name = loginfo[0];
	auto pw = loginfo[1];

	cv::Mat pic,Rgb;
	cv::Mat mapx2(DIM2, CV_32FC1);
	cv::Mat mapy2(DIM2, CV_32FC1);
	cv::Mat R = Mat::eye(3, 3, CV_32F);
	//cv::Mat newCameraMatrix(3, 3, CV_32FC1, Scalar::all(0));
	QImage Img;
	cv::Mat pic2;
	if (init(ip.data(), name.data(), pw.data()) < 0)
	{
		qDebug() << "fail";
		MessageBox(NULL, _T("Camera Not Connected"), _T("Error"), MB_OK);
		Par->ui.stop2->setText(QChar(0xf04b));//play
		Par->stop2_state = 0;
	}
	else {
		Par->ui.stop2->setText(QChar(0xf04d));//stop
		Par->stop2_state = 1;
	}
	while (Par->stop2_state)
	{
		Sleep(5);
		WaitForSingleObject(Par->hMutex, INFINITE);
		Sleep(5);
		pic = getframe();
		cv::resize(pic, pic2, DIM2);
		cv::cvtColor(pic2, Rgb, CV_BGR2RGB);
		if (Par->Cal2_state == 1) {
			fisheye::initUndistortRectifyMap(K2, D2, R, K2, DIM2, CV_32FC1, mapx2, mapy2);
			cv::remap(Rgb, Rgb, mapx2, mapy2, INTER_LINEAR);
			//Sleep(20);
		}
		else {
			//Sleep(20);
		}

		Img = QImage((const uchar*)(Rgb.data), Rgb.cols, Rgb.rows, Rgb.cols * Rgb.channels(), QImage::Format_RGB888);
		Par->ui.videolabel->setPixmap(QPixmap::fromImage(Img));
		if (Par->framesaveflag == 1) {

			std::string str = GetProgramDir() + "//" + Par->ui.filepath->text().toStdString();
			QDateTime current_date_time = QDateTime::currentDateTime();
			QString current_date = current_date_time.toString("yyMMdd-hhmmss");
			if (0 != _access(str.c_str(), 0))
			{
				mkdir(str.c_str());
			}
			cv::imwrite(str + "//" + current_date.toStdString() + ".jpg", pic);
			static cv::Mat mapx(pic.size(), CV_32FC1);
			static cv::Mat mapy(pic.size(), CV_32FC1);
			fisheye::initUndistortRectifyMap(K, D, R, K, pic.size(), CV_32FC1, mapx, mapy);
			cv::remap(pic, pic2, mapx, mapy, INTER_LINEAR);
			cv::imwrite(str + "//" + current_date.toStdString() + "d.jpg", pic2);
			Par->framesaveflag = 0;
		}
		ReleaseMutex(Par->hMutex);
	}

	Par->ui.stop2->setText(QChar(0xf04b));
	Par->stop2_state = 0;
	release();
}