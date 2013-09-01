
#include "StdAfx.h"
#include "Network.h"

/* ========================================================================= */
/* 関数名：CNetwork::procMain												 */
/* 内容：パケット処理メイン													 */
/* 日付：2007/08/28															 */
/* ========================================================================= */
void CNetwork::procMain( short res, char *dat )
{
	int		pLen = 2, header;
	WORD	nEnd, rEnd;
	nEnd = PEND;

	// エンドマーカ取得
	memcpy( &rEnd, &dat[res-2], 2 );

	if( rEnd == nEnd )
	{
//		printf( "エンドマーカ一致！\n" );

		// ヘッダーを取得
		header = dat[pLen];

		switch( header )
		{
			case PK_OBJ_ADD:
				proc_PK_OBJ_ADD( res, dat );
				break;

			case PK_OBJ_REMOVE:
				proc_PK_OBJ_REMOVE( res, dat );
				break;

			case PK_OBJ_UPDATE_ACT:
				proc_PK_OBJ_UPDATE_ACT( res, dat );
				break;

			case PK_USER_AUTH:
				proc_PK_USER_AUTH( res, dat );
				break;

			case PK_DAMAGE:
				proc_PK_DAMAGE( res, dat );
				break;

			default:
				MB( "Error：未定義なパケットが送信されてきました。" );
				break;
		}
	}
	else
	{
		MB( "Error：エンドマーカが一致しませんでした。" );
	}
}

/* ========================================================================= */
/* 関数名：CNetwork::procPacket												 */
/* 内容：パケットのデータ抽出												 */
/* 日付：2007/08/28															 */
/* ========================================================================= */
void CNetwork::procPacket( char *message, char *fmt, ... )
{
	va_list	ap;
	int		*d;
	char	*s;

	int len = 3;
	int charlen;

	va_start( ap, fmt );
	while( *fmt )
	{
		switch( *fmt++ )
		{
			case 's': // char
				// データ取得
				s = va_arg( ap, char * );

				// データの長さ
				charlen = message[len];
				len += 1;

				// データ
				memcpy( s, &message[len], charlen );
				s[charlen] = '\0';
				len += charlen;

//				printf( "%d|%s|", charlen, s );
				//printf( "char %s\n", s );
				break;

			case 'd': // int
				// データ取得
				d = va_arg( ap, int * );

				// データ
				(*d) = message[len];
				len += 1;

//				printf( "%d|", (*d) );
				//printf( "int %d\n", (*d) );
				break;

			case 'h': // short
				// データ取得
				d = va_arg( ap, int * );

				// パケットの長さを取得
				memcpy( &(*d), &message[len], 2 );
				(*d) -= 256; // データ合わせ

				len += 2;

//				printf( "%d|" , (*d) );
				//printf( "short %d\n", (*d) );
				break;
		}
	}
    va_end( ap );

//	printf( "PEND|\n" );
}

/* ========================================================================= */
/* 関数名：CNetwork::proc_PK_OBJ_ADD										 */
/* 内容：キャラクター追加パケット受信										 */
/* 日付：2007/08/28															 */
/* ========================================================================= */
void CNetwork::proc_PK_OBJ_ADD( int res, char *dat )
{
	/*
	for( int i=0; i < res; i++ )
		printf( "%c", message[i] );
	printf( "\n" );

	printf( "|%d|%d|", res, message[2] );
	*/

	int ObjType = 0;
	int ObjNum = 0;
	int PlayerNo = 0;
	char Name[256];
	int x = 0;
	int y = 0;

	procPacket( dat, "%d %d %h %s %h %h", &ObjType, &ObjNum, &PlayerNo, &Name, &x, &y);

	/*
	printf( "ObjType = %d\n", ObjType);
	printf( "ObjNum = %d\n", ObjNum);
	printf( "PlayerNo = %d\n", PlayerNo);
	printf( "Name = %s\n", Name);
	printf( "x = %d\n", x);
	printf( "y = %d\n", y);

	/**************************
	// test
	sprintf_s( Name, "%d", PlayerNo );
	/**************************/

	// オブジェクトタイプ
	switch( ObjType )
	{
	case 2:	// PC
		addPlayerList( PlayerNo, Name, x, y );
		break;
	default:
		MB( "Error：オブジェクトタイプが未定義です。(proc_PK_OBJ_ADD)" );
		break;
	}
}

/* ========================================================================= */
/* 関数名：CNetwork::proc_PK_OBJ_REMOVE										 */
/* 内容：オブジェクト削除パケット受信										 */
/* 日付：2007/09/03															 */
/* ========================================================================= */
void CNetwork::proc_PK_OBJ_REMOVE( int res, char *dat )
{
	/*
	for( int i=0; i < res; i++ )
		printf( "%c", message[i] );
	printf( "\n" );

	printf( "|%d|%d|", res, message[2] );
	*/

	int ObjType = 0;
	int PlayerNo = 0;
	int x = 0;
	int y = 0;

	procPacket( dat, "%d %h %h %h", &ObjType, &PlayerNo, &x, &y);

	/*
	printf( "ObjType = %d\n", ObjType);
	printf( "PlayerNo = %d\n", PlayerNo);
	printf( "x = %d\n", x);
	printf( "y = %d\n", y);
	*/

	// オブジェクトタイプ
	switch( ObjType )
	{
	case 2:	// PC
		erasePlayer( PlayerNo );
		break;
	default:
		MB( "Error：オブジェクトタイプが未定義です。(proc_PK_OBJ_REMOVE)" );
		break;
	}
}

/* ========================================================================= */
/* 関数名：CNetwork::proc_PK_OBJ_UPDATE_ACT									 */
/* 内容：オブジェクトアップデータパケット受信								 */
/* 日付：2007/09/03															 */
/* ========================================================================= */
void CNetwork::proc_PK_OBJ_UPDATE_ACT( int res, char *dat )
{
	/*
	for( int i=0; i < res; i++ )
		printf( "%c", message[i] );
	printf( "\n" );

	printf( "|%d|%d|", res, message[2] );
	*/

	int ObjType = 0;
	int ObjNum = 0;
	int PlayerNo = 0;
	int dx = 0;
	int dy = 0;

	procPacket( dat, "%d %d %h %h %h", &ObjType, &ObjNum, &PlayerNo, &dx, &dy );

	/*
	printf( "ObjType = %d\n", ObjType );
	printf( "ObjNum = %d\n", ObjNum );
	printf( "PlayerNo = %d\n", PlayerNo );
	printf( "dx = %d\n", dx );
	printf( "dy = %d\n", dy );
	*/

	// オブジェクトタイプ
	switch( ObjType )
	{
	case 2: // PC
		movePlayer( PlayerNo, dx, dy );
		break;
	default:
		MB( "Error：オブジェクトタイプが未定義です。(proc_PK_OBJ_UPDATE_ACT)" );
		break;
	}
}

/* ========================================================================= */
/* 関数名：CNetwork::proc_PK_USER_AUTH										 */
/* 内容：認証パケット受信													 */
/* 日付：2007/09/03															 */
/* ========================================================================= */
void CNetwork::proc_PK_USER_AUTH( int res, char *dat )
{
	int	PlayerNum = 0;
	int PlayerNo = 0;
	char id[256] = "NULL";

	procPacket( dat, "%d %h %h", &PlayerNum, &id, &PlayerNo );

	// 自分のキャラを追加する
	addMyList( PlayerNo, "シュール＝シグナル", 0, 0 );
}

/* ========================================================================= */
/* 関数名：CNetwork::proc_PK_DAMAGE											 */
/* 内容：ダメージパケット受信												 */
/* 日付：2007/09/14															 */
/* ========================================================================= */
void CNetwork::proc_PK_DAMAGE( int res, char *dat )
{
	int PlayerNo = 0;
	int damage = 0;

	procPacket( dat, "%h %h", &PlayerNo, &damage );

	// ダメージ処理
	damagePlayer( PlayerNo, damage );
}
