#pragma once

#pragma warning( disable : 4290 )

/* ========================================================================= */
/* ヘッダ読み込み															 */
/* ========================================================================= */
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <process.h>
#include <imm.h>
/*
#include <stdlib.h>
#include <conio.h>
*/

#include "../../Common/GlobalDefine.h"

#include <vector>
#include <list>
using namespace std;

#include "Lib.h"
#include "AnimDraw.h"
#include "GameFrame.h"
#include "CharaData.h"

/* ========================================================================= */
/* ライブラリ読み込み														 */
/* ========================================================================= */
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "ws2_32.lib" )
#pragma comment( lib, "imm32.lib" )

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */
#define RELEASE( x )		if( x ){ x->Release(); x = NULL; }

const int		WIDTH		= 640;
const int		HEIGHT		= 480;

const LPCSTR	CLASS_NAME	= "OrangeBox";
const LPCSTR	TITLE_NAME	= "みかん箱";
