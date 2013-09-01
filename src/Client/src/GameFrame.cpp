
#include "StdAfx.h"

/* ========================================================================= */
/* �錾�Fstatic																 */
/* ========================================================================= */
float			CGameFrame::idouryou[20] = {};
unsigned int	CGameFrame::animcounter = 0;

/* ========================================================================= */
/* �֐����FCGameFrame::CGameFrame											 */
/* ���e�F�R���X�g���N�^														 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
CGameFrame::CGameFrame( void )
{
}

/* ========================================================================= */
/* �֐����FCGameFrame::~CGameFrame											 */
/* ���e�F�f�X�g���N�^														 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
CGameFrame::~CGameFrame( void )
{
}

/* ========================================================================= */
/* �֐����FCGameFrame::~CGameFrame											 */
/* ���e�F�Q�[���{�̂�`��													 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
void CGameFrame::drawGameMain( void )
{
	int x, y;
	GetMousePoint( &x, &y );
	DrawString( 10, 10, "�ւ����I�@( ^��^)���[���h(^��^ )", RGB( 255, 0, 0 ) );
	DrawString( x, y, "( ^��^)", RGB( 255, 0, 0 ) );
}

/* ========================================================================= */
/* �֐����FCGameFrame::doMainLoop											 */
/* ���e�F���C�����[�v														 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
void CGameFrame::doMainLoop( HINSTANCE hInst, int nCmdShow )
{
	bool flag = true;

	// �A�v���P�[�V�����Ɋւ��鏉����
	if( flag ) InitApp( hInst, nCmdShow );

	// Direct3D�Ɋւ��鏉����
	if( flag ) flag = InitD3D();
	if( flag ) flag = InitRender();

	// �ړ��␳
	setIdouHosei();

	try
	{
		loadFiles();					//�e�t�@�C���̓ǂݍ���
	}
	catch( int errcode )
	{
		MB( "�t�@�C���ǂݍ��݃G���[\nerrcode�F%d", errcode );
		return;
	}

	/*****************/
	initStage();
	/*****************/

	// ���b�Z�[�W���[�v
	MSG msg;
    ZeroMemory( &msg, sizeof MSG );

	while( flag )
	{
		// FPS�̎擾
		GetFps();

		// �E�B���h�E�������Ă��鎞�����`�悷�邽�߂̏���
		WINDOWPLACEMENT wndpl;
		GetWindowPlacement( GetHwnd(), &wndpl ); // �E�C���h�E�̏�Ԃ��擾
		if( ( wndpl.showCmd != SW_HIDE ) &&
			( wndpl.showCmd != SW_MINIMIZE ) &&
			( wndpl.showCmd != SW_SHOWMINIMIZED ) &&
			( wndpl.showCmd != SW_SHOWMINNOACTIVE ) )
		{
			LPDIRECT3DDEVICE9	pDevice = GetDevice();
			LPD3DXSPRITE		pSprite = GetSprite();

			// ��ʂ̏���
			pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0, 0 );

			// �X�v���C�g�`��
			if( SUCCEEDED( pDevice->BeginScene() ) )
			{
				if( SUCCEEDED( pSprite->Begin() ) )
				{
					switch( 1 )
					{
					case 1:
						drawGameMain();
						break;
					}

					pSprite->End();
				}
			}
			pDevice->EndScene();

			// ��ʂ̓]��
			if( !Present() )
				flag = false;

			animcounter++;	// �A�j���J�E���^�[
		}

		// ���b�Z�[�W�̏���
		while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			if( !GetMessage( &msg, NULL, 0, 0 ) )
			{
				flag = false;
				break;
			}
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		Sleep( 1 );
	}

	// �I�����̏���
	ReleaseApp();
}

/* ========================================================================= */
/* �֐����FCGameFrame::setIdouHosei											 */
/* ���e�F�ړ��␳															 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
void CGameFrame::setIdouHosei( void )
{
	float frametime = 0;
//	unsigned int s = GetNowCount();
	unsigned int s = GetTickCount();
	for( int i = 0; i < 30; i++ )
	{
		LPDIRECT3DDEVICE9	pDevice = GetDevice();
		LPD3DXSPRITE		pSprite = GetSprite();

		// ��ʂ̏���
		pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0, 0 );

		// �X�v���C�g�`��
		if( SUCCEEDED( pDevice->BeginScene() ) )
		{
			if( SUCCEEDED( pSprite->Begin() ) )
			{

				pSprite->End();
			}
		}
		pDevice->EndScene();

		// ��ʂ̓]��
		Present();

		unsigned int e = GetTickCount();
		frametime = frametime + e - s;
		s = e;
	}
	frametime = frametime / 30;
	for( int i = 1; i < 20; i++ )
	{
		idouryou[i] = (float)(i / frametime * 16.67);
	}
}

/* ========================================================================= */
/* �֐����FCGameFrame::IdouHosei											 */
/* ���e�F�ړ��␳���擾														 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
float CGameFrame::IdouHosei( int num )
{
	if( num < 0 )
	{
		return( -idouryou[-num] );
	}
	else
	{
		return( idouryou[num] );
	}
}

/* ========================================================================= */
/* �֐����FCGameFrame::getAnimCnt											 */
/* ���e�Fanimcounter���擾													 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
unsigned int CGameFrame::getAnimCnt()
{
	return animcounter;
}
