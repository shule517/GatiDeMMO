
#include "StdAfx.h"
#include "Lib.h"

/* ========================================================================= */
/* �錾																		 */
/* ========================================================================= */
vector<GRAPHIC_DATA> GraphicData;

/* ========================================================================= */
/* �֐����FGetNowTime														 */
/* ���e�F���݂̎��Ԃ̎擾													 */
/* ���t�F2007/09/16															 */
/* ========================================================================= */
DWORD GetNowTime( void )
{
	return GetTickCount();
}

/* ========================================================================= */
/* �֐����FGetFps															 */
/* ���e�FFPS�̎擾															 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
void GetFps( void )
{
	static DWORD    last = timeGetTime();
	static DWORD    frames = 0;
	static char     buf[256] = "";
	DWORD           current;

	current = timeGetTime();
	frames++;

	static  float fps;
	if( 300 <= current - last) {// 0.5�b���ɍX�V
		float dt = (float)(current - last) / 1000.0f;
		fps = (float)frames / dt;
		last = current;
		frames = 0;
		sprintf_s( buf, "�݂��� %.02f fps", fps );
		SetWindowText( hwndApp, buf );
	}
}

/* ========================================================================= */
/* �֐����FGetMousePoint													 */
/* ���e�F�}�E�X�J�[�\���̈ʒu���擾											 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
void GetMousePoint( int *x, int *y )
{
	POINT pt;

	GetCursorPos( &pt );
	ScreenToClient( hwndApp, &pt );
	(*x) = pt.x;
	(*y) = pt.y;
}

/* ========================================================================= */
/* �֐����FGetHwnd															 */
/* ���e�FhwndApp�̎擾														 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
HWND GetHwnd( void )
{
	return hwndApp;
}

/* ========================================================================= */
/* �֐����FGetDevice														 */
/* ���e�FpDevice�̎擾														 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
LPDIRECT3DDEVICE9 GetDevice( void )
{
	return pDevice;
}

/* ========================================================================= */
/* �֐����FGetSprite														 */
/* ���e�FpSprite�̎擾														 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
LPD3DXSPRITE GetSprite( void )
{
	return pSprite;
}

/* ========================================================================= */
/* �֐����FDrawBox															 */
/* ���e�F�l�p�`��`��														 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
void DrawBox( int x1, int y1, int x2, int y2, int Color, int FillFlag )
{
	HDC hdc;
	pBackBuff->GetDC( &hdc );

	HPEN hPen, hOldPen;
	HBRUSH hBrush, hOldBrush;

	hPen	= CreatePen( PS_SOLID, 0, Color );
	hOldPen	= (HPEN)SelectObject( hdc, hPen );

	if( FillFlag )
	{ // ����F�œh��
		hBrush	= CreateSolidBrush( Color );
		hOldBrush = (HBRUSH)SelectObject( hdc, hBrush );
	}
	else
	{ // ����F�œh��Ȃ�
		hBrush	= (HBRUSH)GetStockObject( NULL_BRUSH );
		hOldBrush = (HBRUSH)SelectObject( hdc, hBrush );
	}

	// �`��
	Rectangle( hdc, x1, y1, x1+x2, y1+y2 );

	// ���ɖ߂�
	SelectObject( hdc, hOldPen );
	SelectObject( hdc, hOldBrush );

	// �폜
	DeleteObject( hPen );
	DeleteObject( hBrush );
	DeleteObject( hOldPen );
	DeleteObject( hOldBrush );

	pBackBuff->ReleaseDC( hdc );
}

/* ========================================================================= */
/* �֐����FDrawString														 */
/* ���e�F�������`��														 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
void DrawString( int x, int y, char *String, int Color )
{
	HDC hdc;
	pBackBuff->GetDC( &hdc );

	// �A���`�G�C���A�X
	HFONT hFont = CreateFont( 12, 5, 0, 0, 0, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, 0, "�l�r �o�S�V�b�N" );
	SelectObject( hdc, hFont );

	int OldColor	= SetTextColor( hdc, Color );		// �����F�ύX
	int OldMode		= SetBkMode( hdc, TRANSPARENT );	// �w�i�𓧖�

	TextOut( hdc, x, y, String, (int)strlen( String ) );

	SetTextColor( hdc, OldColor );	// ���̐F�ɖ߂�
	SetBkMode( hdc, OldMode );		// ���̃��[�h�ɖ߂�

	DeleteObject( hFont );

	pBackBuff->ReleaseDC( hdc );
}

/* ========================================================================= */
/* �֐����FDrawRichString													 */
/* ���e�F������ɘg��t���ĕ`��												 */
/* ���t�F2007/09/17															 */
/* ========================================================================= */
void DrawRichString( int x, int y, char *String, int Color, int BackColor )
{
	// ����
	DrawString( x - 1, y - 1, String, BackColor );
	// ��
	DrawString( x - 0, y - 1, String, BackColor );
	// �E��
	DrawString( x + 1, y - 1, String, BackColor );
	// ��
	DrawString( x - 1, y - 0, String, BackColor );
	// �E
	DrawString( x + 1, y - 0, String, BackColor );
	// ����
	DrawString( x - 1, y + 1, String, BackColor );
	// ��
	DrawString( x - 0, y + 1, String, BackColor );
	// �E��
	DrawString( x + 1, y + 1, String, BackColor );

	// ���C��
	DrawString( x, y, String, Color );
}

/* ========================================================================= */
/* �֐����FGetStringSize													 */
/* ���e�F������̍����ƕ����擾												 */
/* ���t�F2007/09/17															 */
/* ========================================================================= */
void GetStringSize( char *String, int *width, int *height )
{
	HDC hdc;
	pBackBuff->GetDC( &hdc );

	// �A���`�G�C���A�X
	HFONT hFont = CreateFont( 12, 5, 0, 0, 0, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, 0, "�l�r �o�S�V�b�N" );
	SelectObject( hdc, hFont );

	SIZE size;
	GetTextExtentPoint32( hdc, String, strlen( String ), &size );

	*width = size.cx;
	*height = size.cy;

	DeleteObject( hFont );
	pBackBuff->ReleaseDC( hdc );
}

/* ========================================================================= */
/* �֐����FMB																 */
/* ���e�F���b�Z�[�W�{�b�N�X													 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
void MB( char *string, ... )
{
	char Buffer[256];
	vsprintf_s( Buffer, string, (char *)(&string + 1) );
	MessageBox( 0, Buffer, "���b�Z�[�W", 0 );
}

/* ========================================================================= */
/* �֐����FLoadGraph														 */
/* ���e�F�摜�̓ǂݍ���														 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
int LoadGraph( char *FileName )
{
	GRAPHIC_DATA	temp;

	HBITMAP			hBitmap;
	HDC				hMemDC;
	BITMAP			bmp;

	// �摜�̕��ƍ������擾

	// �r�b�g�}�b�v��ǂݍ���
	hBitmap = (HBITMAP)LoadImage( 0, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );

	// �������f�o�C�X�R���e�L�X�g�̍쐬
	hMemDC = CreateCompatibleDC( NULL );
	SelectObject( hMemDC, hBitmap );
	//�r�b�g�}�b�v�̏����擾
	GetObject( hBitmap, sizeof( BITMAP ), &bmp );
	//�r�b�g�}�b�v�폜
	DeleteObject( hBitmap );

	int w = bmp.bmWidth;
	int h = bmp.bmHeight;

	temp.texHeight = h;
	temp.texWidth = w;

	if( w < 1 || h < 1 )
		return -1;

	//�e�N�X�`���쐬
	D3DXCreateTextureFromFileEx(
		pDevice,
		FileName,					// �t�@�C����
		w, h, 0, 0,
		D3DFMT_A1R5G5B5,			// �F�������\��
		D3DPOOL_MANAGED,
		D3DX_FILTER_LINEAR,
		D3DX_FILTER_LINEAR,
		D3DCOLOR_XRGB( 0, 0, 0 ),	// ���ߐF�̎w��
		NULL,
		NULL,
		&temp.texture				// �e�N�X�`����
	);

	SetRect( &temp.rct, 0, 0, 0, 0 );

	if( temp.texture != 0)
	{
		GraphicData.push_back( temp );
		return GraphicData.size() - 1;
	}

	return -1;
}

/* ========================================================================= */
/* �֐����FLoadDivGraph														 */
/* ���e�F�摜�𕪊����ēǂݍ���												 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
int LoadDivGraph( char *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf )
{
	GRAPHIC_DATA	temp;

	HBITMAP			hBitmap;
	HDC				hMemDC;
	BITMAP			bmp;

	// �摜�̕��ƍ������擾

	// �r�b�g�}�b�v��ǂݍ���
	hBitmap = (HBITMAP)LoadImage( 0, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );

	// �������f�o�C�X�R���e�L�X�g�̍쐬
	hMemDC = CreateCompatibleDC( NULL );
	SelectObject( hMemDC, hBitmap );
	// �r�b�g�}�b�v�̏����擾
	GetObject( hBitmap, sizeof( BITMAP ), &bmp );
	// �r�b�g�}�b�v�폜
	DeleteObject( hBitmap );

	int w = bmp.bmWidth;
	int h = bmp.bmHeight;

	temp.texHeight = h;
	temp.texWidth = w;

	if( w < 1 && h < 1 )
		return -1;

	// �e�N�X�`���쐬
	HRESULT hr = D3DXCreateTextureFromFileEx(
		pDevice,
		FileName,					// �t�@�C����
		w, h, 0, 0,
		D3DFMT_A1R5G5B5,			// �F�������\��
		D3DPOOL_MANAGED,
		D3DX_FILTER_LINEAR,
		D3DX_FILTER_LINEAR,
		D3DCOLOR_XRGB( 255, 0, 255 ),	// ���ߐF�̎w��
		NULL,
		NULL,
		&temp.texture				// �e�N�X�`����
	);

	if (FAILED(hr))
		return false;

	SetRect( &temp.rct, 0, 0, 0, 0 );

	if( temp.texture != 0)
	{
		for( int y = 0; y < YNum; y++ )
		{
			for( int x = 0; x < XNum; x++ )
			{
				SetRect( &temp.rct, XSize*x, YSize*y, XSize*x+XSize, YSize*y+YSize );
				GraphicData.push_back( temp );
				HandleBuf[x+(y*XNum)] = GraphicData.size() - 1;
			}
		}
		return 1;
	}

	return -1;
}

// testtttttttttttttttttttttttttt
void SetMaterial(D3DMATERIAL9 *material, float r, float g, float b, float a)
{
	ZeroMemory(material, sizeof(D3DMATERIAL9));
	material->Diffuse.r = r;
	material->Diffuse.g = g;
	material->Diffuse.b = b;
	material->Diffuse.a = a;
	material->Ambient.r = r;
	material->Ambient.g = g;
	material->Ambient.b = b;
	material->Ambient.a = a;
	material->Specular.r = 1.0f;
	material->Specular.g = 1.0f;
	material->Specular.b = 1.0f;
	material->Specular.a = 1.0f;
	material->Emissive.r = 0;
	material->Emissive.g = 0;
	material->Emissive.b = 0;
	material->Power = 10;
}

/* ========================================================================= */
/* �֐����FDrawGraph														 */
/* ���e�F�摜�̕`��															 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
void DrawGraph( int x, int y, int GrHandle )
{

	/*
	float size = 20.0f;
	LPDIRECT3DVERTEXBUFFER9 pVB;
	D3DXVECTOR3 Vec;
	D3DMATERIAL9 Mat;
	/***********************************************
//	Vec = vec;
	pVB = 0;
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(D3DVERTEX) * 4, 0, D3DVERTEX::FVF, D3DPOOL_MANAGED, &pVB, 0)))
		return;

	D3DVERTEX *pV;
	pVB->Lock(0, 0, (void **)&pV, 0);

	pV[0].Set(-size / 2, 0.0f, -size / 2, 0, 1, 0, 0, 0);
	pV[1].Set(-size / 2, 0.0f,  size / 2, 0, 1, 0, 0, 1);
	pV[2].Set( size / 2, 0.0f,  size / 2, 0, 1, 0, 1, 1);
	pV[3].Set( size / 2, 0.0f, -size / 2, 0, 1, 0, 1, 0);

	pVB->Unlock();

	/*
	if (!Tex.LoadImage(pD3DDevice, "../data/magic_a.png"))
		return;
	*

	SetMaterial( &Mat, 1.0, 1.0, 1.0, 1.0 );
	/***********************************************

	D3DXMATRIX m, rot;
//	D3DXMatrixRotationY( &rot, Angle );
//	D3DXMatrixTranslation( &m, Vec.x, Vec.y + 3.5f, Vec.z );
//	D3DXMatrixMultiply( &m, &rot, &m );

	D3DXMatrixTranslation( &m, 0, 0 + 3.5f, 0 );

	pDevice->SetTransform( D3DTS_WORLD, &m );
	pDevice->SetMaterial( &Mat );
	pDevice->SetTexture( 0, GraphicData.at( GrHandle ).texture );
	pDevice->SetFVF( D3DVERTEX::FVF );
	pDevice->SetStreamSource( 0, pVB, 0, sizeof( D3DVERTEX ) );
	pDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, 2 );


	*/
	GRAPHIC_DATA graphic = GraphicData.at( GrHandle );

	float x2 = (float)(x + graphic.rct.right - graphic.rct.left);
	float y2 = (float)(y + graphic.rct.bottom - graphic.rct.top);
	float sx1 = (float)(graphic.rct.left);
	float sy1 = (float)(graphic.rct.top);
	float sx2 = (float)(graphic.rct.right);
	float sy2 = (float)(graphic.rct.bottom);

	DWORD color = D3DCOLOR_RGBA( 255, 255, 255, 255 );

	D3DTLVERTEX v[] =
	{
		{ x  - 0.5f, y  - 0.5f, 0, 1, color, sx1 / graphic.texWidth, sy1 / graphic.texHeight },
		{ x2 - 0.5f, y  - 0.5f, 0, 1, color, sx2 / graphic.texWidth, sy1 / graphic.texHeight },
		{ x  - 0.5f, y2 - 0.5f, 0, 1, color, sx1 / graphic.texWidth, sy2 / graphic.texHeight },
		{ x2 - 0.5f, y2 - 0.5f, 0, 1, color, sx2 / graphic.texWidth, sy2 / graphic.texHeight },
	};

	pDevice->SetTexture( 0, graphic.texture );
	pDevice->SetFVF( D3DTLVERTEX::FVF );
	pDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof( D3DTLVERTEX ) );
}

/* ========================================================================= */
/* �֐����FDrawGraph														 */
/* ���e�F�摜�̕`��(�A���t�@�l)												 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
void DrawGraph( int x, int y, int GrHandle, int Alpha )
{
	GRAPHIC_DATA graphic = GraphicData.at( GrHandle );

	float x2 = (float)(x + graphic.rct.right - graphic.rct.left);
	float y2 = (float)(y + graphic.rct.bottom - graphic.rct.top);
	float sx1 = (float)(graphic.rct.left);
	float sy1 = (float)(graphic.rct.top);
	float sx2 = (float)(graphic.rct.right);
	float sy2 = (float)(graphic.rct.bottom);

	DWORD color = D3DCOLOR_RGBA( 255, 255, 255, Alpha );

	D3DTLVERTEX v[] =
	{
		{ x  - 0.5f, y  - 0.5f, 0, 1, color, sx1 / graphic.texWidth, sy1 / graphic.texHeight },
		{ x2 - 0.5f, y  - 0.5f, 0, 1, color, sx2 / graphic.texWidth, sy1 / graphic.texHeight },
		{ x  - 0.5f, y2 - 0.5f, 0, 1, color, sx1 / graphic.texWidth, sy2 / graphic.texHeight },
		{ x2 - 0.5f, y2 - 0.5f, 0, 1, color, sx2 / graphic.texWidth, sy2 / graphic.texHeight },
	};

	pDevice->SetTexture( 0, graphic.texture );
	pDevice->SetFVF( D3DTLVERTEX::FVF );
	pDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof( D3DTLVERTEX ) );
}


/* ========================================================================= */
/* �֐����FGetInput															 */
/* ���e�F�C���v�b�g															 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
int GetInput()
{
	int key = 0;
	unsigned char KeyBuffer[256];

	GetKeyboardState( KeyBuffer );

	if( KeyBuffer[ VK_LBUTTON ] & 0x80 )
		key |= MOUSE_INPUT_LEFT ;
	if( KeyBuffer[ VK_RBUTTON ] & 0x80 )
		key |= MOUSE_INPUT_RIGHT ;
	if( KeyBuffer[ VK_MBUTTON ] & 0x80 )
		key |= MOUSE_INPUT_MIDDLE ;

	return key;
}

int BeforeInputBuffer = 0;

/* ========================================================================= */
/* �֐����FGetInputOnce														 */
/* ���e�F�C���v�b�g															 */
/* ���t�F2007/09/15															 */
/* ========================================================================= */
int GetInputOnce()
{
	int InputBuffer = 0;
	unsigned char KeyBuffer[256];

	int DownInputBuffer = 0;
	int UpInputBuffer = 0;

	GetKeyboardState( KeyBuffer );

	if( KeyBuffer[ VK_LBUTTON ] & 0x80 )	InputBuffer |= MOUSE_INPUT_LEFT;
	if( KeyBuffer[ VK_RBUTTON ] & 0x80 )	InputBuffer |= MOUSE_INPUT_RIGHT;
	if( KeyBuffer[ VK_MBUTTON ] & 0x80 )	InputBuffer |= MOUSE_INPUT_MIDDLE;

	DownInputBuffer	= InputBuffer & ~BeforeInputBuffer;
	UpInputBuffer	= ~InputBuffer & BeforeInputBuffer;

	BeforeInputBuffer = InputBuffer;

	return DownInputBuffer;
}
