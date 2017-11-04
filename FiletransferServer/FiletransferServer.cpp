// FiletransferServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "DataInfo.h"
#include <process.h>
#include <string>
#include <iostream>
#include <fstream>
#include <direct.h>
#pragma comment(lib, "Ws2_32.lib")  


using namespace std;
int getinfolength(char* src,int& index);
string getinfoname(char* src, int& index);
void Receive(string path, SOCKET socket){
	char* recvlengthBuf = new char[128];
	char* recvInfoBuf;
	string currentpath, curinfoname, infotmp;
	char* curinfo;
	ofstream fout;
	int res;
	while (1){
		ZeroMemory(recvlengthBuf, sizeof(recvlengthBuf));
		res = recv(socket, recvlengthBuf, 128, 0);
		if (res == SOCKET_ERROR || res == 0)
			continue;
		else if (res > 0){
			int length = stoi(string(recvlengthBuf));
			recvInfoBuf = new char[length+1];
			while (recv(socket, recvInfoBuf, length, 0) <= 0){
			}
			int curindex = 0;
			int filecount = getinfolength(recvInfoBuf + curindex, curindex);
			cout << endl;
			for (int i = 0; i < filecount; i++)
			{
				curinfoname = getinfoname(recvInfoBuf + curindex, curindex);
				int curinfolength = getinfolength(recvInfoBuf + curindex, curindex);
				curinfo = new char[curinfolength];
				copy(recvInfoBuf + curindex, recvInfoBuf+ curindex + curinfolength, curinfo);
				curindex += curinfolength + 1;
				currentpath = path + "\\" + curinfoname;
				fout.open(currentpath, ios::binary | ios::trunc);
				fout.write(curinfo, curinfolength);
				fout.close();
				string str = "文件" + curinfoname + "接收成功,并存储于" + path;
				cout << str << endl;
			}
			cout << "接收完毕" << endl;
			delete[] recvInfoBuf;
		}
	}
}

int getinfolength(char* src, int& index){
	string tmp;
	while (*src != '-'){
		index++;
		tmp += *src ++;
	}
	index++;
	return stoi(tmp);
}

string getinfoname(char* src, int& index){
	string tmp;
	while (*src != '-'){
		index++;
		tmp += *src ++;
	}
	index++;
	return tmp;
}
int _tmain(int argc, _TCHAR* argv[])
{
	Info Filedata;
	string Storage = "F:\\tmp";
	fstream fs;
	cout << "输入存储的文件夹 格式（c:\\\\a）" << endl;
	//cin >> Storage;
	fs.open(Storage, ios::in);
	if (!fs){
		_mkdir(Storage.c_str());
	}
	fs.open(Storage, ios::in);
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {   // 返回值为零的时候是表示成功申请WSAStartup  
		exit(1);
		//MessageBox(NULL, _T("初始化失败"), (LPCWSTR)_T("错误"), MB_OK);
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) { // 检查版本号是否正确  
		WSACleanup();
		exit(1);
		//MessageBox(NULL, _T("初始化失败"), (LPCWSTR)_T("错误"), MB_OK);
	}

	Filedata.sListen = socket(AF_INET, SOCK_STREAM, 0);
	if (Filedata.sListen == INVALID_SOCKET) {
		MessageBox(NULL, _T("Socket初始化失败"), _T("Error"), MB_OK);
		WSACleanup();
		exit(1);
	}

	// 绑定socket到一个本地地址
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (::bind(Filedata.sListen, (LPSOCKADDR)&sin, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		exit(1);
	}
	cout << "Start Listen......." << endl;
	// 设置socket进入监听状态
	if (::listen(Filedata.sListen, 10) == SOCKET_ERROR)
	{
		MessageBox(NULL, _T("Socket监听失败"), _T("错误"), MB_OK);
		exit(1);
	}
	// 循环接收client端的连接请求

	Filedata.nAddrLen = sizeof(sockaddr_in);
	Filedata.sockClient;
	while (INVALID_SOCKET == (Filedata.sockClient = ::accept(Filedata.sListen, (sockaddr*)&Filedata.ClientAddr, &Filedata.nAddrLen)))
	{
	}
	cout << "Client  Connect......." << endl;

	Receive(Storage, Filedata.sockClient);
	// 关闭同客户端的连接
	::closesocket(Filedata.sockClient);
	::closesocket(Filedata.sListen);
	return 0;
}
