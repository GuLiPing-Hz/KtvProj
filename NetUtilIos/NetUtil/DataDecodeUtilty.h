#ifndef DATADECODEUTILTY__H__
#define DATADECODEUTILTY__H__

#include "ResultData.h"
#include "wrap/RWStream.h"

int _ChangeAvarat( ResultData_UserFullInfo * pInfo, NetworkUtil::BinaryReadStream * pBuf );
int _ReadUserData( NetworkUtil::BinaryReadStream & stream, ResultData_UserFullInfo & pInfo );

#endif//DATADECODEUTILTY__H__
