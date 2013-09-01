
#include "Network.h"
#include "../../Common/GlobalDefine.h"

#include <stdarg.h>

/*
// 定数定義
#define MAXLEN_ACCOUNT		32		// 最大アカウント名長
#define MAXLEN_PASSWORD		32		// 最大パスワード長
#define MAXLEN_CHARNAME		32		// 最大キャラネーム長
#define MAX_BUFFER			4096
#define MAX_USER			1024	// 最大ユーザ数

#define MAXLINE				1024
*/

/* ========================================================================= */
/* 関数名：CNetwork::CreatePacket											 */
/* 内容：パケットを作る														 */
/* 日付：2007/08/25															 */
/* ========================================================================= */
void CNetwork::CreatePacket( char *dat, char *fmt, ... )
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
				printf( "%h Error：CreatePacket\n" );

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

	// パケット全体の長さ
	memcpy( &message[0], &len, 2 );

	// パケット欠損チェック
	for( int i = 0; i < len-256; i++ )
	{
		if( message[i] == 0 )
			printf( "Error：パケット欠損（CreatePacket）\n" );
	}
/*
	short res;
	memcpy( &res, &message[0], 2 );
	printf( "パケット全体の長さ：%d(2+2)\n", res-256 );

	// とりあえず表示してみる
	for( int i=0; i < len - 256; i++ )
		printf( "%c", message[i] );
	printf( "\n**********************************\n" );
*/
	// データを引き渡し
	memset( dat, 0, MAXLINE );	// 初期化
	memcpy( dat, &message[0], len-256 );
}

/* ========================================================================= */
/* 関数名：CNetwork::send_PK_OBJ_ADD										 */
/* 内容：オブジェクト追加パケットの送信										 */
/* 日付：2007/08/25															 */
/* ========================================================================= */
void CNetwork::send_PK_OBJ_ADD( int PlayerNo )
{
//	ClientData *cd = get( PlayerNo );
//	char dat[MAXLINE];
//	CreatePacket( dat, "%d %d %d %h %s %h %h", PK_OBJ_ADD, /*ｵﾌﾞｼﾞｪｸﾄﾀｲﾌﾟ*/2, /*ｵﾌﾞｼﾞｪｸﾄ数*/6, PlayerNo, "シュール＝シグナル", /*x*/(*cd).x, /*y*/(*cd).y );
//	sendPacket( SEND_ALL, PlayerNo, dat );
//	sendPacket( SEND_ME, PlayerNo, dat );
}

/* ========================================================================= */
/* 関数名：CNetwork::send_PK_OBJ_ADD										 */
/* 内容：オブジェクト削除パケットの送信										 */
/* 日付：2007/09/03															 */
/* ========================================================================= */
void CNetwork::send_PK_OBJ_REMOVE( int PlayerNo )
{
	char dat[MAXLINE];
	CreatePacket( dat, "%d %d %h %h %h", PK_OBJ_REMOVE, /*ｵﾌﾞｼﾞｪｸﾄﾀｲﾌﾟ*/2, PlayerNo, /*x*/rand()%640, /*y*/rand()%480 );
	sendPacket( SEND_ALL, PlayerNo, dat );
	sendPacket( SEND_ME, PlayerNo, dat );
}

/* ========================================================================= */
/* 関数名：CNetwork::send_PK_OBJ_UPDATE_ACT									 */
/* 内容：オブジェクトアップデートパケット									 */
/* 日付：2007/09/03															 */
/* ========================================================================= */
void CNetwork::send_PK_OBJ_UPDATE_ACT( int PlayerNo, int x, int y )
{
	char dat[MAXLINE];
	CreatePacket( dat, "%d %d %d %h %h %h", PK_OBJ_UPDATE_ACT, /*ｵﾌﾞｼﾞｪｸﾄﾀｲﾌﾟ*/2, /*ｵﾌﾞｼﾞｪｸﾄ数*/1, PlayerNo, x, y );
	sendPacket( SEND_ALL, PlayerNo, dat );
	sendPacket( SEND_ME, PlayerNo, dat );
}

/* ========================================================================= */
/* 関数名：CNetwork::send_PK_USER_AUTH										 */
/* 内容：ユーザ認証パケット													 */
/* 日付：2007/09/12															 */
/* ========================================================================= */
void CNetwork::send_PK_USER_AUTH( int PlayerNo )
{
	char dat[MAXLINE];
	CreatePacket( dat, "%d %d %h %h", PK_USER_AUTH, /*ｱｸｾｽ者数*/10, /*ID*/"ID", PlayerNo );
	sendPacket( SEND_ME, PlayerNo, dat );
}

/* ========================================================================= */
/* 関数名：CNetwork::sendPlayer												 */
/* 内容：現在接続している人を通知する										 */
/* 日付：2007/09/03															 */
/* ========================================================================= */
void CNetwork::sendNowPlayer( int PlayerNo )
{
	list<ClientData>::iterator it;
	it = ClientList.begin();

	while( it != ClientList.end() )
	{
		char dat[MAXLINE];
		CreatePacket( dat, "%d %d %d %h %s %h %h", PK_OBJ_ADD, /*ｵﾌﾞｼﾞｪｸﾄﾀｲﾌﾟ*/2, /*ｵﾌﾞｼﾞｪｸﾄ数*/1, (*it).id, "シュール＝シグナル", /*x*/(int)(*it).x, /*y*/(int)(*it).y );

		if( PlayerNo != (*it).id )
		{	// 自分以外のデータを自分に送る
			sendPacket( SEND_ME, PlayerNo, dat );
		}
		else
		if( PlayerNo == (*it).id )
		{	// 全員に自分のデータを送る
			sendPacket( SEND_ALL, PlayerNo, dat );
		}
		++it;
	}
}

/* ========================================================================= */
/* 関数名：CNetwork::send_PK_DAMAGE											 */
/* 内容：ダメージパケを送信													 */
/* 日付：2007/09/14															 */
/* ========================================================================= */
void CNetwork::send_PK_DAMAGE( int PlayerNo, int DamagePlayerNo, int damage )
{
	char dat[MAXLINE];
	CreatePacket( dat, "%d %h %h", PK_DAMAGE, DamagePlayerNo, damage );

	// 自分の攻撃で自分もダメージを受ける場合はここを変更する
	sendPacket( SEND_ALL, PlayerNo, dat );
	sendPacket( SEND_ME, PlayerNo, dat );
}
