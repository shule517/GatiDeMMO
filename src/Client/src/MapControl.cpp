#include "StdAfx.h"
#include "MapControl.h"

/* ========================================================================= */
/* �֐����FCMapControl::CMapControl											 */
/* ���e�F�R���X�g���N�^														 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
CMapControl::CMapControl( void )
{
}

/* ========================================================================= */
/* �֐����FCMapControl::~CMapControl										 */
/* ���e�F�f�X�g���N�^														 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
CMapControl::~CMapControl( void )
{
}

/* ========================================================================= */
/* �֐����FCMapControl::loadFiles											 */
/* ���e�F�t�@�C���̓ǂݍ���													 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
void CMapControl::loadFiles( void )
{
	int ghandle[216];
	if( LoadDivGraph( "data/glasstown001.bmp", 216, 8, 27, 32, 32, ghandle ) == -1 ) throw( -2 );

	for( int i = 0; i < 216; i++ )
		MapAnim.setImage( i, ghandle[i] );
}

/* ========================================================================= */
/* �֐����FCMapControl::init												 */
/* ���e�F������																 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
void CMapControl::init()
{
	for( int y = 0; y < MAP_HEIGHT; y++ )
	{
		for( int x = 0; x < MAP_WIDTH; x++ )
		{
			switch( rand()%3 )
			{
			case 0:
				mapdat[y][x] = 0;
				break;
			case 1:
				mapdat[y][x] = 1;
				break;
			case 2:
				mapdat[y][x] = 2;
				break;
			}
		}
	}
}

/* ========================================================================= */
/* �֐����FCMapControl::move												 */
/* ���e�F�}�b�v�̏���														 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
void CMapControl::move( float JikiX, float JikiY )
{
	for( int y = 0; y < MAP_HEIGHT; y++ )
	{
		for( int x = 0; x < MAP_WIDTH; x++ )
		{
			if( ( JikiX - WIDTH/2 - 32 < x*32 && x*32 < JikiX + WIDTH/2 )  && ( JikiY - HEIGHT/2 - 32 < y*32 && y*32 < JikiY + HEIGHT/2 ) )
				MapAnim.draw( mapdat[y][x], x*32 - JikiX + WIDTH/2, y*32 - JikiY + HEIGHT/2 );
		}
	}
}
