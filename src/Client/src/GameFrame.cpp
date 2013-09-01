
#include "StdAfx.h"

/* ========================================================================= */
/* 宣言：static																 */
/* ========================================================================= */
float			CGameFrame::idouryou[20] = {};
unsigned int	CGameFrame::animcounter = 0;

/* ========================================================================= */
/* 関数名：CGameFrame::CGameFrame											 */
/* 内容：コンストラクタ														 */
/* 日付：2007/09/10															 */
/* ========================================================================= */
CGameFrame::CGameFrame( void )
{
}

/* ========================================================================= */
/* 関数名：CGameFrame::~CGameFrame											 */
/* 内容：デストラクタ														 */
/* 日付：2007/09/10															 */
/* ========================================================================= */
CGameFrame::~CGameFrame( void )
{
}

/* ========================================================================= */
/* 関数名：CGameFrame::~CGameFrame											 */
/* 内容：ゲーム本体を描画													 */
/* 日付：2007/09/10															 */
/* ========================================================================= */
void CGameFrame::drawGameMain( void )
{
	int x, y;
	GetMousePoint( &x, &y );
	DrawString( 10, 10, "へっぉ！　( ^ω^)ワールド(^ω^ )", RGB( 255, 0, 0 ) );
	DrawString( x, y, "( ^ω^)", RGB( 255, 0, 0 ) );
}

/* ========================================================================= */
/* 関数名：CGameFrame::doMainLoop											 */
/* 内容：メインループ														 */
/* 日付：2007/09/10															 */
/* ========================================================================= */
void CGameFrame::doMainLoop( HINSTANCE hInst, int nCmdShow )
{
	bool flag = true;

	// アプリケーションに関する初期化
	if( flag ) InitApp( hInst, nCmdShow );

	// Direct3Dに関する初期化
	if( flag ) flag = InitD3D();
	if( flag ) flag = InitRender();

	// 移動補正
	setIdouHosei();

	try
	{
		loadFiles();					//各ファイルの読み込み
	}
	catch( int errcode )
	{
		MB( "ファイル読み込みエラー\nerrcode：%d", errcode );
		return;
	}

	/*****************/
	initStage();
	/*****************/

	// メッセージループ
	MSG msg;
    ZeroMemory( &msg, sizeof MSG );

	while( flag )
	{
		// FPSの取得
		GetFps();

		// ウィンドウが見えている時だけ描画するための処理
		WINDOWPLACEMENT wndpl;
		GetWindowPlacement( GetHwnd(), &wndpl ); // ウインドウの状態を取得
		if( ( wndpl.showCmd != SW_HIDE ) &&
			( wndpl.showCmd != SW_MINIMIZE ) &&
			( wndpl.showCmd != SW_SHOWMINIMIZED ) &&
			( wndpl.showCmd != SW_SHOWMINNOACTIVE ) )
		{
			LPDIRECT3DDEVICE9	pDevice = GetDevice();
			LPD3DXSPRITE		pSprite = GetSprite();

			// 画面の消去
			pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0, 0 );

			// スプライト描画
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

			// 画面の転送
			if( !Present() )
				flag = false;

			animcounter++;	// アニメカウンター
		}

		// メッセージの処理
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

	// 終了時の処理
	ReleaseApp();
}

/* ========================================================================= */
/* 関数名：CGameFrame::setIdouHosei											 */
/* 内容：移動補正															 */
/* 日付：2007/09/10															 */
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

		// 画面の消去
		pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0, 0 );

		// スプライト描画
		if( SUCCEEDED( pDevice->BeginScene() ) )
		{
			if( SUCCEEDED( pSprite->Begin() ) )
			{

				pSprite->End();
			}
		}
		pDevice->EndScene();

		// 画面の転送
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
/* 関数名：CGameFrame::IdouHosei											 */
/* 内容：移動補正を取得														 */
/* 日付：2007/09/10															 */
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
/* 関数名：CGameFrame::getAnimCnt											 */
/* 内容：animcounterを取得													 */
/* 日付：2007/09/10															 */
/* ========================================================================= */
unsigned int CGameFrame::getAnimCnt()
{
	return animcounter;
}
