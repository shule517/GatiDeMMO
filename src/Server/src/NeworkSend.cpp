
#include "Network.h"
#include "../../Common/GlobalDefine.h"

#include <stdarg.h>

/*
// �萔��`
#define MAXLEN_ACCOUNT		32		// �ő�A�J�E���g����
#define MAXLEN_PASSWORD		32		// �ő�p�X���[�h��
#define MAXLEN_CHARNAME		32		// �ő�L�����l�[����
#define MAX_BUFFER			4096
#define MAX_USER			1024	// �ő僆�[�U��

#define MAXLINE				1024
*/

/* ========================================================================= */
/* �֐����FCNetwork::CreatePacket											 */
/* ���e�F�p�P�b�g�����														 */
/* ���t�F2007/08/25															 */
/* ========================================================================= */
void CNetwork::CreatePacket( char *dat, char *fmt, ... )
{
	va_list	ap;
	int		d;
	char	*s;

	char			message[MAXLINE];
	unsigned short	nEnd = PEND;
	int				len, charlen;

	memset( &message, 0, MAXLINE );	// ������
	len = 2;
 
//	printf( "***********�p�P�b�g�쐬***********\n" );

	va_start( ap, fmt );
	while( *fmt )
	{
		switch( *fmt++ )
		{
		case 's':
			// �f�[�^�擾
			s = va_arg( ap, char * );

			// �f�[�^�̒���
			charlen = (int)strlen( s );
			message[len] = charlen;
			len += 1;

			// �f�[�^
			memcpy( &message[len], s, charlen );
			len += charlen;

//			printf( "�����F%d+1 char�F%s\n", charlen, s );
			break;

		case 'd':
			// �f�[�^�擾
			d = va_arg( ap, int );

			// �f�[�^
			message[len] = d;
			len += 1;
//			printf( "�����F1 int�F%d\n", d );
			break;

		case 'h':
			// �f�[�^�擾
			d = va_arg( ap, int );

			d += 256;	// �f�[�^���킹
			memcpy( &message[len], &d, 2 );

			if( d < 256 )
				printf( "%h Error�FCreatePacket\n" );

			len += 2;
//			printf( "�����F2 short�F%d\n", d-256 );
			break;
		}
	}
    va_end( ap );

	// �G���h�}�[�J
	memcpy( &message[len], &nEnd, 2 );
	len += 2;

	// �f�[�^��킹�̂��� �� �ォ��256�������Ȃ��Ƃ����Ȃ�
	len += 256;

	// �p�P�b�g�S�̂̒���
	memcpy( &message[0], &len, 2 );

	// �p�P�b�g�����`�F�b�N
	for( int i = 0; i < len-256; i++ )
	{
		if( message[i] == 0 )
			printf( "Error�F�p�P�b�g�����iCreatePacket�j\n" );
	}
/*
	short res;
	memcpy( &res, &message[0], 2 );
	printf( "�p�P�b�g�S�̂̒����F%d(2+2)\n", res-256 );

	// �Ƃ肠�����\�����Ă݂�
	for( int i=0; i < len - 256; i++ )
		printf( "%c", message[i] );
	printf( "\n**********************************\n" );
*/
	// �f�[�^�������n��
	memset( dat, 0, MAXLINE );	// ������
	memcpy( dat, &message[0], len-256 );
}

/* ========================================================================= */
/* �֐����FCNetwork::send_PK_OBJ_ADD										 */
/* ���e�F�I�u�W�F�N�g�ǉ��p�P�b�g�̑��M										 */
/* ���t�F2007/08/25															 */
/* ========================================================================= */
void CNetwork::send_PK_OBJ_ADD( int PlayerNo )
{
//	ClientData *cd = get( PlayerNo );
//	char dat[MAXLINE];
//	CreatePacket( dat, "%d %d %d %h %s %h %h", PK_OBJ_ADD, /*��޼ު������*/2, /*��޼ު�Đ�*/6, PlayerNo, "�V���[�����V�O�i��", /*x*/(*cd).x, /*y*/(*cd).y );
//	sendPacket( SEND_ALL, PlayerNo, dat );
//	sendPacket( SEND_ME, PlayerNo, dat );
}

/* ========================================================================= */
/* �֐����FCNetwork::send_PK_OBJ_ADD										 */
/* ���e�F�I�u�W�F�N�g�폜�p�P�b�g�̑��M										 */
/* ���t�F2007/09/03															 */
/* ========================================================================= */
void CNetwork::send_PK_OBJ_REMOVE( int PlayerNo )
{
	char dat[MAXLINE];
	CreatePacket( dat, "%d %d %h %h %h", PK_OBJ_REMOVE, /*��޼ު������*/2, PlayerNo, /*x*/rand()%640, /*y*/rand()%480 );
	sendPacket( SEND_ALL, PlayerNo, dat );
	sendPacket( SEND_ME, PlayerNo, dat );
}

/* ========================================================================= */
/* �֐����FCNetwork::send_PK_OBJ_UPDATE_ACT									 */
/* ���e�F�I�u�W�F�N�g�A�b�v�f�[�g�p�P�b�g									 */
/* ���t�F2007/09/03															 */
/* ========================================================================= */
void CNetwork::send_PK_OBJ_UPDATE_ACT( int PlayerNo, int x, int y )
{
	char dat[MAXLINE];
	CreatePacket( dat, "%d %d %d %h %h %h", PK_OBJ_UPDATE_ACT, /*��޼ު������*/2, /*��޼ު�Đ�*/1, PlayerNo, x, y );
	sendPacket( SEND_ALL, PlayerNo, dat );
	sendPacket( SEND_ME, PlayerNo, dat );
}

/* ========================================================================= */
/* �֐����FCNetwork::send_PK_USER_AUTH										 */
/* ���e�F���[�U�F�؃p�P�b�g													 */
/* ���t�F2007/09/12															 */
/* ========================================================================= */
void CNetwork::send_PK_USER_AUTH( int PlayerNo )
{
	char dat[MAXLINE];
	CreatePacket( dat, "%d %d %h %h", PK_USER_AUTH, /*�����Ґ�*/10, /*ID*/"ID", PlayerNo );
	sendPacket( SEND_ME, PlayerNo, dat );
}

/* ========================================================================= */
/* �֐����FCNetwork::sendPlayer												 */
/* ���e�F���ݐڑ����Ă���l��ʒm����										 */
/* ���t�F2007/09/03															 */
/* ========================================================================= */
void CNetwork::sendNowPlayer( int PlayerNo )
{
	list<ClientData>::iterator it;
	it = ClientList.begin();

	while( it != ClientList.end() )
	{
		char dat[MAXLINE];
		CreatePacket( dat, "%d %d %d %h %s %h %h", PK_OBJ_ADD, /*��޼ު������*/2, /*��޼ު�Đ�*/1, (*it).id, "�V���[�����V�O�i��", /*x*/(int)(*it).x, /*y*/(int)(*it).y );

		if( PlayerNo != (*it).id )
		{	// �����ȊO�̃f�[�^�������ɑ���
			sendPacket( SEND_ME, PlayerNo, dat );
		}
		else
		if( PlayerNo == (*it).id )
		{	// �S���Ɏ����̃f�[�^�𑗂�
			sendPacket( SEND_ALL, PlayerNo, dat );
		}
		++it;
	}
}

/* ========================================================================= */
/* �֐����FCNetwork::send_PK_DAMAGE											 */
/* ���e�F�_���[�W�p�P�𑗐M													 */
/* ���t�F2007/09/14															 */
/* ========================================================================= */
void CNetwork::send_PK_DAMAGE( int PlayerNo, int DamagePlayerNo, int damage )
{
	char dat[MAXLINE];
	CreatePacket( dat, "%d %h %h", PK_DAMAGE, DamagePlayerNo, damage );

	// �����̍U���Ŏ������_���[�W���󂯂�ꍇ�͂�����ύX����
	sendPacket( SEND_ALL, PlayerNo, dat );
	sendPacket( SEND_ME, PlayerNo, dat );
}
