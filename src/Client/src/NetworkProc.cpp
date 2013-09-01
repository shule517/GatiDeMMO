
#include "StdAfx.h"
#include "Network.h"

/* ========================================================================= */
/* �֐����FCNetwork::procMain												 */
/* ���e�F�p�P�b�g�������C��													 */
/* ���t�F2007/08/28															 */
/* ========================================================================= */
void CNetwork::procMain( short res, char *dat )
{
	int		pLen = 2, header;
	WORD	nEnd, rEnd;
	nEnd = PEND;

	// �G���h�}�[�J�擾
	memcpy( &rEnd, &dat[res-2], 2 );

	if( rEnd == nEnd )
	{
//		printf( "�G���h�}�[�J��v�I\n" );

		// �w�b�_�[���擾
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
				MB( "Error�F����`�ȃp�P�b�g�����M����Ă��܂����B" );
				break;
		}
	}
	else
	{
		MB( "Error�F�G���h�}�[�J����v���܂���ł����B" );
	}
}

/* ========================================================================= */
/* �֐����FCNetwork::procPacket												 */
/* ���e�F�p�P�b�g�̃f�[�^���o												 */
/* ���t�F2007/08/28															 */
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
				// �f�[�^�擾
				s = va_arg( ap, char * );

				// �f�[�^�̒���
				charlen = message[len];
				len += 1;

				// �f�[�^
				memcpy( s, &message[len], charlen );
				s[charlen] = '\0';
				len += charlen;

//				printf( "%d|%s|", charlen, s );
				//printf( "char %s\n", s );
				break;

			case 'd': // int
				// �f�[�^�擾
				d = va_arg( ap, int * );

				// �f�[�^
				(*d) = message[len];
				len += 1;

//				printf( "%d|", (*d) );
				//printf( "int %d\n", (*d) );
				break;

			case 'h': // short
				// �f�[�^�擾
				d = va_arg( ap, int * );

				// �p�P�b�g�̒������擾
				memcpy( &(*d), &message[len], 2 );
				(*d) -= 256; // �f�[�^���킹

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
/* �֐����FCNetwork::proc_PK_OBJ_ADD										 */
/* ���e�F�L�����N�^�[�ǉ��p�P�b�g��M										 */
/* ���t�F2007/08/28															 */
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

	// �I�u�W�F�N�g�^�C�v
	switch( ObjType )
	{
	case 2:	// PC
		addPlayerList( PlayerNo, Name, x, y );
		break;
	default:
		MB( "Error�F�I�u�W�F�N�g�^�C�v������`�ł��B(proc_PK_OBJ_ADD)" );
		break;
	}
}

/* ========================================================================= */
/* �֐����FCNetwork::proc_PK_OBJ_REMOVE										 */
/* ���e�F�I�u�W�F�N�g�폜�p�P�b�g��M										 */
/* ���t�F2007/09/03															 */
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

	// �I�u�W�F�N�g�^�C�v
	switch( ObjType )
	{
	case 2:	// PC
		erasePlayer( PlayerNo );
		break;
	default:
		MB( "Error�F�I�u�W�F�N�g�^�C�v������`�ł��B(proc_PK_OBJ_REMOVE)" );
		break;
	}
}

/* ========================================================================= */
/* �֐����FCNetwork::proc_PK_OBJ_UPDATE_ACT									 */
/* ���e�F�I�u�W�F�N�g�A�b�v�f�[�^�p�P�b�g��M								 */
/* ���t�F2007/09/03															 */
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

	// �I�u�W�F�N�g�^�C�v
	switch( ObjType )
	{
	case 2: // PC
		movePlayer( PlayerNo, dx, dy );
		break;
	default:
		MB( "Error�F�I�u�W�F�N�g�^�C�v������`�ł��B(proc_PK_OBJ_UPDATE_ACT)" );
		break;
	}
}

/* ========================================================================= */
/* �֐����FCNetwork::proc_PK_USER_AUTH										 */
/* ���e�F�F�؃p�P�b�g��M													 */
/* ���t�F2007/09/03															 */
/* ========================================================================= */
void CNetwork::proc_PK_USER_AUTH( int res, char *dat )
{
	int	PlayerNum = 0;
	int PlayerNo = 0;
	char id[256] = "NULL";

	procPacket( dat, "%d %h %h", &PlayerNum, &id, &PlayerNo );

	// �����̃L������ǉ�����
	addMyList( PlayerNo, "�V���[�����V�O�i��", 0, 0 );
}

/* ========================================================================= */
/* �֐����FCNetwork::proc_PK_DAMAGE											 */
/* ���e�F�_���[�W�p�P�b�g��M												 */
/* ���t�F2007/09/14															 */
/* ========================================================================= */
void CNetwork::proc_PK_DAMAGE( int res, char *dat )
{
	int PlayerNo = 0;
	int damage = 0;

	procPacket( dat, "%h %h", &PlayerNo, &damage );

	// �_���[�W����
	damagePlayer( PlayerNo, damage );
}
