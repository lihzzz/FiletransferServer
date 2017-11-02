#include "stdafx.h"
#include <string>
#include <Winsock2.h>
#include <sys/stat.h>
#include <fstream>  
using namespace std;

#define MAXFILE 1024
#define	PORT				4096			// �˿ں�
#define INVALID_MSG					-1		// ��Ч����Ϣ��ʶ
#define MSG_FILEINFO				1	  // �ļ�������
#define MSG_CLIENT_READY			3		// �����׼�������ļ�
#define MSG_FILE					4		// �ͻ��˿�ʼ�����ļ�
#define MSG_SENDFILESUCCESS			5		// �����ļ��ɹ�
#define MSG_OPENFILE_ERROR			10		// ���ļ�ʧ��,�������ļ�·�������Ҳ����ļ���ԭ��
#define MSG_FILEALREADYEXIT_ERROR	11		// Ҫ������ļ��Ѿ�������
#define MSG_QUIT					0


typedef struct myfile{
	int FileLength;
	//int namelength;
	char name[256];
}*mfile;

class Info{
public:
	Info();
	~Info();
	void OnPrepare();
	SOCKET sockClient;
	SOCKET sListen;
	string file;
	myfile Fileinfo;
	sockaddr_in ClientAddr;
	int nAddrLen;

};