
#include "StdAfx.h"
#include "Lib.h"

/* ========================================================================= */
/* �錾																		 */
/* ========================================================================= */
list<EDITBOX_DATA> EditList;

/* ========================================================================= */
/* �֐����FCreateEditBox													 */
/* ���e�F�G�f�B�b�g�{�b�N�X���쐬											 */
/* ���t�F2007/09/16															 */
/* ========================================================================= */
int CreateEditBox( void )
{
	EDITBOX_DATA edit;
	edit.Height = 10;
	edit.Width = 100;
	sprintf_s( edit.String, "" );
	edit.edit_no = EditList.size();

	EditList.push_back( edit );

	return ( edit.edit_no );
}

/* ========================================================================= */
/* �֐����FDrawEditBox														 */
/* ���e�F�G�f�B�b�g�{�b�N�X�̕`��Ə���										 */
/* ���t�F2007/09/16															 */
/* ========================================================================= */
void DrawEditBox( int EditNo, int x, int y )
{
	/*
	list<EDITBOX_DATA>::iterator it;
	it = EditList.begin();

	while( it != EditList.end() )
	{
		if( (*it).edit_no == EditNo )
		{
			DrawString( x, y, (*it).String, RGB( 128, 128, 255 ) );
			return;
		}
		++it;
	}
	*/
/*
	char data[256];
	int b = imeProc.GetBlockNum();
	int c = imeProc.GetListNum();
	char *d = imeProc.GetListString();
	DWORD e = imeProc.GetSeclect();
	int f = imeProc.GetThisList();
	int g = imeProc.GetThisPoint();

	sprintf_s( data, "GetBlockNum = %d  �ϊ�����Ƃ��ɉ��ɕ�����邩", b );
	DrawString( x+20, y-220, data, RGB( 128, 128, 255 ) );
	sprintf_s( data, "GetListNum = %d   �ύX���̐�", c );
	DrawString( x+20, y-240, data, RGB( 128, 128, 255 ) );
	sprintf_s( data, "GetSeclect = %f   �ϊ����X�g�̑I�����Ă���ԍ��|�P", e );
	DrawString( x+20, y-260, data, RGB( 128, 128, 255 ) );
	sprintf_s( data, "GetThisList = %d", f );
	DrawString( x+20, y-280, data, RGB( 128, 128, 255 ) );
	sprintf_s( data, "GetThisPoint = %d ���C��������̒���", g );
	DrawString( x+20, y-200, data, RGB( 128, 128, 255 ) );
	sprintf_s( data, "GetThisPointNum = %d �J�[�\���̈ʒu", imeProc.GetThisPointNum() );
	DrawString( x+20, y-300, data, RGB( 128, 128, 255 ) );
*/
	// ���C��
	char buffer[BUF_SIZE];
	memcpy(buffer,imeProc.GetAllString(),imeProc.GetThisPoint());
	buffer[imeProc.GetThisPoint()] = '\0';

	char *dat = imeProc.GetAllString();
	char buffer2[BUF_SIZE];
	memcpy(&buffer2[0],&dat[strlen(buffer)], imeProc.GetAllStringSize() - strlen(buffer));
	buffer2[imeProc.GetAllStringSize() - strlen(buffer)] = '\0';

	char *Buf = imeProc.GetAllString();

	int width, height;
	GetStringSize( buffer, &width, &height );
	int width2, height2;
	GetStringSize( imeProc.GetImeString(), &width2, &height2 );

	// �J�[�\�����E�̕����̕`��
//	DrawString( x + width + width2, y, buffer2, RGB( 255, 255, 255 ) );
	DrawRichString( x + width + width2, y, buffer2, RGB( 255, 255, 255 ), RGB( 0, 0, 0 ) );
	// �J�[�\����荶�̕����̕`��
//	DrawString( x, y, buffer, RGB( 255, 255, 255 ) );
	DrawRichString( x, y, buffer, RGB( 255, 255, 255 ), RGB( 0, 0, 0) );
	// IME�̕����`��
//	DrawString( x + width, y, imeProc.GetImeString(), RGB( 128, 128, 255 ) );
	DrawRichString( x + width, y, imeProc.GetImeString(), RGB( 128, 128, 255 ), RGB( 0, 0, 0 ) );

	/***********************************/
	int num = imeProc.GetBlockNum();

	DWORD theListStart = 0;
	for( int i = 0; i < num; i++ )
	{
		char tmp[256];
		bool theNew;
		int theSize;
		imeProc.GetImeBlockString(i,tmp,&theSize,&theNew);

		static int len = 0;
		static bool flag = true;
		if( i == 0 ) flag = true;
		if( theNew )
		{
//			DrawString( x + width + len, y, tmp, RGB( 0, 255, 0 ) );
			DrawRichString( x + width + len, y, tmp, RGB( 0, 255, 0 ), RGB( 0 , 0, 0 ) );
			flag = false;
			len = 0;
		}
		if( flag )
		{
			int tmp_x, tmp_y;
			GetStringSize( tmp, &tmp_x, &tmp_y );
			len += tmp_x;
		}
	}
	/***********************************/

	// �����̕`��
	num = imeProc.GetListNum();
	for(int i = 0; i < num;i++)
	{
		char tmp[256];
		bool theNew;
		int theSize;
		imeProc.GetListBlockString(i,tmp,&theSize,&theNew);
		if( i == imeProc.GetThisList() )
		{
//			DrawString( x, y+20+i*20, tmp, RGB( 255, 0, 0 ) );
			DrawRichString( x, y+20+i*20, tmp, RGB( 255, 0, 0 ), RGB( 0, 0, 0 ) );
		}
		else
		{
//			DrawString( x, y+20+i*20, tmp, RGB( 128, 128, 255 ) );
			DrawRichString( x, y+20+i*20, tmp, RGB( 128, 128, 255 ), RGB( 0, 0, 0 ) );
		}
	}

	// �J�[�\���̕\��
	static int i = 0;
	if( i > 20 )
	{
		DrawString( x + width + width2 - 2, y, "|", RGB( 255, 0, 0 ) );
		if( i > 40 )
			i = 0;
	}
	i++;

	DrawBox( x-2, y-2, 400, 16, RGB( 255, 255, 0 ), FALSE );
}

/* ========================================================================= */
/* �֐����FGetEditBoxText													 */
/* ���e�F�G�f�B�b�g�{�b�N�X�̕������擾										 */
/* ���t�F2007/09/16															 */
/* ========================================================================= */
char* GetEditText( int EditNo )
{
	list<EDITBOX_DATA>::iterator it;
	it = EditList.begin();

	while( it != EditList.end() )
	{
		if( (*it).edit_no == EditNo )
		{
			return (*it).String;
		}
		++it;
	}

	return "�G�f�B�b�g�{�b�N�X�̕������擾�ł��܂���ł����B";
}

/* ========================================================================= */
/* �֐����FSetEditBoxText													 */
/* ���e�F�G�f�B�b�g�{�b�N�X�̕�����ύX										 */
/* ���t�F2007/09/16															 */
/* ========================================================================= */
void SetEditText( int EditNo, char *String )
{
	list<EDITBOX_DATA>::iterator it;
	it = EditList.begin();

	while( it != EditList.end() )
	{
		if( (*it).edit_no == EditNo )
		{
			sprintf_s( (*it).String, "%s", String );
			return;
		}
		++it;
	}
}

/* ========================================================================= */
/* �֐����FDeleteEditBox													 */
/* ���e�F�G�f�B�b�g�{�b�N�X���폜											 */
/* ���t�F2007/09/16															 */
/* ========================================================================= */
void DeleteEditBox( int EditNo )
{
	list<EDITBOX_DATA>::iterator it;
	it = EditList.begin();

	while( it != EditList.end() )
	{
		if( (*it).edit_no == EditNo )
		{
			EditList.erase( it );
			return;
		}
		++it;
	}
}
