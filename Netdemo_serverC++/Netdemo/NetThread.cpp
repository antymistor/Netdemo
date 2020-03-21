#include"NetThread.h"
#include <ctime>
#include "Mstring.h"
void NetThread::run() {
	static Mstring str;
    static std::vector<Mstring> recdata;
	static sockaddr_in ClientAddr;
	static int SenderAddrSize = sizeof(ClientAddr);
	while (Par->start_state) {
		//WaitForSingleObject(Par->hMutex, INFINITE);
		memset(RecvBuf, 0, BufLen * sizeof(char));
		recvfrom(Par->ServerSocket, RecvBuf, BufLen, 0, (SOCKADDR *)&Par->ClientAddr, &SenderAddrSize);
		//qDebug() << RecvBuf ;
		if (Par->stop_state) {
			str = RecvBuf;
			recdata = str.split();
			if (recdata[0] == "D") {
				Par->time1_list.push_back((float)(clock() - Par->starttime) / 1000);
				Par->data1_list.push_back(stof(recdata[1]));
				Par->data2_list.push_back(stof(recdata[2]));
			}
			else {
				if (str.size()>0) {
					Par->time2_list.push_back((float)(clock() - Par->starttime) / 1000);
					Par->orginrec_list.push_back(str);
				}
			}
		}
		//ReleaseMutex(Par->hMutex);
		Sleep(5);
		
	}	
}
DWORD WINAPI netthread2(void *parent) {
	char RecvBuf[100];//发送数据的缓冲区
	int BufLen = 100; //缓冲区大小
	Netdemo *Par;
	Par = (Netdemo *)parent;
	static Mstring str;
	static std::vector<Mstring> recdata;
	static sockaddr_in ClientAddr;
	static int SenderAddrSize = sizeof(ClientAddr);
	while (Par->start_state) {
		//WaitForSingleObject(Par->hMutex, INFINITE);
		memset(RecvBuf, 0, BufLen * sizeof(char));
		recvfrom(Par->ServerSocket, RecvBuf, BufLen, 0, (SOCKADDR *)&Par->ClientAddr, &SenderAddrSize);
		//qDebug() << RecvBuf ;
		if (Par->stop_state) {
			str = RecvBuf;
			recdata = str.split();
			if (recdata[0] == "D") {
				Par->time1_list.push_back((float)(clock() - Par->starttime) / 1000);
				Par->data1_list.push_back(stof(recdata[1]));
				Par->data2_list.push_back(stof(recdata[2]));
			}
			else {
				if (str.size()>0) {
					Par->time2_list.push_back((float)(clock() - Par->starttime) / 1000);
					Par->orginrec_list.push_back(str);
				}
			}
		}
		//ReleaseMutex(Par->hMutex);
		Sleep(1);

	}
}
