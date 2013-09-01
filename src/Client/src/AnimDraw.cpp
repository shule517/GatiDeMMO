
#include "StdAfx.h"

/* ========================================================================= */
/* 関数名：CAnimDraw::CAnimDraw												 */
/* 内容：コンストラクタ														 */
/* 日付：2007/09/10															 */
/* ========================================================================= */
CAnimDraw::CAnimDraw( void )
{
	aplist.push_back( animpat() );
}

/* ========================================================================= */
/* 関数名：CAnimDraw::~CAnimDraw											 */
/* 内容：デストラクタ														 */
/* 日付：2007/09/10															 */
/* ========================================================================= */
CAnimDraw::~CAnimDraw( void )
{
}

/* ========================================================================= */
/* 関数名：CAnimDraw::setImage												 */
/* 内容：画像のセット														 */
/* 日付：2007/09/10															 */
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
/* 関数名：CAnimDraw::setGap												 */
/* 内容：アニメーションギャップのセット										 */
/* 日付：2007/09/10															 */
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
/* 関数名：CAnimDraw::setImage												 */
/* 内容：アニメーションの描画												 */
/* 日付：2007/09/10															 */
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
/* 関数名：CAnimDraw::setImage												 */
/* 内容：アニメーションの描画(アルファ値)									 */
/* 日付：2007/09/10															 */
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
