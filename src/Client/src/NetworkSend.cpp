
#include "StdAfx.h"
#include "Network.h"

/* ========================================================================= */
/* 関数名：CNetwork::sendPacket												 */
/* 内容：パケットを作ってから送信											 */
/* 日付：2007/09/03															 */
/* ========================================================================= */
void CNetwork::sendPacket( char *fmt, ... )
{
	va_list	ap;
	int		d;
	char	*s;

	char			message[MAXLINE];
	unsigned short	nEnd = PEND;
	int				len, charlen;

	memset( &message, 0, MAXLINE );	// 初期化
	len = 2;
 
//	printf( "***********パケット作成***********\n" );

	va_start( ap, fmt );
	while( *fmt )
	{
		switch( *fmt++ )
		{
		case 's':
			// データ取得
			s = va_arg( ap, char * );

			// データの長さ
			charlen = (int)strlen( s );
			message[len] = charlen;
			len += 1;

			// データ
			memcpy( &message[len], s, charlen );
			len += charlen;

//			printf( "長さ：%d+1 char：%s\n", charlen, s );
			break;

		case 'd':
			// データ取得
			d = va_arg( ap, int );

			// データ
			message[len] = d;
			len += 1;
//			printf( "長さ：1 int：%d\n", d );
			break;

		case 'h':
			// データ取得
			d = va_arg( ap, int );

			d += 256;	// データ合わせ
			memcpy( &message[len], &d, 2 );

			if( d < 256 )
				MB( "%h Error：CreatePacket\n" );

			len += 2;
//			printf( "長さ：2 short：%d\n", d-256 );
			break;
		}
	}
    va_end( ap );

	// エンドマーカ
	memcpy( &message[len], &nEnd, 2 );
	len += 2;

	// データ会わせのため → 後から256を引かないといけない
	len += 256;

	// パケットの長さ
	memcpy( &message[0], &len, 2 );

	short res;
	memcpy( &res, &message[0], 2 );

	// パケット欠損チェック
	for( int i = 0; i < len-256; i++ )
	{
		if( message[i] == 0 )
		{
			//MB( "Error：パケット欠損（CreatePacket）" );
		}
	}

//	printf( "パケット全体の長さ：%d(2+2)\n", res-256 );

	/*
	// とりあえず表示してみる
	for( int i=0; i < len - 256; i++ )
		printf( "%c", message[i] );
	printf( "\n**********************************\n" );
	*/

	sendServer( message );
}

/* ========================================================================= */
/* 関数名：CNetwork::send_PK_CMD_MV											 */
/* 内容：移動パケット送信													 */
/* 日付：2007/09/03															 */
/* ========================================================================= */
void CNetwork::send_PK_CMD_MV( int x, int y, int dx, int dy )
{
	sendPacket( "%d %h %h %h %h", PK_CMD_MV, x, y, /*x*/dx, /*y*/dy );
}

/* ========================================================================= */
/* 関数名：CNetwork::send_PK_CMD_MV											 */
/* 内容：認証パケット送信													 */
/* 日付：2007/09/03															 */
/* ========================================================================= */
void CNetwork::send_PK_USER_AUTH( char *id, char *pw )
{
	sendPacket( "%d %s %s", PK_USER_AUTH, id, pw );
}

/* ========================================================================= */
/* 関数名：CNetwork::send_PK_ATTACK											 */
/* 内容：攻撃パケット送信													 */
/* 日付：2007/09/14															 */
/* ========================================================================= */
void CNetwork::send_PK_ATTACK( int x, int y )
{
	sendPacket( "%d %h %h", PK_ATTACK, x, y );
}
