
#ifndef __MD5_H__
#define __MD5_H__

//#include "cg_common.h"



#ifdef __cplusplus
extern "C"
{
#endif

extern int MD5String(char* szInput,int nLen, char *szOutStr);
extern int MD5StringUpperCase(char* szInput, int nLen, char *szOutStr);

#ifdef __cplusplus
}
#endif

#endif
