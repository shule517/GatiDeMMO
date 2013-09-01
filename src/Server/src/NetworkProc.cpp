
#include "Network.h"

/* ========================================================================= */
/* 関数名：CNetwork::proc													 */
/* 内容：受信したパケット処理												 */
/* 日付：2007/08/28															 */
/* ========================================================================= */
void CNetwork::proc( int PlayerNo, int res, char *message )
{
	int		pLen = 2, header;
	WORD	nEnd, rEnd;
	nEnd = PEND;

	// エンドマーカ取得
	//memcpy( &rEnd, &message[res-2], 2 );

	//if( rEnd == nEnd )
	{
//		printf( "エンドマーカ一致！\n" );

		// ヘッダーを取得
		header = message[pLen];

		switch( header )
		{
			case PK_CMD_MV:
				proc_PK_CMD_MV( PlayerNo, res, message );
				break;

			case PK_USER_AUTH:
				proc_PK_USER_AUTH( PlayerNo, res, message );
				break;

			case PK_ATTACK:
				proc_PK_ATTACK( PlayerNo, res, message );
				break;

			default:
				printf( "未定義なパケットが送信されてきました。\n" );
				break;
		}
	}
//	else
	{
//		printf( "エンドマーカが一致しなかったので終了。\n" );
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
/* 関数名：CNetwork::proc_PK_CMD_MV											 */
/* 内容：移動パケット受信													 */
/* 日付：2007/09/04															 */
/* ========================================================================= */
void CNetwork::proc_PK_CMD_MV( int PlayerNo, int res, char *message )
{
/*	for( int i=0; i < res; i++ )
		printf( "%c", message[i] );
	printf( "\n" );

	printf( "|%d|%d|", res, message[2] );
*/
	int x = 0;
	int y = 0;
	int dx = 0;
	int dy = 0;

	procPacket( message, "%h %h %h %h", &x, &y, &dx, &dy );

//	printf( "x = %d\n", x);
//	printf( "y = %d\n", y);

	list<ClientData>::iterator it;
	it = ClientList.begin();

	while( it != ClientList.end() )
	{
		if( (*it).id == PlayerNo )
		{
			(*it).x = (float)x;
			(*it).y = (float)y;
			(*it).px = (*it).x;
			(*it).py = (*it).y;

			(*it).dx = (float)dx;
			(*it).dy = (float)dy;

			(*it).move_start_time = GetNowTime();

			float x = (*it).dx - (*it).x;
			float y = (*it).dy - (*it).y;
			float time = ( sqrtf( x*x + y*y ) / move_speed * (1.0f/60.0f) )*1000;
			(*it).move_end_time = GetNowTime() + (int)time;

//			(*it).state = STATE_MOVE;
			break;
		}
		++it;
	}

	// 周りのキャラに移動を伝える
	send_PK_OBJ_UPDATE_ACT( PlayerNo, dx, dy );
}

/* ========================================================================= */
/* 関数名：CNetwork::proc_PK_USER_AUTH										 */
/* 内容：認証パケット受信													 */
/* 日付：2007/09/04															 */
/* ========================================================================= */
void CNetwork::proc_PK_USER_AUTH( int PlayerNo, int res, char *message )
{
	char id[256];
	char pw[256];

	procPacket( message, "%s %s", &id, &pw );

	// 認証完了　自分のキャラデータを送信
	send_PK_USER_AUTH( PlayerNo );

	// プレイヤーキャラデータを送信
	sendNowPlayer( PlayerNo );
}

/* ========================================================================= */
/* 関数名：CNetwork::proc_PK_ATTACK											 */
/* 内容：攻撃パケット受信													 */
/* 日付：2007/09/03															 */
/* ========================================================================= */
void CNetwork::proc_PK_ATTACK( int PlayerNo, int res, char *message )
{
	int x = 0;
	int y = 0;

	procPacket( message, "%h %h", &x, &y );

	list<ClientData>::iterator it;
	it = ClientList.begin();

	// scale * scaleの正方形の範囲にダメージ
	float scale = 50;

	while( it != ClientList.end() )
	{
		if( (float)( x-scale/2 ) < (*it).x && (*it).x < (float)( x+scale/2) )
		{
			if( (float)( y-scale/2 ) < (*it).y && (*it).y < (float)( y+scale/2 ) )
			{
				// ダメージ
				send_PK_DAMAGE( PlayerNo, (*it).id, rand()%0xC8D1 );
			}
		}
		++it;
	}

	// 周りのキャラに攻撃を伝える

}
