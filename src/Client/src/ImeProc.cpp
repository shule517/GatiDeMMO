
#include "StdAfx.h"
#include "ImeProc.h"

HIMC himc;

//=======================
//コンストラクタ
//=======================
ImeProc::ImeProc()
{
	m_AllString[0] = '\0';
	m_ShiftFlag = 0;
	m_ThisPoint = 0;
	m_StringSize = 0;
	m_On = 1;
	m_ThisPointNum = 0;
	m_StringNum = 0;
}

//=======================
//デストラクタ
//=======================
ImeProc::~ImeProc()
{


}

//=======================
//初期化
//=======================
void ImeProc::Init(HWND hwnd)
{
	/*********************************/
	himc = ImmGetContext(hwnd);
	/*********************************/
//	m_ImeManager.Init(hwnd);
	listString[BUF_SIZE] = '\0';
	imeString[BUF_SIZE] = '\0';
	m_ThisBlock = 0;
	m_ThisList = 0;
}


//==========================
//文字列を取得
//==========================
char *ImeProc::GetAllString()
{
	return m_AllString;
}

//=================================
//現在の文字列の位置から引数分移動
//=================================
bool ImeProc::Move(int num)
{
	int tmpPoint = m_ThisPointNum + num;
	if(!(0 <= tmpPoint && tmpPoint <= (int)m_StringNum))
	{
		return 0;
	}

	m_ThisPointNum = tmpPoint;
	m_ThisPoint = 0;
	for(int i = 0; i < (int)m_ThisPointNum; i++)
	{
		if(IsZenkaku(m_AllString[m_ThisPoint]))
		{
			m_ThisPoint++;
		}
		m_ThisPoint++;
	}
	return 1;
}



//==========================================
//キーが押された時のメッセージ
//==========================================
BOOL ImeProc::KeyDown(WPARAM wparam)
{
	if(m_On == 0)
	{
		return 0;
	}
	int key = (int)wparam;
	//SHIFT検出
	if(key == VK_SHIFT)
	{
		m_ShiftFlag = 1;
		return 0;
	}

	if(key == SEND_KEY)
	{
		//ここで、文字列を送信
		Send();
	}

	if(key == VK_SPACE)
	{
		Add(" ",1);
		return 0;
	}

	//文字のカーソル移動
	if(key == VK_LEFT)
	{
		Move(-1);
		return 0;
	}
	else if(key == VK_RIGHT)
	{
		Move(1);
		return 0;
	}
	else if(key == VK_BACK)
	{
		if(Move(-1))
		{
			Delete();
		}
		return 0;
	}

	//削除ファンク
	if(key == VK_DELETE)
	{
		Delete();
		return 0;
	}



	Add(key);
	



	return 0;
}


//==========================================
//1文字を追加します。
//==========================================
BOOL ImeProc::Add(unsigned int key)
{
	int code;

	if(m_ShiftFlag)
	{
		code = KeyShift(key);
		if(code == -1)
		{
			return 0;
		}
	}
	else
	{
		code = KeyNonShift(key);
		if(code == -1)
		{
			return 0;
		}
	}
	
	
	char buffer[2];
	sprintf_s(buffer,"%c",code);
	return Add(buffer,1);
}

int ImeProc::GetAllStringSize()
{
	return m_StringSize;
}


//==========================================
//文字列を追加します。
//==========================================
BOOL ImeProc::Add(char *buffer,int size)
{
	int backPoint = m_ThisPoint;
	for(int i = m_StringSize + size; i >= m_ThisPoint + size; i--)
	{
		m_AllString[i] = m_AllString[i - size];
	}
	m_StringSize += size;
	m_AllString[m_StringSize] = '\0';
	
	memcpy(m_AllString + backPoint,buffer,size);

	
	int num = 0;
	for(int i = 0; i < size; i++)
	{
		if(IsZenkaku(buffer[i]))
		{
			i++;
		}
		num++;
	}

	m_StringNum += num;
	Move(num);
	return 1;
}

//==========================================
//指定した位置を削除します。
//==========================================
void ImeProc::DeletePoint(int size)
{
	for(int i = m_ThisPoint + size; i < m_StringSize; i++)
	{
		m_AllString[i - size] = m_AllString[i];
	}
	m_StringSize -= size;
	m_AllString[m_StringSize] = '\0';
}

//===================================
//現在の指している場所を削除します。
//===================================
void ImeProc::Delete()
{
	if(IsZenkaku(m_AllString[m_ThisPoint] ))
	{
		if(m_StringSize >= m_ThisPoint + 2)
		{
			DeletePoint(2);
			m_StringNum--;
		}
		return;
	}
	if(m_StringSize >= m_ThisPoint + 1)
	{
		DeletePoint(1);
		m_StringNum--;
	}
}

//================================
//指しているところを取得
//================================
int ImeProc::GetThisPoint()
{
	return m_ThisPoint;
}

//===================================
//キーが離されたときに呼ばれるフラグです。
//===================================
BOOL ImeProc::KeyUp(WPARAM wparam)
{
	if(m_On == 0)
	{
		return 0;
	}

	if(wparam == VK_SHIFT)
	{
		m_ShiftFlag = 0;
	}
	return 0;
}


//===================================
//IMEをONにする関数です。
//===================================
void ImeProc::SetOn(bool imeOn)
{
	m_On = imeOn;
}

//===================================
//IMEをONにする関数です。
//===================================
int ImeProc::KeyNonShift(unsigned int key)
{
	if(isupper(key))
	{
		return tolower(key);
	}
	if(isdigit(key))
	{
		return key;
	}
	
    switch(key)
	{
		case 0xba:
		    return 0x3a;    // ':'
		case 0xbb:
		    return 0x3b;    // ';'
		case 0xbc:
		    return 0x2c;    // ','
		case 0xbd:
		    return 0x2d;    // '-'
		case 0xbe:
		    return 0x2e;    // '.'
		case 0xbf:
		    return 0x2f;    // '/'
		case 0xc0:
		    return 0x40;    // '@'
		case 0xdb:
		    return 0x5b;    // '['
		case 0xdc:
		    return 0x5c;    // '\'
		case 0xdd:
	        return 0x5d;    // ']'
	    case 0xde:
	        return 0x5e;    // '^'
	    case 0xe2:
	        return 0x5c;    //バックスラッシュ
    }
	return -1;
}

//====================================
//SHIFTキーを押された時に呼ぶ関数
//====================================
int ImeProc::KeyShift(unsigned int key)
{
	if(isupper(key))
	{
		return key;
	}
    switch(key)
	{
		case '1':
			return '!';
		case '2':
			return '"';
		case '3':
			return '#';
		case '4':
			return '$';
		case '5':
			return '%';
		case '6':
			return '&';
		case '7':
			return 0x27;
		case '8':
			return '(';
		case '9':
			return ')';
		case '-':
			return '=';
		case '^':
			return '~';
		case 0x5c:
			return 0x7c;
		case 0xba:
		    return 0x2a;    // '*'
		case 0xbb:
		    return 0x2b;    // '+'
		case 0xbc:
		    return 0x3c;    // '<'
		case 0xbd:
		    return 0x3d;    // '='
		case 0xbe:
		    return 0x3e;    // '>'
		case 0xbf:
		    return 0x3f;    // '?'
	    case 0xc0:
		    return 0x60;    //バックスラッシュ
		case 0xdb:
		    return 0x7b;    // '{' 
	    case 0xdc:
	        return 0x7c;    // '|'
	    case 0xdd:
	        return 0x7d;    // '}'
	    case 0xde:
	        return 0x7e;    // '~'
	    case 0xe2:
	        return 0x5f;    // '_'
	}
	return -1;
}


//==========================================================================
//
//		全角チェック
//
//==========================================================================
bool ImeProc::IsZenkaku( unsigned char c )
{
	if ( c < 0x81 ) return false;//制御コード、半角英数字
	if ( c < 0xA0 ) return true;
	if ( c < 0xE0 ) return false;//半角カナ
	if ( c < 0xFF ) return true;

	return false;
}




//=======================
//現在の候補を取得します。
//=======================
DWORD ImeProc::GetSeclect()
{
	return m_ThisBlock;
}

//=======================
//コンテキストのSet
//=======================
BOOL ImeProc::SetCon(LPARAM lparam)
{
	lparam &= ~ISC_SHOWUIALL;
	return 0;
}
/******************************************************************/
//=====================================
//IMEの文字列を取得
//=====================================
LONG GetCompString(DWORD flag,LPVOID buffer,DWORD size)
{
	return ImmGetCompositionString(himc,flag,buffer,size);
}

/******************************************************************/

//=======================
//コンテキストの編集
//=======================
BOOL ImeProc::Compo(LPARAM lp)
{
	if(m_On == 0)
	{
		return 0;
	}
	if (lp & GCS_RESULTSTR) 
	{
//		int size  = m_ImeManager.GetCompString(GCS_RESULTSTR,imeString,BUF_SIZE);
		int size  = GetCompString(GCS_RESULTSTR,imeString,BUF_SIZE);
		imeString[size] = '\0';
		Add(imeString,size);
	}
	if (lp & GCS_COMPSTR) 
	{
//		int size  = m_ImeManager.GetCompString(GCS_COMPSTR,imeString,BUF_SIZE);
		int size  = GetCompString(GCS_COMPSTR,imeString,BUF_SIZE);
		imeString[size] = '\0';
	}
//	m_BlockNum = m_ImeManager.GetCompString(GCS_COMPCLAUSE,m_BlockIndex,sizeof(DWORD) * BLOCK_SIZE);
	m_BlockNum = GetCompString(GCS_COMPCLAUSE,m_BlockIndex,sizeof(DWORD) * BLOCK_SIZE);
	m_BlockNum /= sizeof(DWORD);
	m_BlockNum--;

	BYTE charAttr[BUF_SIZE];
//	int num = m_ImeManager.GetCompString(GCS_COMPATTR,charAttr,BUF_SIZE);
	int num = GetCompString(GCS_COMPATTR,charAttr,BUF_SIZE);

	for(int i = 0; i < num; i++)
	{
		switch(charAttr[i]) 
		{
			case ATTR_TARGET_CONVERTED:
			case ATTR_TARGET_NOTCONVERTED:
				m_ThisBlock = i;
				return 0;
		}
	}
	return 0;
}

void ImeProc::GetListBlock(int inIndex,int *outStart,int *outEnd,bool *outNew)
{
	*outStart = m_ListIndex[inIndex];
	*outEnd = m_ListIndex[inIndex + 1] - 1;

	if(inIndex == m_ThisList)
	{
		*outNew = 1;
	}
	else
	{
		*outNew = 0;
	}
}

void ImeProc::GetListBlockString(int inIndex,char *outString,int *outSize,bool *outNew)
{
	int theStart;
	int theEnd;
	GetListBlock(inIndex,&theStart,&theEnd,outNew);

	*outSize = theEnd - theStart + 1;
	memcpy(outString,listString + theStart,*outSize);
	outString[*outSize - 1] = '\0';
}

int ImeProc::GetListNum()
{
	return m_ListNum;
}


int ImeProc::GetBlockNum()
{
	return m_BlockNum;
}



void ImeProc::GetImeBlock(int inIndex,int *outStart,int *outEnd,bool *outNew)
{
	*outStart = m_BlockIndex[inIndex];
	*outEnd = m_BlockIndex[inIndex + 1] - 1;

	if(*outStart == m_ThisBlock)
	{
		*outNew = 1;
	}
	else
	{
		*outNew = 0;
	}
}

void ImeProc::GetImeBlockString(int inIndex,char *outString,int *outSize,bool *outNew)
{
	int theStart;
	int theEnd;
	GetImeBlock(inIndex,&theStart,&theEnd,outNew);

	*outSize = theEnd - theStart + 1;
	memcpy(outString,imeString + theStart,*outSize);
	outString[*outSize] = '\0';
}

//=======================
//コンテキストのスタート
//=======================
BOOL ImeProc::StartCompo()
{
	 return 0;
}

//=======================
//コンテキストの終了
//=======================
BOOL ImeProc::EndCompo()
{
	listString[0] = '\0';
	imeString[0] = '\0';

	return 0;
}

//=======================
//送信
//=======================
BOOL ImeProc::Send()
{
	/*
	FILE *fp = fopen("hoge.txt","a");
	fprintf(fp,"%s\n",m_AllString);
	fclose(fp);
	*/

	while( m_ThisPoint != 0 )
	{
		if(Move(-1))
		{
			Delete();
		}
	}

	m_AllString[0] = '\0';
	m_ShiftFlag = 0;
	m_ThisPoint = 0;
	m_StringSize = 0;
	listString[BUF_SIZE] = '\0';
	imeString[BUF_SIZE] = '\0';
	m_ThisBlock = 0;
	m_ListNum = 0;
	m_ThisList = 0;

	return 1;
}

//========================================
//候補リストを文字列に起こします。
//========================================
void ImeProc::CreateListString(DWORD index,LPCANDIDATELIST lpcdl)
{
	char buffer[BUF_SIZE];
	listString[0] = '\0';
	DWORD start = (index / DRAW_LIST_SIZE) * DRAW_LIST_SIZE;
	DWORD end = start + DRAW_LIST_SIZE;
	if(end > lpcdl->dwCount)
	{
		end = lpcdl->dwCount - 1;
	}
	m_ThisList = (lpcdl->dwSelection % DRAW_LIST_SIZE);
	m_ListNum = 0;
	int num = 0;
	m_ListIndex[0] = 0;
	m_ListNum = end - start;
	for (DWORD i = 0; i < m_ListNum; i++) 
	{
		wsprintf(buffer, "%1d. %s\n",i + 1, (char *)lpcdl + lpcdl->dwOffset[i + start]);
		num += (int)strlen(buffer);
		strcat_s(listString,buffer);
		m_ListIndex[i + 1] = num;
	}
}

int ImeProc::GetThisList()
{
	return m_ThisList;
}

/**********************************************/
//=====================================
//IMEの候補リストの文字列を取得
//=====================================
BOOL GetList(DWORD deIndex, LPCANDIDATELIST lpCandList,DWORD dwBufLen)
{
	return ImmGetCandidateList(himc,deIndex,lpCandList,dwBufLen);
}
/**********************************************/

//========================================
//文字列のイベント関連
//========================================
BOOL ImeProc::Nofity(WPARAM wparam,HWND hwnd)
{
	if(m_On == 0)
	{
		return 0;
	}
	LPCANDIDATELIST lpcdl;
    DWORD dwSize;
	switch (wparam) 
	{
		case IMN_OPENCANDIDATE:
			listString[0] = '\0';
//			dwSize = m_ImeManager.GetList(0, NULL, 0);
			dwSize = GetList(0, NULL, 0);
			lpcdl = (LPCANDIDATELIST)malloc(dwSize);
//            m_ImeManager.GetList(0, lpcdl, dwSize);
            GetList(0, lpcdl, dwSize);
			CreateListString(0,lpcdl);
			free(lpcdl);
			return 0;
		case IMN_CHANGECANDIDATE:
			listString[0] = '\0';
//			dwSize = m_ImeManager.GetList(0, NULL, 0);
			dwSize = GetList(0, NULL, 0);
			lpcdl = (LPCANDIDATELIST)malloc(dwSize);
//			m_ImeManager.GetList(0, lpcdl, dwSize);
			GetList(0, lpcdl, dwSize);
			CreateListString(lpcdl->dwSelection,lpcdl);
			free(lpcdl);
			return 0;
		case IMN_CLOSECANDIDATE:
			listString[0] = '\0';
			m_ListNum = 0;
			return 0;
	}
	return 1;
}



//========================================
//リストを取得
//========================================
char *ImeProc::GetListString()
{
	return listString;
}


//========================================
//IMEの文字を取得
//========================================
char *ImeProc::GetImeString()
{
	return imeString;
}




