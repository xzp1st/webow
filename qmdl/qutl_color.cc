
#include "qmdl_base.h"


qstr_enum_begin(QUIColor)
	qstr_enum_item( "black",			QCLR_BLACK )
	qstr_enum_item( "gray0",			QCLR_GRAY0 )
	qstr_enum_item( "gray1",			QCLR_GRAY1 )
	qstr_enum_item( "gray2",			QCLR_GRAY2 )
	qstr_enum_item( "gray3",			QCLR_GRAY3 )
	qstr_enum_item( "gray4",			QCLR_GRAY4 )
	qstr_enum_item( "gray5",			QCLR_GRAY5 )
	qstr_enum_item( "gray6",			QCLR_GRAY6 )
	qstr_enum_item( "gray7",			QCLR_GRAY7 )
	qstr_enum_item( "gray8",			QCLR_GRAY8 )
	qstr_enum_item( "gray9",			QCLR_GRAY9 )
	qstr_enum_item( "gray10",			QCLR_GRAY10 )
	qstr_enum_item( "gray11",			QCLR_GRAY11 )
	qstr_enum_item( "gray12",			QCLR_GRAY12 )
	qstr_enum_item( "gray13",			QCLR_GRAY13 )
	qstr_enum_item( "gray14",			QCLR_GRAY14 )
	qstr_enum_item( "gray15",			QCLR_GRAY15 )
	qstr_enum_item( "white",			QCLR_WHITE )

	qstr_enum_item( "blue",			QCLR_BLUE )
	qstr_enum_item( "green",			QCLR_GREEN )
	qstr_enum_item( "cyan",			QCLR_CYAN )
	qstr_enum_item( "red",			QCLR_RED )
	qstr_enum_item( "magenta",		QCLR_MAGENTA )
	qstr_enum_item( "brown",			QCLR_BROWN )
	qstr_enum_item( "ltgray",			QCLR_LTGRAY )
	qstr_enum_item( "gray",			QCLR_GRAY )
	qstr_enum_item( "ltblue",			QCLR_LTBLUE )
	qstr_enum_item( "ltgreen",		QCLR_LTGREEN )
	qstr_enum_item( "ltcyan",			QCLR_LTCYAN )
	qstr_enum_item( "ltred",			QCLR_LTRED )
	qstr_enum_item( "ltmagenta",		QCLR_LTMAGENTA )
	qstr_enum_item( "yellow",			QCLR_YELLOW )
qstr_enum_end


QCLR quiStr2Color(QSTR color, QINT size)
{
	QINT nlen, ncount, ncolor, nrcolor, ngcolor, nbcolor, nacolor;
	
	if(color == NULL)
	{
		return 0;
	}
	ncount = 0;
	ncolor = qstr2enum(QUIColor, 1, (QPNT )color, &ncount);
	if(ncount > 0)
	{
		return 0;
	}
	if(color[0] != '#')
	{
		nrcolor = qclrGetRColor(ncolor);
		ngcolor = qclrGetGColor(ncolor);
		nbcolor = qclrGetBColor(ncolor);
		nacolor = qclrGetAColor(ncolor);
	}
	else
	{
		nlen = qstrlen(color);
		if(nlen <= 6)
		{
			return 0;
		}
		ncount = 2;
		nrcolor = qstrint(16, (QPNT )&color[1], &ncount);
		ncount = 2;
		ngcolor = qstrint(16, (QPNT )&color[3], &ncount);
		ncount = 2;
		nbcolor = qstrint(16, (QPNT )&color[5], &ncount);
		if(nlen < 8)
		{
			nacolor = 255;
		}
		else
		{
			ncount = 2;
			nacolor = qstrint(16, (QPNT )&color[7], &ncount);
		}
	}
	
	return (QCLR)qclrMakeRGBA(nrcolor, ngcolor, nbcolor, nacolor);
}
