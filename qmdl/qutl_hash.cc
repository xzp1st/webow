
#include "qmdl_util.h"


#define GHASH_MIN_SIZE								11
#define GHASH_MAX_SIZE								13845163


struct qmdl_hash_node
{
	QPNT pkey;
	QPNT pvalue;
	QPCB pcbfree;
	struct qmdl_hash_node *pnext;
};

struct qmdl_hash_data
{
	QINT nsize;
	QINT nnodesize;
	struct qmdl_hash_node **pnodelist;
	QHDL hprmlist;
	qhash_value_cb pcbvalue;
	qhash_equal_cb pcbequal;
};


static QINT qhashResize(QHDL hash);

#define GHASH_RESIZE(hash)																\
	do {																				\
		if(	(hash->nsize >= 3 * hash->nnodesize && hash->nsize > GHASH_MIN_SIZE) ||		\
			(3 * hash->nsize <= hash->nnodesize && hash->nsize < GHASH_MAX_SIZE) )		\
		{ qhashResize (hash);	}														\
	} while(0)


QHDL qhashMake(QHDL mem)
{
	QINT nindex;
	struct qmdl_hash_data *pdata;
	struct qmdl_hash_node **ppnode;

	pdata = (struct qmdl_hash_data *)qmalloc(mem, sizeof(struct qmdl_hash_data));
	if(pdata == NULL)
	{
		return NULL;
	}
	memset(pdata, 0, sizeof(struct qmdl_hash_data));
	pdata->nsize = GHASH_MIN_SIZE;
	pdata->nnodesize = 0;
	pdata->hprmlist = NULL;
	pdata->pcbvalue = qhash_direct_value_cb;
	pdata->pcbequal = NULL;
	pdata->pnodelist = (struct qmdl_hash_node **)qmalloc(mem, sizeof(struct qmdl_hash_node *)*pdata->nsize);
	if(pdata->pnodelist == NULL)
	{
		goto CREATE_FAILED;
	}
	nindex = 0;
	ppnode = pdata->pnodelist;
	while(nindex < pdata->nsize)
	{
		*ppnode++ = NULL;
		nindex++;
	}

	return (QHDL )pdata;

CREATE_FAILED:
	if(pdata != NULL)
	{
		if(pdata->pnodelist != NULL)
		{
			qmfree(pdata->pnodelist);
		}
		qmfree(pdata);
	}

	return NULL;
}

QINT qhashFree(QHDL hash)
{
	struct qmdl_hash_data *pdata;

	pdata = (struct qmdl_hash_data *)hash;
	if(pdata == NULL)
	{
		return 0;
	}
	if(pdata->pnodelist != NULL)
	{
		qhashDelNode(hash, NULL);
		qmfree(pdata->pnodelist);
	}
	if(pdata->hprmlist != NULL)
	{
		qprmfree(pdata->hprmlist);
	}
	qmfree(pdata);

	return 0;
}

QINT qhashGetSize(QHDL hash)
{
	struct qmdl_hash_data *pdata;

	pdata = (struct qmdl_hash_data *)hash;
	if(pdata == NULL)
	{
		return -1;
	}

	return pdata->nnodesize;
}

qhash_value_cb qhashSetValueCb(QHDL hash, qhash_value_cb value_cb)
{
	struct qmdl_hash_data *pdata;
	qhash_value_cb pcbvalue;

	pdata = (struct qmdl_hash_data *)hash;
	if(pdata == NULL)
	{
		return NULL;
	}
	pcbvalue = pdata->pcbvalue;
	pdata->pcbvalue = value_cb;

	return pcbvalue;
}

qhash_value_cb qhashGetValueCb(QHDL hash)
{
	struct qmdl_hash_data *pdata;

	pdata = (struct qmdl_hash_data *)hash;
	if(pdata == NULL)
	{
		return NULL;
	}

	return pdata->pcbvalue;
}

qhash_equal_cb qhashSetEqualCb(QHDL hash, qhash_equal_cb equal_cb)
{
	struct qmdl_hash_data *pdata;
	qhash_equal_cb pcbequal;

	pdata = (struct qmdl_hash_data *)hash;
	if(pdata == NULL)
	{
		return NULL;
	}
	pcbequal = pdata->pcbequal;
	pdata->pcbequal = equal_cb;

	return pcbequal;
}

qhash_equal_cb qhashGetEqualCb(QHDL hash)
{
	struct qmdl_hash_data *pdata;

	pdata = (struct qmdl_hash_data *)hash;
	if(pdata == NULL)
	{
		return NULL;
	}

	return pdata->pcbequal;
}

QPNT qhashSetParamsx(QHDL hash, QPCB free_cb, QPNT params[], QINT count)
{
	struct qmdl_hash_data *pdata;

	pdata = (struct qmdl_hash_data *)hash;
	if(pdata == NULL)
	{
		return NULL;
	}
	if(pdata->hprmlist != NULL)
	{
		qprmfree(pdata->hprmlist);
	}

	return qprmmakex(hash, NULL, 0, NULL, free_cb, params, &count);
}

QPNT qhashSetParams(QHDL hash, QPCB free_cb, QINT count, ...)
{
	QPNT presult;
	QPNT *ppprms;
	QCHR vbuff[QPRM_BUFF_SIZE] = {0};
	va_list marker;
	
	va_start(marker, count);
	ppprms = (QPNT *)qprmmakev(hash, vbuff, sizeof(vbuff), NULL, NULL, marker, &count);
	va_end(marker);
	
	presult = qhashSetParamsx(hash, free_cb, ppprms, count);
	
	qprmfree(ppprms);
	
	return presult;
}

QPNT qhashGetParams(QHDL hash)
{
	struct qmdl_hash_data *pdata;

	pdata = (struct qmdl_hash_data *)hash;
	if(pdata == NULL)
	{
		return NULL;
	}

	return (QPNT )pdata->hprmlist;
}

QINT qhashInitx(QHDL hash, qhash_value_cb value_cb, qhash_equal_cb equal_cb, QPCB free_cb, QPNT params[], QINT count)
{
	qhashSetValueCb(hash, value_cb);
	qhashSetEqualCb(hash, equal_cb);
	qhashSetParamsx(hash, free_cb, params, count);

	return 0;
}

QHDL qhashInit(QHDL hash, qhash_value_cb value_cb, qhash_equal_cb equal_cb, QPCB free_cb, QINT count, ...)
{
	QHDL hresult;
	QPNT *ppprms;
	QCHR vbuff[QPRM_BUFF_SIZE] = {0};
	va_list marker;
	
	if(hash != NULL)
	{
		hresult = hash;
	}
	else
	{
		hresult = qhashMake(NULL);
		if(hresult == NULL)
		{
			return NULL;
		}
	}
	
	va_start(marker, count);
	ppprms = (QPNT *)qprmmakev(hresult, vbuff, sizeof(vbuff), NULL, NULL, marker, &count);
	va_end(marker);
	
	qhashSetValueCb(hresult, value_cb);
	qhashSetEqualCb(hresult, equal_cb);
	qhashSetParamsx(hresult, free_cb, ppprms, count);
	
	qprmfree(ppprms);
	
	return hresult;
}

static QHDL qhashMakeNode(QHDL mem, QPCB free_cb, QPNT key, QPNT value)
{
	struct qmdl_hash_node *pnode;

	pnode = (struct qmdl_hash_node *)qmalloc(mem, sizeof(struct qmdl_hash_node));
	if(pnode == NULL)
	{
		return NULL;
	}
	memset(pnode, 0, sizeof(struct qmdl_hash_node));
	pnode->pcbfree = free_cb;
	pnode->pkey = key;
	pnode->pvalue = value;

	return (QHDL )pnode;
}

static QINT qhashFreeNode(QHDL hash, QHDL node)
{
	struct qmdl_hash_data *pdata;
	struct qmdl_hash_node *pnode;

	pdata = (struct qmdl_hash_data *)hash;
	if(pdata == NULL)
	{
		return 0;
	}
	pnode = (struct qmdl_hash_node *)node;
	if(pnode == NULL)
	{
		return 0;
	}
	if(pnode->pcbfree != NULL)
	{
		pnode->pcbfree(hash, node, QCD_CALL, (QPNT *)pdata->hprmlist, qprmcnt(pdata->hprmlist));
	}
	qmfree(pnode);

	return 0;
}

static struct qmdl_hash_node **qhashFindNode(QHDL hash, QPNT key)
{
	QINT nvalue;
	struct qmdl_hash_data *pdata;
	struct qmdl_hash_node **ppnode;

	pdata = (struct qmdl_hash_data *)hash;
	if(pdata == NULL)
	{
		return NULL;
	}
	if(pdata->pcbvalue == NULL)
	{
		return NULL;
	}
	nvalue = pdata->pcbvalue(hash, key, (QPNT *)pdata->hprmlist, qprmcnt(pdata->hprmlist));
	ppnode = &pdata->pnodelist[nvalue % pdata->nsize];
	if(pdata->pcbequal == NULL)
	{
		while(*ppnode)
		{
			if((*ppnode)->pkey == key)
			{
				break;
			}
			ppnode = &(*ppnode)->pnext;
		}
	}
	else
	{
		while(*ppnode)
		{
			if(pdata->pcbequal(hash, (*ppnode)->pkey, key, (QPNT *)pdata->hprmlist, qprmcnt(pdata->hprmlist)))
			{
				break;
			}
			ppnode = &(*ppnode)->pnext;
		}
	}

	return ppnode;
}

QHDL qhashGetValue(QHDL hash, QPNT key)
{
	struct qmdl_hash_node **ppnode;

	ppnode = qhashFindNode(hash, key);
	if(ppnode == NULL)
	{
		return NULL;
	}
	if(*ppnode == NULL)
	{
		return NULL;
	}

	return (*ppnode)->pvalue;
}

QHDL qhashGetNode(QHDL hash, QPNT key)
{
	struct qmdl_hash_node **ppnode;

	ppnode = qhashFindNode(hash, key);
	if(ppnode == NULL)
	{
		return NULL;
	}

	return (*ppnode);
}

QPNT qhashNodeGetKey(QHDL node)
{
	struct qmdl_hash_node *pnode;

	pnode = (struct qmdl_hash_node *)node;
	if(pnode == NULL)
	{
		return NULL;
	}

	return pnode->pkey;
}

QPNT qhashNodeGetValue(QHDL node)
{
	struct qmdl_hash_node *pnode;

	pnode = (struct qmdl_hash_node *)node;
	if(pnode == NULL)
	{
		return NULL;
	}

	return pnode->pvalue;
}

QPNT qhashNodeSetValue(QHDL node, QPNT value)
{
	QPNT pvalue;
	struct qmdl_hash_node *pnode;

	pnode = (struct qmdl_hash_node *)node;
	if(pnode == NULL)
	{
		return NULL;
	}
	pvalue = pnode->pvalue;
	pnode->pvalue = value;

	return pvalue;
}

QPCB qhashNodeGetFreeCb(QHDL node)
{
	struct qmdl_hash_node *pnode;

	pnode = (struct qmdl_hash_node *)node;
	if(pnode == NULL)
	{
		return NULL;
	}

	return pnode->pcbfree;
}

QPCB qhashNodeSetFreeCb(QHDL node, QPCB free_cb)
{
	QPCB pcbfree;
	struct qmdl_hash_node *pnode;

	pnode = (struct qmdl_hash_node *)node;
	if(pnode == NULL)
	{
		return NULL;
	}
	pcbfree = pnode->pcbfree;
	pnode->pcbfree = free_cb;

	return pcbfree;
}

static const int gvprimelist[] =
{
	11,
	19,
	37,
	73,
	109,
	163,
	251,
	367,
	557,
	823,
	1237,
	1861,
	2777,
	4177,
	6247,
	9371,
	14057,
	21089,
	31627,
	47431,
	71143,
	106721,
	160073,
	240101,
	360163,
	540217,
	810343,
	1215497,
	1823231,
	2734867,
	4102283,
	6153409,
	9230113,
	13845163,
};

static QINT qhashResize(QHDL hash)
{
	struct qmdl_hash_data *pdata;
	QINT nindex, ntotal, nsize, nvalue;
	struct qmdl_hash_node *pnode, *pnext, **ppnode;

	pdata = (struct qmdl_hash_data *)hash;
	if(pdata == NULL)
	{
		return -1;
	}
	// 找到最接近的散列表大小。
	nindex = 0;
	ntotal = sizeof(gvprimelist)/sizeof(gvprimelist[0]);
	while(nindex < ntotal)
	{
		if(gvprimelist[nindex] > pdata->nnodesize)
		{
			break;
		}
		nindex++;
	}
	if(nindex >= ntotal)
	{
		nindex = ntotal-1;
	}
	nsize = gvprimelist[nindex];
	if(nsize > GHASH_MAX_SIZE)
	{
		nsize = GHASH_MAX_SIZE;
	}
	else if(nsize < GHASH_MIN_SIZE)
	{
		nsize = GHASH_MIN_SIZE;
	}
	ppnode = (struct qmdl_hash_node **)qmalloc(hash, sizeof(struct qmdl_hash_node *)*nsize);
	if(ppnode == NULL)
	{
		return -1;
	}
	nindex = 0;
	while(nindex < pdata->nsize)
	{
		pnode = pdata->pnodelist[nindex];
		while(pnode)
		{
			pnext = pnode->pnext;
			nvalue = pdata->pcbvalue(hash, pnode->pkey, (QPNT *)pdata->hprmlist, qprmcnt(pdata->hprmlist))%nsize;
			pnode->pnext = ppnode[nvalue];
			ppnode[nvalue] = pnode;
			pnode = pnext;
		}
		nindex++;
	}
	qmfree(pdata->pnodelist);
	pdata->pnodelist = ppnode;
	pdata->nsize = nsize;

	return 0;
}

QHDL qhashAddNode(QHDL hash, QPCB free_cb, QPNT key, QPNT value)
{
	struct qmdl_hash_data *pdata;
	struct qmdl_hash_node *pnode, **ppnode;

	pdata = (struct qmdl_hash_data *)hash;
	if(pdata == NULL)
	{
		return NULL;
	}
	ppnode = (struct qmdl_hash_node **)qhashFindNode(hash, key);
	if(ppnode == NULL)
	{
		return NULL;
	}
	pnode = *ppnode;
	if(pnode != NULL)
	{
		if(pnode->pcbfree != NULL)
		{
			pnode->pcbfree(hash, pnode, QCD_CALL, (QPNT *)pdata->hprmlist, qprmcnt(pdata->hprmlist));
		}
		pnode->pcbfree = free_cb;
		pnode->pkey = key;
		pnode->pvalue = value;
	}
	else
	{
		pnode = (struct qmdl_hash_node *)qhashMakeNode(hash, free_cb, key, value);
		if(pnode == NULL)
		{
			return NULL;
		}
		*ppnode = pnode;
		pdata->nnodesize++;
		GHASH_RESIZE(pdata);
	}

	return (QHDL )pnode;
}

QINT qhashDelNode(QHDL hash, QPNT key)
{
	QINT nindex, ncount;
	struct qmdl_hash_data *pdata;
	struct qmdl_hash_node *pnode, **ppnode;

	pdata = (struct qmdl_hash_data *)hash;
	if(pdata == NULL)
	{
		return -1;
	}
	if(key == NULL)
	{
		nindex = 0;
		ncount = 0;
		while(nindex < pdata->nsize)
		{
			ppnode = &(pdata->pnodelist[nindex]);
			while(*ppnode)
			{
				pnode = *ppnode;
				*ppnode = pnode->pnext;
				qhashFreeNode(hash, pnode);
				pdata->nnodesize--;
				ncount++;
			}
			nindex++;
		}
		GHASH_RESIZE(pdata);
	}
	else
	{
		ppnode = (struct qmdl_hash_node **)qhashFindNode(hash, key);
		if(ppnode == NULL)
		{
			return -1;
		}
		if(*ppnode == NULL)
		{
			return -1;
		}
		ncount = 1;
		pnode = *ppnode;
		*ppnode = pnode->pnext;
		qhashFreeNode(hash, pnode);
		pdata->nnodesize--;
		GHASH_RESIZE(pdata);
	}

	return 0;
}

QINT qhashScan(QHDL hash, QPCB scan_cb, QPNT in, QPNT out)
{
	struct qmdl_hash_data *pdata;
	QINT nindex, ncount, nresult;
	struct qmdl_hash_node **ppnode;

	pdata = (struct qmdl_hash_data *)hash;
	if(pdata == NULL || scan_cb == NULL)
	{
		return -1;
	}
	nindex = 0;
	ncount = 0;
	while(nindex < pdata->nsize)
	{
		ppnode = &(pdata->pnodelist[nindex]);
		while(*ppnode)
		{
			nresult = scan_cb(hash, *ppnode, QCD_CALL, (QPNT *)pdata->hprmlist, qprmcnt(pdata->hprmlist));
			if(nresult != QSCN_OK)
			{
				break;
			}
			ncount++;
			ppnode = &((*ppnode)->pnext);
		}
		nindex++;
	}

	return ncount;
}


QINT qhash_int_value_cb(QHDL hash, QPNT key, QPNT params[], QINT count)
{
	return (((QINT )key * 2654435769) >> 28);
}

QINT qhash_int_equal_cb(QHDL hash, QPNT key1, QPNT key2, QPNT params[], QINT count)
{
	return (key1==key2);
}

QINT qhash_direct_value_cb(QHDL hash, QPNT key, QPNT params[], QINT count)
{
	return (QINT )key;
}

QINT qhash_direct_equal_cb(QHDL hash, QPNT key1, QPNT key2, QPNT params[], QINT count)
{
	return (key1==key2);
}

QINT qhash_commonstr_value_cb(QHDL hash, QPNT key, QPNT params[], QINT count)
{
	QUINT h;
	QUCHR *p;

	for(h=0, p=(unsigned char *)key; *p; p++)
	{
		h = 31*h + *p;
	}

	return (int )h;
}

QINT qhash_commonstr_equal_cb(QHDL hash, QPNT key1, QPNT key2, QPNT params[], QINT count)
{
	return qstrcmp(QSTR_NONE, key1, key2, 0);
}

QINT qhash_phpstr_value_cb(QHDL hash, QPNT key, QPNT params[], QINT count)
{
	unsigned long h = 0, g;
	QCHR *arKey = (QCHR *)key, *arEnd = (QCHR *)key+qstrlen(key);

	while(arKey < arEnd)
	{
		h = (h << 4) + *arKey++;
		if((g = (h & 0xF0000000)))
		{
			h = h ^ (g >> 24);
			h = h ^ g;
		}
	}

	return (QINT )h;
}

QINT qhash_phpstr_equal_cb(QHDL hash, QPNT key1, QPNT key2, QPNT params[], QINT count)
{
	return qstrcmp(QSTR_NONE, key1, key2, 0);
}

// mysql
QINT qhash_calcstr_value_cb(QHDL hash, QPNT key, QPNT params[], QINT count)
{
	QCHR *pkey = (QCHR *)key;
	const QCHR *end = (QCHR *)key+qstrlen(key);
	QUINT value;

	for(value = 0; pkey < end; pkey++)
	{
		value *= 16777619;
		value ^= (QUINT )*(QUCHR *)pkey;
	}

	return value;
}

QINT qhash_calcstr_equal_cb(QHDL hash, QPNT key1, QPNT key2, QPNT params[], QINT count)
{
	return qstrcmp(QSTR_NONE, key1, key2, 0);
}

QINT qhash_calcstr2_value_cb(QHDL hash, QPNT key, QPNT params[], QINT count)
{
	QUINT hsval = 2654435769;
	QINT i;
	QINT iShift = 0;
	QINT iLen = qstrlen(key);
	QCHR *pkey = (QCHR *)key;

	for(i=0; i < iLen; i++)
	{
		hsval ^= (pkey[i]<<iShift);
		iShift+=3;
		if(iShift>24)
			iShift=0;
	}

	return hsval;
}

QINT qhash_calcstr2_equal_cb(QHDL hash, QPNT key1, QPNT key2, QPNT params[], QINT count)
{
	return qstrcmp(QSTR_NONE, key1, key2, 0);
}

// MPQ改进版
static QINT qhash_mpqstr_free_cb(QHDL hash, QPNT node, QINT code, QPNT params[], QINT count)
{
	unsigned long *pcrypttable;

	if(params == NULL || count <= 0)
	{
		return 0;
	}
	pcrypttable = (unsigned long *)params[0];
	if(pcrypttable == NULL)
	{
		return 0;
	}
	qmfree(pcrypttable);

	return 0;
}

QINT qhash_toupper(QINT chr)
{
	return (chr >= 'a' && chr <= 'z') ? ('A'+chr-'a') : chr;
}

QINT qhash_mpqstr_value_cb(QHDL hash, QPNT key, QPNT params[], QINT count)
{
	unsigned long *pcrypttable;
	unsigned long seed = 0x00100001, index1 = 0, index2 = 0, i;

	unsigned char *pkey  = (QUCHR *)key;
	unsigned long seed1 = 0x7FED7FED;
	unsigned long seed2 = 0xEEEEEEEE;
	unsigned long dwHashType = 0; // 可取值0、1、2
	QINT ch;

	if(params != NULL && count > 0)
	{
		pcrypttable = (unsigned long *)params[0];
	}
	else
	{
		pcrypttable = NULL;
	}
	if(pcrypttable == NULL)
	{
		pcrypttable = (unsigned long *)qmalloc(hash, sizeof(unsigned long)*0x500);
		if(pcrypttable == NULL)
		{
			return -1;
		}
		for( index1 = 0; index1 < 0x100; index1++ )  
		{   
			for( index2 = index1, i = 0; i < 5; i++, index2 += 0x100 )  
			{   
				unsigned long temp1, temp2;  

				seed = (seed * 125 + 3) % 0x2AAAAB;  
				temp1 = (seed & 0xFFFF) << 0x10;  

				seed = (seed * 125 + 3) % 0x2AAAAB;  
				temp2 = (seed & 0xFFFF);  

				pcrypttable[index2] = ( temp1 | temp2 );   
			}   
		}
		qhashSetParams(hash, qhash_mpqstr_free_cb, 1, pcrypttable);
	}	
	while( *pkey != 0 )  
	{   
		ch = qhash_toupper(*pkey++);  
		seed1 = pcrypttable[(dwHashType << 8) + ch] ^ (seed1 + seed2);  
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;   
	}  

	return seed1;   
}

QINT qhash_mpqdstr_equal_cb(QHDL hash, QPNT key1, QPNT key2, QPNT params[], QINT count)
{
	return qstrcmp(QSTR_NONE, key1, key2, 0);
}

