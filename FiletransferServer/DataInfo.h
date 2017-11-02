#include "stdafx.h"
#include <string>
#include <Winsock2.h>
#include <sys/stat.h>
#include <fstream>  
using namespace std;

#define MAXFILE 1024
#define	PORT				4096			// 端口号
#define INVALID_MSG					-1		// 无效的消息标识
#define MSG_FILEINFO				1	  // 文件的名称
#define MSG_CLIENT_READY			3		// 服务端准备接收文件
#define MSG_FILE					4		// 客户端开始传送文件
#define MSG_SENDFILESUCCESS			5		// 传送文件成功
#define MSG_OPENFILE_ERROR			10		// 打开文件失败,可能是文件路径错误找不到文件等原因
#define MSG_FILEALREADYEXIT_ERROR	11		// 要保存的文件已经存在了
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