#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv2\opencv.hpp>
#include <iostream>
#include <time.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <windows.h>
#include "HCNetSDK.h"
#include "plaympeg4.h"


#define USECOLOR 1

using namespace cv;
using namespace std;

//--------------------------------------------
int iPicNum = 0;//Set channel NO.
LONG nPort = -1;
HWND hWnd = NULL;
CRITICAL_SECTION g_cs_frameList;
//list<Mat> g_frameList;
Mat g_frame;
bool Newg_frame = false;
LONG lUserID;
NET_DVR_DEVICEINFO_V30 struDeviceInfo;
HANDLE hThread;
LONG lRealPlayHandle = -1;
void yv12toYUV(char *outYuv, char *inYv12, int width, int height, int widthStep)
{
	int col, row;
	unsigned int Y, U, V;
	int tmp;
	int idx;
	for (row = 0; row < height; row++)
	{
		idx = row * widthStep;
		int rowptr = row * width;

		for (col = 0; col < width; col++)
		{
			tmp = (row / 2)*(width / 2) + (col / 2);

			Y = (unsigned int)inYv12[row*width + col];
			U = (unsigned int)inYv12[width*height + width * height / 4 + tmp];
			V = (unsigned int)inYv12[width*height + tmp];

			outYuv[idx + col * 3] = Y;
			outYuv[idx + col * 3 + 1] = U;
			outYuv[idx + col * 3 + 2] = V;
		}
	}
}

//解码回调 视频为YUV数据(YV12)，音频为PCM数据
void CALLBACK DecCBFun(long nPort, char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1, long nReserved2)
{
	long lFrameType = pFrameInfo->nType;

	if (lFrameType == T_YV12)
	{
#if USECOLOR
		//int start = clock();
		static IplImage* pImgYCrCb = cvCreateImage(cvSize(pFrameInfo->nWidth, pFrameInfo->nHeight), 8, 3);//得到图像的Y分量  
		yv12toYUV(pImgYCrCb->imageData, pBuf, pFrameInfo->nWidth, pFrameInfo->nHeight, pImgYCrCb->widthStep);//得到全部RGB图像
		static IplImage* pImg = cvCreateImage(cvSize(pFrameInfo->nWidth, pFrameInfo->nHeight), 8, 3);
		cvCvtColor(pImgYCrCb, pImg, CV_YCrCb2RGB);
		//int end = clock();
#else
		static IplImage* pImg = cvCreateImage(cvSize(pFrameInfo->nWidth, pFrameInfo->nHeight), 8, 1);
		memcpy(pImg->imageData, pBuf, pFrameInfo->nWidth*pFrameInfo->nHeight);
#endif

		EnterCriticalSection(&g_cs_frameList);
		Newg_frame = true;
		g_frame = cv::cvarrToMat(pImg);
		LeaveCriticalSection(&g_cs_frameList);
	}
}

///实时流回调
void CALLBACK g_RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser)
{
	DWORD dRet;
	switch (dwDataType)
	{
	case NET_DVR_SYSHEAD:    //系统头
		if (nPort >= 0)
		{
			break;  //该通道取流之前已经获取到句柄，后续接口不需要再调用
		}

		if (!PlayM4_GetPort(&nPort)) //获取播放库未使用的通道号
		{
			break;
		}
		if (dwBufSize > 0)
		{
			if (!PlayM4_SetStreamOpenMode(nPort, STREAME_REALTIME))  //设置实时流播放模式
			{
				//dRet = PlayM4_GetLastError(nPort);
				break;
			}
			if (!PlayM4_OpenStream(nPort, pBuffer, dwBufSize, SOURCE_BUF_MAX))
			{
				//dRet = PlayM4_GetLastError(nPort);
				break;
			}
			//设置解码回调函数 只解码不显示
			if (!PlayM4_SetDecCallBack(nPort, DecCBFun))
			{
				//dRet = PlayM4_GetLastError(nPort);
				break;
			}
			//打开视频解码
			if (!PlayM4_Play(nPort, hWnd))
			{
				//dRet = PlayM4_GetLastError(nPort);
				break;
			}
		}
		break;

	case NET_DVR_STREAMDATA:   //码流数据
		if (dwBufSize > 0 && nPort != -1)
		{
			if (!PlayM4_InputData(nPort, pBuffer, dwBufSize))
			{
				break;
			}
		}
		break;
	default: //其他数据
		if (dwBufSize > 0 && nPort != -1)
		{
			if (!PlayM4_InputData(nPort, pBuffer, dwBufSize))
			{
				break;
			}
		}
		break;
	}


}



bool OpenCamera(const char* ip, const char* usr, const char* password)
{
	NET_DVR_USER_LOGIN_INFO struLoginInfo = { 0 };
	struLoginInfo.bUseAsynLogin = 0; //同步登录方式
	strcpy(struLoginInfo.sDeviceAddress, ip); //设备IP地址
	struLoginInfo.wPort = 8000; //设备服务端口
	strcpy(struLoginInfo.sUserName, usr); //设备登录用户名
	strcpy(struLoginInfo.sPassword, password); //设备登录密码

											   //设备信息, 输出参数
	NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = { 0 };


	lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);

	if (lUserID < 0)
	{
		printf("Login failed, error code: %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		return FALSE;
	}
	else {
		printf("Login success");
		//cout <<lUserID << endl;
		return TRUE;
	}

}
DWORD WINAPI ReadCamera(LPVOID IpParameter)
{
//	NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);

	//LONG lRealPlayHandle;
	HWND hWnd = GetConsoleWindow();
	NET_DVR_PREVIEWINFO struPlayInfo = { 0 };
	struPlayInfo.hPlayWnd = NULL;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
	struPlayInfo.lChannel = 1;       //预览通道号
	struPlayInfo.dwStreamType = 0;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
	struPlayInfo.dwLinkMode = 0;       //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP
	struPlayInfo.bBlocked = 0;       //0- 非阻塞取流，1- 阻塞取流

	lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, g_RealDataCallBack_V30, NULL);
	if (lRealPlayHandle < 0)
	{
		printf("NET_DVR_RealPlay_V40 failed! Error number: %d\n", NET_DVR_GetLastError());
		NET_DVR_Logout(lUserID);
		NET_DVR_Cleanup();
		return -1;
	}

	else
	{
		cout << "NET_DVR_RealPlay_V40 success" << endl;
	}
	Sleep(-1);
	if (!NET_DVR_StopRealPlay(lRealPlayHandle))
	{
		printf("NET_DVR_StopRealPlay error! Error number: %d\n", NET_DVR_GetLastError());
		return 0;
	}

	PlayM4_Stop(nPort);
	PlayM4_CloseStream(nPort);
	PlayM4_FreePort(nPort);

	NET_DVR_Logout(lUserID);
	NET_DVR_Cleanup();
	return 0;
}


int init(const char* ip, const char* usr, const char* password ){
	
	NET_DVR_Init();
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);
	if (!OpenCamera(ip, usr, password))
	{
		return -1;
	}
	
	InitializeCriticalSection(&g_cs_frameList);
	hThread = ::CreateThread(NULL, 0, ReadCamera,NULL, 0, 0);
	return 1;
}
long Realplay(HWND hWnd)
{
	NET_DVR_PREVIEWINFO struPlayInfo;
	struPlayInfo = { 0 };
	struPlayInfo.hPlayWnd = NULL;//需要 SDK 解码时句柄设为有效值，仅取流不解码时可设为空
	struPlayInfo.lChannel = 1;//预览通道号
	struPlayInfo.dwStreamType = 0;//码流类型：0-主码流，1-子码流，2-三码流，3-虚拟码流，以此类推
	struPlayInfo.dwLinkMode = 0;//0- TCP 方式，1- UDP 方式，2- 多播方式，3- RTP 方式，4-RTP/RTSP，5-RSTP/HTTP
	struPlayInfo.bBlocked = 1;//0- 非阻塞取流，1- 阻塞取流

	long IRealPlayHandle = NET_DVR_RealPlay_V40(0, &struPlayInfo, NULL, NULL);

	if (IRealPlayHandle <0)
	{
		cout << "NET_DVR_RealPlay_V40 error;error number " << NET_DVR_GetLastError();
		NET_DVR_Logout(lUserID);
		return 0;
	}
	else
		return IRealPlayHandle;
}

Mat getframe() {
	Mat frame1;
	EnterCriticalSection(&g_cs_frameList);
	while (!Newg_frame) {
		LeaveCriticalSection(&g_cs_frameList);
		EnterCriticalSection(&g_cs_frameList);
	}
	
	frame1 = g_frame;
	//g_frameList.clear();   // 丢掉旧的帧
	LeaveCriticalSection(&g_cs_frameList);
	//return(frame1);
	return(frame1);
}

void release() {
	::CloseHandle(hThread);
	NET_DVR_StopRealPlay(lRealPlayHandle);
	//关闭预览
	NET_DVR_Logout(lUserID);
	//注销用户
	NET_DVR_Cleanup();
}