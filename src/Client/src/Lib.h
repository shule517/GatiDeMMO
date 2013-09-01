#pragma once

#include "ImeProc.h"

/* ========================================================================= */
/* 宣言：extern																 */
/* ========================================================================= */
extern HWND					hwndApp;	// ハンドル
extern LPDIRECT3DSURFACE9	pBackBuff;	// バックバッファ
extern LPD3DXSPRITE			pSprite;	// スプライト
extern LPDIRECT3DDEVICE9	pDevice;	// デバイス

extern ImeProc imeProc;

/* ========================================================================= */
/* 宣言：初期化ライブラリ													 */
/* ========================================================================= */
bool	InitApp			( HINSTANCE hInst, int nCmdShow );
bool	InitD3D			( void );
bool	InitRender		( void );
void	ReleaseApp		( void );
bool	Present			( void );
bool	AppIdle			( void );
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, UINT wParam, LONG lParam );

/* ========================================================================= */
/* 宣言：ライブラリ															 */
/* ========================================================================= */
DWORD	GetNowTime		( void );
void	GetFps			( void );
void	GetMousePoint	( int *x, int *y );
HWND	GetHwnd			( void );

LPDIRECT3DDEVICE9	GetDevice( void );
LPD3DXSPRITE		GetSprite( void );

void	DrawBox			( int x1, int y1, int x2, int y2, int Color, int FillFlag );
void	DrawString		( int x, int y, char *String, int Color );
void	DrawRichString	( int x, int y, char *String, int Color, int BackColor );
void	GetStringSize	( char *String, int *width, int *height );
void	MB				( char *string, ... );
int		LoadGraph		( char *FileName );
int		LoadDivGraph	( char *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf );
void	DrawGraph		( int x, int y, int GrHandle  );
void	DrawGraph		( int x, int y, int GrHandle, int Alpha );
int		GetInput		();
int		GetInputOnce	();

/* ========================================================================= */
/* 宣言：ＩＭＥライブラリ													 */
/* ========================================================================= */
int		CreateEditBox	( void );
void	DrawEditBox		( int EditNo, int x, int y );
char*	GetEditText		( int EditNo );
void	SetEditText		( int EditNo, char *String );
void	DeleteEditBox	( int EditNo );

/* ========================================================================= */
/* 型の定義																	 */
/* ========================================================================= */
struct GRAPHIC_DATA
{
	LPDIRECT3DTEXTURE9	texture;
	RECT				rct;
	int					texWidth;	// 実際の幅
	int					texHeight;	// 実際の高さ
};

struct EDITBOX_DATA
{
	int		edit_no;		// ID
	int		Width;			// 実際の幅
	int		Height;			// 実際の高さ
	char	String[512];	// 文字
};

struct D3DTLVERTEX
{
	float x, y, z;
	float rhw;
	DWORD color;
	float tu0, tv0;

	enum { FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 };
};

struct D3DVERTEX {
	float x, y, z;
	float nx, ny, nz;
	float tu0, tv0;

	D3DVERTEX()
	{
	}
	D3DVERTEX(float _x, float _y, float _z, float _nx, float _ny, float _nz, float _tu0, float _tv0):
		x(_x), y(_y), z(_z), nx(_nx), ny(_ny), nz(_nz), tu0(_tu0), tv0(_tv0)
	{
	}
	void Set(float _x, float _y, float _z, float _nx, float _ny, float _nz, float _tu0, float _tv0)
	{
		x = _x;
		y = _y;
		z = _z;
		nx = _nx;
		ny = _ny;
		nz = _nz;
		tu0 = _tu0;
		tv0 = _tv0;
	}
	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
};

/* ========================================================================= */
/* 定数の定義																 */
/* ========================================================================= */
// マウス入力定義
#define MOUSE_INPUT_LEFT							(0x0001)			// マウス左ボタン
#define MOUSE_INPUT_RIGHT							(0x0002)			// マウス右ボタン
#define MOUSE_INPUT_MIDDLE							(0x0004)			// マウス中央ボタン
