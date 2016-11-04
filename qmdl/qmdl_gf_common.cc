
#include "qmdl_config.h"

#ifdef QMDL_VER_GF

#include "stdafx.h"
#include "qmdl_gf.h"


CString quiStr2Path(QSTR str, QINT size)
{
	QINT nlen;
	QCHR vprotocol[QSTR_BUFF_SIZE];
	QCHR vpath[QSTR_BUFF_SIZE];
	CString strpath;

	strpath.Empty();
	nlen = qurlGetProtocol(str, vprotocol, sizeof(vprotocol));
	if(nlen <= 0)
	{
		strpath += Util::Dir::Program::GetResDir();
	}
	else
	{
		if(qstrcmp(QSTR_CMP_ICASE, vprotocol, "res", 0))
		{
			strpath += Util::Dir::Program::GetResDir();
		}
		else if(qstrcmp(QSTR_CMP_ICASE, vprotocol, "xtml", 0))
		{
			strpath += Util::Dir::Program::GetXtmlDir();
		}
		else if(qstrcmp(QSTR_CMP_ICASE, vprotocol, "sysres", 0))
		{
			strpath += Util::Dir::Program::GetSysResDir();
		}
		else if(qstrcmp(QSTR_CMP_ICASE, vprotocol, "sysxtml", 0))
		{
			strpath += Util::Dir::Program::GetSysXtmlDir();
		}
		else if(qstrcmp(QSTR_CMP_ICASE, vprotocol, "gpres", 0))
		{
			strpath += Util::Dir::Program::GetGamePlusResDir();
		}
		else if(qstrcmp(QSTR_CMP_ICASE, vprotocol, "gpxtml", 0))
		{
			strpath += Util::Dir::Program::GetGamePlusXtmlDir();
		}
		else if(qstrcmp(QSTR_CMP_ICASE, vprotocol, "locale", 0))
		{
			strpath += Util::Dir::Program::GetChineseLocaleDir();
		}
	}
	nlen = qurlGetPath(str, vpath, sizeof(vpath));
	if(nlen > 0)
	{
		strpath.AppendFormat(_T("%s"), vpath);
	}

	return strpath;
}

#endif // QMDL_VER_GF
