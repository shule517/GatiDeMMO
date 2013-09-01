
#include "StdAfx.h"

/* ========================================================================= */
/* �錾�F�A�v���P�[�V�����f�[�^												 */
/* ========================================================================= */
HWND	hwndApp;
bool	bFullScreen = false;

/* ========================================================================= */
/* �錾�FDirectX�I�u�W�F�N�g												 */
/* ========================================================================= */
LPDIRECT3D9				pD3D		= NULL;
LPDIRECT3DDEVICE9		pDevice		= NULL;
LPDIRECT3DSURFACE9		pBackBuff	= NULL;		//�o�b�N�o�b�t�@
LPD3DXSPRITE			pSprite		= NULL;		//�X�v���C�g
D3DPRESENT_PARAMETERS	d3dpp;

/* ========================================================================= */
/* �֐����FInitApp															 */
/* ���e�F�A�v���P�[�V�����̏�����											 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
bool InitApp( HINSTANCE hInst, int nCmdShow )
{
	int			iWidth, iHeight;
	WNDCLASSEX	wndclass;

	// 256�F�ȉ��̃��[�h�ŋN���s��
	HDC dc = GetDC( 0 );
	int iBit = GetDeviceCaps( dc, BITSPIXEL );
	ReleaseDC( 0, dc );
	if( iBit < 15 ) {
		MB( "256�F�ȉ��ł̓v���C�ł��܂���" );
		return false;
	}

	// �E�B���h�E�N���X�̓o�^
	wndclass.cbSize 		= sizeof( WNDCLASSEX );
	wndclass.style			= CS_CLASSDC;
	wndclass.lpfnWndProc	= (WNDPROC)WndProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hbrBackground	= (HBRUSH)GetStockObject( BLACK_BRUSH );
	wndclass.hInstance		= hInst;
	wndclass.hIcon			= NULL;
	wndclass.hCursor		= LoadCursor( NULL, IDC_ARROW );
	wndclass.lpszMenuName	= NULL;
	wndclass.lpszClassName	= CLASS_NAME;
	wndclass.hIconSm		= NULL;

	if( !RegisterClassEx( &wndclass ) ) return false;

	// ���C���E�B���h�E�쐬
	iWidth	= WIDTH + GetSystemMetrics( SM_CXDLGFRAME ) * 2;
	iHeight	= HEIGHT + GetSystemMetrics( SM_CYDLGFRAME ) * 2 + GetSystemMetrics( SM_CYCAPTION );

	// ���C���E�B���h�E
	hwndApp = CreateWindowEx( 0, CLASS_NAME, TITLE_NAME,
							WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
							CW_USEDEFAULT, CW_USEDEFAULT, iWidth, iHeight,
							(HWND)NULL, (HMENU)NULL,
							hInst, (LPSTR)NULL );
	ShowWindow( hwndApp, nCmdShow );
	UpdateWindow( hwndApp );

	// �����g�p
	srand( GetTickCount() );
/***********************/
	imeProc.Init( hwndApp );
/***********************/
	return true;
}

/* ========================================================================= */
/* �֐����FInitD3D															 */
/* ���e�FDirect3D�̏�����													 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
bool InitD3D( void )
{
	D3DDISPLAYMODE dsp;

	// Direct3D�I�u�W�F�N�g�̎擾
	pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if( pD3D == NULL ) return false;

	// ���݂̃f�B�X�v���C���[�h�𓾂�
	if( FAILED( pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &dsp ) ) ) return false;

	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // ���������Ńt���b�v
	if( bFullScreen )
	{	// �t���X�N���[�����[�h
		d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
		d3dpp.Windowed = 0;
	}
	else
	{	// �E�B���h�E���[�h
		d3dpp.BackBufferFormat = dsp.Format;
		d3dpp.Windowed = 1;
	}

	d3dpp.BackBufferWidth			= WIDTH;
	d3dpp.BackBufferHeight			= HEIGHT;
	d3dpp.BackBufferCount			= 1;
	d3dpp.EnableAutoDepthStencil	= TRUE;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;
	d3dpp.Flags						= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	// �f�o�C�X�̍쐬
	// T&L HAL
	if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwndApp, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pDevice ) ) )
	{
		// HAL
		if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwndApp, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice ) ) )
		{
			// HEL
			if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hwndApp, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice ) ) )
			{
				return false;
			}
		}
	}

	// �o�b�N�o�b�t�@�擾
	if( FAILED( pDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuff ) ) )
		return false;

	// �X�v���C�g�쐬
	if( FAILED( D3DXCreateSprite( pDevice, &pSprite ) ) ) return false;

	return true;
}

/* ========================================================================= */
/* �֐����FInitRender														 */
/* ���e�F�`��̐ݒ�															 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
bool InitRender( void )
{
	float			fAspect;
	D3DVIEWPORT9	vp;
	D3DXMATRIX		mat;

	pDevice->GetViewport( &vp );
	fAspect = (float)vp.Width / (float)vp.Height;

	// �ϊ��s��̐ݒ�
	D3DXMatrixPerspectiveFovLH( &mat, D3DXToRadian( 45.0f ), fAspect, 0.1f, 100.0f );
	pDevice->SetTransform( D3DTS_PROJECTION, &mat );
	D3DXMatrixLookAtLH( &mat, &D3DXVECTOR3( 0.0f, 0.0f, -20.0f ), &D3DXVECTOR3( 0, 0, 0), &D3DXVECTOR3( 0, 1, 0 ) );
	pDevice->SetTransform( D3DTS_VIEW, &mat );

	// �`��ݒ�
	pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );	// �J�����O�͍s��Ȃ�
	pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );			// ���C�e�B���O���Ȃ�

	return true;
}

/* ========================================================================= */
/* �֐����FReleaseApp														 */
/* ���e�F�I���̏���															 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
void ReleaseApp( void )
{
	RELEASE( pBackBuff );
	RELEASE( pDevice );
	RELEASE( pD3D );
}

/* ========================================================================= */
/* �֐����FPresent															 */
/* ���e�F�摜�̓]��															 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
bool Present( void )
{
	if( FAILED( pDevice->Present( NULL, NULL, NULL, NULL) ) )
	{
		if( FAILED( pDevice->Reset( &d3dpp ) ) ) return false;
	}
	return true;
}


/************************/
bool flag = 0;
ImeProc imeProc;
/* ========================================================================= */
/* �֐����FWndProc															 */
/* ���e�F�E�B���h�E�v���V�[�W��												 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, UINT wParam, LONG lParam )
{
	switch( msg )
	{
	/****************************************************/
	case WM_IME_SETCONTEXT:
		flag = 1;
		return imeProc.SetCon(lParam);
	case WM_KEYDOWN:
		flag = 1;
		return imeProc.KeyDown(wParam);
	case WM_KEYUP:
		flag = 1;
		return imeProc.KeyUp(wParam);
	case WM_IME_COMPOSITION:
		flag = 1;
		return imeProc.Compo(lParam);
	case WM_IME_STARTCOMPOSITION:
		flag = 1;
		return imeProc.StartCompo();
	case WM_IME_ENDCOMPOSITION:
		flag = 1;
		return imeProc.EndCompo();
	case WM_IME_NOTIFY:
		flag = 1;
		return imeProc.Nofity(wParam,hWnd);
	/****************************************************/
	case WM_MOUSEMOVE:
		ShowCursor( FALSE );
		break;
/*
	case WM_KEYDOWN:
		switch( wParam )
		{
		case VK_ESCAPE:
			PostMessage( hWnd, WM_CLOSE, 0, 0 );
			break;
		default:
			break;
		}
		break;
*/
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	default:
		return DefWindowProc( hWnd, msg, wParam, lParam );
	}

	return 0L;
}
