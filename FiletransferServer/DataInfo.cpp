#include "stdafx.h"
#include "DataInfo.h"

Info::Info(){
	memset(Fileinfo.name, 0, sizeof(Fileinfo.name));
}

Info::~Info(){
}

void Info::OnPrepare(){

}