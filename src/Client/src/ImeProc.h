#pragma once

//#include "ImeManager.h"


const int DRAW_LIST_SIZE = 8;
const int BLOCK_SIZE = 256;


const int BUF_SIZE = 1024;
const int SEND_KEY = VK_RETURN;	//ENTERキーを押したらサーバーに送信

//=================================
//IME関連、文字列関連を編集するクラス
//描画の方は一切触っていません。
//=================================
class ImeProc
{
public:
	ImeProc();
	~ImeProc();
private:
//	ImeManager m_ImeManager;
	char listString[BUF_SIZE];		//候補リストの文字列
	char imeString[BUF_SIZE];		//imeの現在の文字列
	DWORD m_BlockIndex[BLOCK_SIZE];
	int m_BlockNum;
	DWORD m_ListIndex[BLOCK_SIZE];
	DWORD m_ListNum;
	DWORD m_ThisBlock;				//現在の文節の番地
	DWORD m_ThisList;
	DWORD m_ThisPointNum;
	DWORD m_StringNum;
	char m_AllString[BUF_SIZE];//文字列バッファ
	bool m_ShiftFlag;		//SHIFT！！が押された時に文字を判別するときに使う
	int m_ThisPoint;		//現在の指定されているポイント
	int m_StringSize;		//現在の文字列の大きさ
	bool m_On;				//このフラグがONのときに文字列操作をします。

	void CreateListString(DWORD index,LPCANDIDATELIST);
public:
	void Init(HWND hwnd);			//初期化
	BOOL Compo(LPARAM lparam);
	BOOL StartCompo();
	BOOL EndCompo();
	BOOL Nofity(WPARAM wparam,HWND hwnd);
	BOOL SetCon(LPARAM);
	DWORD GetSeclect();
	char *GetListString();			
	char *GetImeString();
	int GetThisList();
	void GetImeBlock(int inIndex,int *outStart,int *outEnd,bool *outNew);
	void GetImeBlockString(int inIndex,char *outString,int *outSize,bool *outNew);
	int GetBlockNum();
	void GetListBlock(int inIndex,int *outStart,int *outEnd,bool *outNew);
	void GetListBlockString(int inIndex,char *outString,int *outSize,bool *outNew);
	int GetListNum();

	DWORD GetThisPointNum(){ return m_ThisPointNum; }

	bool IsZenkaku( unsigned char c );
	void DeletePoint(int size);	//ThisPointのさしている文字列を削除します。
	BOOL Add(unsigned int key);
	int KeyShift(unsigned int key);
	int KeyNonShift(unsigned int key);
	virtual BOOL Send();
	char *GetAllString();		//文字列バッファのアドレスを取得
	BOOL KeyDown(WPARAM);		//KeyDownProc
	BOOL KeyUp(WPARAM);			//KeyUpProc
	int GetAllStringSize();
	bool Move(int num);		//指定している文字を動かします。
	BOOL Add(char *buffer,int size);	//文字列を追加します。
	void Delete();				//現在の指している文字を削除します。
	void SetOn(bool imeOn);		//IMEをOnにします。オンラインゲームとかOFFの時もあるよね
	int GetThisPoint();			//現在の指している位置を取得します。文字を書くときに点滅している'|'の位置です。
};
