#pragma once

class CAnimDraw
{
public:
	CAnimDraw( void );
	virtual ~CAnimDraw( void );

private:
	struct animpat
	{
		vector<int> ghlist;
		int anigap;
		animpat(){ anigap = 1; }
	};
	vector<animpat> aplist;

public:
	int setImage( int apid, int ghandle );
	int setGap( int apid, int gap );
	void draw( int apid, float x, float y );
	void draw( int apid, float x, float y, int alpha );
};
