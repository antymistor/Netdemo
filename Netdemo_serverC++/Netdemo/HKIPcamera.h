#ifndef __HKIPCAMERA__
#define __HKIPCAMERA__


#include <opencv2/opencv.hpp>
using namespace cv;
int init(const char* ip, const char* usr, const char* password);
Mat getframe();
void release();
long Realplay(HWND hWnd);
#endif