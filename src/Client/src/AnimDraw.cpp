
#include "StdAfx.h"

/* ========================================================================= */
/* �֐����FCAnimDraw::CAnimDraw												 */
/* ���e�F�R���X�g���N�^														 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
CAnimDraw::CAnimDraw( void )
{
	aplist.push_back( animpat() );
}

/* ========================================================================= */
/* �֐����FCAnimDraw::~CAnimDraw											 */
/* ���e�F�f�X�g���N�^														 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
CAnimDraw::~CAnimDraw( void )
{
}

/* ========================================================================= */
/* �֐����FCAnimDraw::setImage												 */
/* ���e�F�摜�̃Z�b�g														 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
int CAnimDraw::setImage( int apid, int ghandle )
{
	if( apid >= (int)aplist.size() )
	{
		aplist.push_back( animpat() );
		apid = aplist.size() - 1;
	}
	aplist.at( apid ).ghlist.push_back( ghandle );

	return apid;
}

/* ========================================================================= */
/* �֐����FCAnimDraw::setGap												 */
/* ���e�F�A�j���[�V�����M���b�v�̃Z�b�g										 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
int CAnimDraw::setGap( int apid, int gap )
{
	if( apid >= (int)aplist.size() )
	{
		aplist.push_back( animpat() );
		apid = aplist.size() - 1;
	}
	aplist.at( apid ).anigap = gap;

	return apid;
}

/* ========================================================================= */
/* �֐����FCAnimDraw::setImage												 */
/* ���e�F�A�j���[�V�����̕`��												 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
void CAnimDraw::draw( int apid, float x, float y )
{
	if( aplist.at( apid ).ghlist.size() > 1 )
	{
		int curpat = CGameFrame::getAnimCnt() / aplist.at( apid ).anigap % aplist.at( apid ).ghlist.size();
		DrawGraph( (int)x, (int)y, aplist.at( apid ).ghlist.at( curpat ) );
	}
	else
	{
		DrawGraph( (int)x, (int)y, aplist.at( apid ).ghlist.at( 0 ) );
	}
}

/* ========================================================================= */
/* �֐����FCAnimDraw::setImage												 */
/* ���e�F�A�j���[�V�����̕`��(�A���t�@�l)									 */
/* ���t�F2007/09/10															 */
/* ========================================================================= */
void CAnimDraw::draw( int apid, float x, float y, int alpha )
{
	if( aplist.at( apid ).ghlist.size() > 1 )
	{
		int curpat = CGameFrame::getAnimCnt() / aplist.at( apid ).anigap % aplist.at( apid ).ghlist.size();
		DrawGraph( (int)x, (int)y, aplist.at( apid ).ghlist.at( curpat ), alpha);
	}
	else
	{
		DrawGraph( (int)x, (int)y, aplist.at( apid ).ghlist.at( 0 ), alpha );
	}
}
