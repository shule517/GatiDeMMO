#pragma once

//#include "ImeManager.h"


const int DRAW_LIST_SIZE = 8;
const int BLOCK_SIZE = 256;


const int BUF_SIZE = 1024;
const int SEND_KEY = VK_RETURN;	//ENTER�L�[����������T�[�o�[�ɑ��M

//=================================
//IME�֘A�A������֘A��ҏW����N���X
//�`��̕��͈�ؐG���Ă��܂���B
//=================================
class ImeProc
{
public:
	ImeProc();
	~ImeProc();
private:
//	ImeManager m_ImeManager;
	char listString[BUF_SIZE];		//��⃊�X�g�̕�����
	char imeString[BUF_SIZE];		//ime�̌��݂̕�����
	DWORD m_BlockIndex[BLOCK_SIZE];
	int m_BlockNum;
	DWORD m_ListIndex[BLOCK_SIZE];
	DWORD m_ListNum;
	DWORD m_ThisBlock;				//���݂̕��߂̔Ԓn
	DWORD m_ThisList;
	DWORD m_ThisPointNum;
	DWORD m_StringNum;
	char m_AllString[BUF_SIZE];//������o�b�t�@
	bool m_ShiftFlag;		//SHIFT�I�I�������ꂽ���ɕ����𔻕ʂ���Ƃ��Ɏg��
	int m_ThisPoint;		//���݂̎w�肳��Ă���|�C���g
	int m_StringSize;		//���݂̕�����̑傫��
	bool m_On;				//���̃t���O��ON�̂Ƃ��ɕ����񑀍�����܂��B

	void CreateListString(DWORD index,LPCANDIDATELIST);
public:
	void Init(HWND hwnd);			//������
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
	void DeletePoint(int size);	//ThisPoint�̂����Ă��镶������폜���܂��B
	BOOL Add(unsigned int key);
	int KeyShift(unsigned int key);
	int KeyNonShift(unsigned int key);
	virtual BOOL Send();
	char *GetAllString();		//������o�b�t�@�̃A�h���X���擾
	BOOL KeyDown(WPARAM);		//KeyDownProc
	BOOL KeyUp(WPARAM);			//KeyUpProc
	int GetAllStringSize();
	bool Move(int num);		//�w�肵�Ă��镶���𓮂����܂��B
	BOOL Add(char *buffer,int size);	//�������ǉ����܂��B
	void Delete();				//���݂̎w���Ă��镶�����폜���܂��B
	void SetOn(bool imeOn);		//IME��On�ɂ��܂��B�I�����C���Q�[���Ƃ�OFF�̎���������
	int GetThisPoint();			//���݂̎w���Ă���ʒu���擾���܂��B�����������Ƃ��ɓ_�ł��Ă���'|'�̈ʒu�ł��B
};
