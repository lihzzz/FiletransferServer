// FiletransferServer.cpp : �������̨Ӧ�ó������ڵ㡣
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

void Receive(string path, SOCKET socket){
	int recvinfo;
	char* recvBuflength = new char[128];
	char* recvInfoBuf;
	char* sendBuf = new char[10];
	string currentpath;
	ofstream fout;
	int res;
	int a;
	string str;
	sockaddr_in ClientAddr;
	int nAddrLen = sizeof(sockaddr_in);
	while (1){
		ZeroMemory(recvBuflength, sizeof(recvBuflength));
		res = recv(socket, recvBuflength, sizeof(recvBuflength), 0);
		if (res == SOCKET_ERROR || res == 0)
			continue;
		else if (res > 0){
			
			switch (recvinfo)
			{
			case MSG_FILE:
				recvInfoBuf = new char[Filedata.Fileinfo.FileLength];
				while (recv(Filedata.sockClient, recvInfoBuf, Filedata.Fileinfo.FileLength, 0) <= 0){
				}
				currentpath = path + "\\" + Filedata.file;
				fout.open(currentpath, ios::binary | ios::trunc);
				fout.write(recvInfoBuf, Filedata.Fileinfo.FileLength);
				delete[] recvInfoBuf;
				fout.close();
				str = Filedata.Fileinfo.name;
				str = "�ļ�" + str + "���ճɹ�,���洢��" + path;
				cout << str << endl;
				sendto(Filedata.sockClient, "5", 1, 0, (sockaddr*)&Filedata.ClientAddr, Filedata.nAddrLen);
				send(Filedata.sockClient, "5", 1, 0);
				break;
			case MSG_FILEINFO:
				recvInfoBuf = new char[sizeof(Filedata.Fileinfo)];
				a = sizeof(Filedata.Fileinfo);
				while (recv(Filedata.sockClient, recvInfoBuf, sizeof(Filedata.Fileinfo), 0) <= 0){
				}
				memcpy(&Filedata.Fileinfo, recvInfoBuf, sizeof(Filedata.Fileinfo));
				Filedata.file = Filedata.Fileinfo.name;
				cout << Filedata.Fileinfo.name << endl;
				cout << "�ļ���С��" << Filedata.Fileinfo.FileLength << "�ֽ�" << endl;

				delete[]  recvInfoBuf;
				break;
			case MSG_OPENFILE_ERROR:
				break;
			case MSG_FILEALREADYEXIT_ERROR:
				break;
			default:
				break;
			}
		}
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	Info Filedata;
	string Storage = "E:\\tmp";
	fstream fs;
	cout << "����洢���ļ��� ��ʽ��c:\\\\a��" << endl;
	//cin >> Storage;
	fs.open(Storage, ios::in);
	if (!fs){
		_mkdir(Storage.c_str());
	}
	fs.open(Storage, ios::in);
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {   // ����ֵΪ���ʱ���Ǳ�ʾ�ɹ�����WSAStartup  
		exit(1);
		//MessageBox(NULL, _T("��ʼ��ʧ��"), (LPCWSTR)_T("����"), MB_OK);
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) { // ���汾���Ƿ���ȷ  
		WSACleanup();
		exit(1);
		//MessageBox(NULL, _T("��ʼ��ʧ��"), (LPCWSTR)_T("����"), MB_OK);
	}

	Filedata.sListen = socket(AF_INET, SOCK_STREAM, 0);
	if (Filedata.sListen == INVALID_SOCKET) {
		MessageBox(NULL, _T("Socket��ʼ��ʧ��"), _T("Error"), MB_OK);
		WSACleanup();
		exit(1);
	}

	// ��socket��һ�����ص�ַ
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (::bind(Filedata.sListen, (LPSOCKADDR)&sin, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		exit(1);
	}
	cout << "Start Listen......." << endl;
	// ����socket�������״̬
	if (::listen(Filedata.sListen, 10) == SOCKET_ERROR)
	{
		MessageBox(NULL, _T("Socket����ʧ��"), _T("����"), MB_OK);
		exit(1);
	}
	// ѭ������client�˵���������

	Filedata.nAddrLen = sizeof(sockaddr_in);
	Filedata.sockClient;
	while (INVALID_SOCKET == (Filedata.sockClient = ::accept(Filedata.sListen, (sockaddr*)&Filedata.ClientAddr, &Filedata.nAddrLen)))
	{
	}
	cout << "Client  Connect......." << endl;

	Receive(Storage, Filedata);
	// �ر�ͬ�ͻ��˵�����
	::closesocket(Filedata.sockClient);
	::closesocket(Filedata.sListen);
	return 0;
}
