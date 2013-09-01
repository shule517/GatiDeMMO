
#include "StdAfx.h"
#include "Lib.h"

/* ========================================================================= */
/* 宣言																		 */
/* ========================================================================= */
vector<GRAPHIC_DATA> GraphicData;

/* ========================================================================= */
/* 関数名：GetNowTime														 */
/* 内容：現在の時間の取得													 */
/* 日付：2007/09/16															 */
/* ========================================================================= */
DWORD GetNowTime( void )
{
	return GetTickCount();
}

/* ========================================================================= */
/* 関数名：GetFps															 */
/* 内容：FPSの取得															 */
/* 日付：2007/09/10															 */
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
	if( 300 <= current - last) {// 0.5秒毎に更新
		float dt = (float)(current - last) / 1000.0f;
		fps = (float)frames / dt;
		last = current;
		frames = 0;
		sprintf_s( buf, "みかん箱 %.02f fps", fps );
		SetWindowText( hwndApp, buf );
	}
}

/* ========================================================================= */
/* 関数名：GetMousePoint													 */
/* 内容：マウスカーソルの位置を取得											 */
/* 日付：2007/09/10															 */
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
/* 関数名：GetHwnd															 */
/* 内容：hwndAppの取得														 */
/* 日付：2007/09/10															 */
/* ========================================================================= */
HWND GetHwnd( void )
{
	return hwndApp;
}

/* ========================================================================= */
/* 関数名：GetDevice														 */
/* 内容：pDeviceの取得														 */
/* 日付：2007/09/10															 */
/* ========================================================================= */
LPDIRECT3DDEVICE9 GetDevice( void )
{
	return pDevice;
}

/* ========================================================================= */
/* 関数名：GetSprite														 */
/* 内容：pSpriteの取得														 */
/* 日付：2007/09/10															 */
/* ========================================================================= */
LPD3DXSPRITE GetSprite( void )
{
	return pSprite;
}

/* ========================================================================= */
/* 関数名：DrawBox															 */
/* 内容：四角形を描画														 */
/* 日付：2007/09/10															 */
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
	{ // 中を色で塗る
		hBrush	= CreateSolidBrush( Color );
		hOldBrush = (HBRUSH)SelectObject( hdc, hBrush );
	}
	else
	{ // 中を色で塗らない
		hBrush	= (HBRUSH)GetStockObject( NULL_BRUSH );
		hOldBrush = (HBRUSH)SelectObject( hdc, hBrush );
	}

	// 描画
	Rectangle( hdc, x1, y1, x1+x2, y1+y2 );

	// 元に戻す
	SelectObject( hdc, hOldPen );
	SelectObject( hdc, hOldBrush );

	// 削除
	DeleteObject( hPen );
	DeleteObject( hBrush );
	DeleteObject( hOldPen );
	DeleteObject( hOldBrush );

	pBackBuff->ReleaseDC( hdc );
}

/* ========================================================================= */
/* 関数名：DrawString														 */
/* 内容：文字列を描画														 */
/* 日付：2007/09/10															 */
/* ========================================================================= */
void DrawString( int x, int y, char *String, int Color )
{
	HDC hdc;
	pBackBuff->GetDC( &hdc );

	// アンチエイリアス
	HFONT hFont = CreateFont( 12, 5, 0, 0, 0, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, 0, "ＭＳ Ｐゴシック" );
	SelectObject( hdc, hFont );

	int OldColor	= SetTextColor( hdc, Color );		// 文字色変更
	int OldMode		= SetBkMode( hdc, TRANSPARENT );	// 背景を透明

	TextOut( hdc, x, y, String, (int)strlen( String ) );

	SetTextColor( hdc, OldColor );	// 元の色に戻す
	SetBkMode( hdc, OldMode );		// 元のモードに戻す

	DeleteObject( hFont );

	pBackBuff->ReleaseDC( hdc );
}

/* ========================================================================= */
/* 関数名：DrawRichString													 */
/* 内容：文字列に枠を付けて描画												 */
/* 日付：2007/09/17															 */
/* ========================================================================= */
void DrawRichString( int x, int y, char *String, int Color, int BackColor )
{
	// 左上
	DrawString( x - 1, y - 1, String, BackColor );
	// 上
	DrawString( x - 0, y - 1, String, BackColor );
	// 右上
	DrawString( x + 1, y - 1, String, BackColor );
	// 左
	DrawString( x - 1, y - 0, String, BackColor );
	// 右
	DrawString( x + 1, y - 0, String, BackColor );
	// 左下
	DrawString( x - 1, y + 1, String, BackColor );
	// 下
	DrawString( x - 0, y + 1, String, BackColor );
	// 右下
	DrawString( x + 1, y + 1, String, BackColor );

	// メイン
	DrawString( x, y, String, Color );
}

/* ========================================================================= */
/* 関数名：GetStringSize													 */
/* 内容：文字列の高さと幅を取得												 */
/* 日付：2007/09/17															 */
/* ========================================================================= */
void GetStringSize( char *String, int *width, int *height )
{
	HDC hdc;
	pBackBuff->GetDC( &hdc );

	// アンチエイリアス
	HFONT hFont = CreateFont( 12, 5, 0, 0, 0, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, 0, "ＭＳ Ｐゴシック" );
	SelectObject( hdc, hFont );

	SIZE size;
	GetTextExtentPoint32( hdc, String, strlen( String ), &size );

	*width = size.cx;
	*height = size.cy;

	DeleteObject( hFont );
	pBackBuff->ReleaseDC( hdc );
}

/* ========================================================================= */
/* 関数名：MB																 */
/* 内容：メッセージボックス													 */
/* 日付：2007/09/10															 */
/* ========================================================================= */
void MB( char *string, ... )
{
	char Buffer[256];
	vsprintf_s( Buffer, string, (char *)(&string + 1) );
	MessageBox( 0, Buffer, "メッセージ", 0 );
}

/* ========================================================================= */
/* 関数名：LoadGraph														 */
/* 内容：画像の読み込み														 */
/* 日付：2007/09/10															 */
/* ========================================================================= */
int LoadGraph( char *FileName )
{
	GRAPHIC_DATA	temp;

	HBITMAP			hBitmap;
	HDC				hMemDC;
	BITMAP			bmp;

	// 画像の幅と高さを取得

	// ビットマップを読み込む
	hBitmap = (HBITMAP)LoadImage( 0, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );

	// メモリデバイスコンテキストの作成
	hMemDC = CreateCompatibleDC( NULL );
	SelectObject( hMemDC, hBitmap );
	//ビットマップの情報を取得
	GetObject( hBitmap, sizeof( BITMAP ), &bmp );
	//ビットマップ削除
	DeleteObject( hBitmap );

	int w = bmp.bmWidth;
	int h = bmp.bmHeight;

	temp.texHeight = h;
	temp.texWidth = w;

	if( w < 1 || h < 1 )
		return -1;

	//テクスチャ作成
	D3DXCreateTextureFromFileEx(
		pDevice,
		FileName,					// ファイル名
		w, h, 0, 0,
		D3DFMT_A1R5G5B5,			// 色抜きを可能に
		D3DPOOL_MANAGED,
		D3DX_FILTER_LINEAR,
		D3DX_FILTER_LINEAR,
		D3DCOLOR_XRGB( 0, 0, 0 ),	// 透過色の指定
		NULL,
		NULL,
		&temp.texture				// テクスチャ名
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
/* 関数名：LoadDivGraph														 */
/* 内容：画像を分割して読み込む												 */
/* 日付：2007/09/10															 */
/* ========================================================================= */
int LoadDivGraph( char *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf )
{
	GRAPHIC_DATA	temp;

	HBITMAP			hBitmap;
	HDC				hMemDC;
	BITMAP			bmp;

	// 画像の幅と高さを取得

	// ビットマップを読み込む
	hBitmap = (HBITMAP)LoadImage( 0, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );

	// メモリデバイスコンテキストの作成
	hMemDC = CreateCompatibleDC( NULL );
	SelectObject( hMemDC, hBitmap );
	// ビットマップの情報を取得
	GetObject( hBitmap, sizeof( BITMAP ), &bmp );
	// ビットマップ削除
	DeleteObject( hBitmap );

	int w = bmp.bmWidth;
	int h = bmp.bmHeight;

	temp.texHeight = h;
	temp.texWidth = w;

	if( w < 1 && h < 1 )
		return -1;

	// テクスチャ作成
	HRESULT hr = D3DXCreateTextureFromFileEx(
		pDevice,
		FileName,					// ファイル名
		w, h, 0, 0,
		D3DFMT_A1R5G5B5,			// 色抜きを可能に
		D3DPOOL_MANAGED,
		D3DX_FILTER_LINEAR,
		D3DX_FILTER_LINEAR,
		D3DCOLOR_XRGB( 255, 0, 255 ),	// 透過色の指定
		NULL,
		NULL,
		&temp.texture				// テクスチャ名
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
/* 関数名：DrawGraph														 */
/* 内容：画像の描画															 */
/* 日付：2007/09/10															 */
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
/* 関数名：DrawGraph														 */
/* 内容：画像の描画(アルファ値)												 */
/* 日付：2007/09/10															 */
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
/* 関数名：GetInput															 */
/* 内容：インプット															 */
/* 日付：2007/09/11															 */
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
/* 関数名：GetInputOnce														 */
/* 内容：インプット															 */
/* 日付：2007/09/15															 */
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
