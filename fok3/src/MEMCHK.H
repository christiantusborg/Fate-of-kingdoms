
#if !defined(MEMCHK_H)
#define MEMCHK_H

#if defined(_DEBUG) && !defined(NOMEMCHK)


#include <crtdbg.h>

	extern _CrtMemState gState;
	extern char gChkName[];

#define L_CHKSTART(name) \
	char sName[] = #name; \
	_CrtMemState lState;\
	_CrtMemCheckpoint(&lState); \
	bug("Entering local checkpoint: %s\n", sName);

#define L_CHKEND() \
	bug("Leaving local checkpoint: %s\n", sName); \
	_CrtMemDumpAllObjectsSince(&lState);

#define L_CHKENDBRK() \
	bug("Leaving local checkpoint: %s\n", sName); \
	DebugBreak(); \
	_CrtMemDumpAllObjectsSince(&lState);

#define G_CHKSTART(name) \
	strcpy(gChkName, #name); \
	_CrtMemCheckpoint(&gState); \
	bug("Entering global checkpoint: %s\n", gChkName);

#define G_CHKEND() \
	bug("Leaving global checkpoint: %s\n", gChkName); \
	_CrtMemDumpAllObjectsSince(&gState);

#define G_CHKENDBRK() \
	bug("Leaving global checkpoint: %s\n", gChkName); \
	DebugBreak(); \
	_CrtMemDumpAllObjectsSince(&gState);

#define A_CHKSTART(name) \
	char sName[] = #name; \
	_CrtMemState inState;\
	_CrtMemState outState;\
	_CrtMemState difState;\
	static bool aCkg##name##First = TRUE; \
	if(!aCkg##name##First) \
	{ \
		_CrtMemCheckpoint(&inState); \
		bug("Entering local AUTO checkpoint: %s\n", sName); \
	}

#define A_CHKEND(name) \
	if(aCkg##name##First) \
	{ \
		aCkg##name##First = FALSE; \
	} \
	else \
	{ \
		_CrtMemCheckpoint(&outState); \
		if(_CrtMemDifference(&difState, &inState, &outState) ) \
		{ \
			bug("Auto checkpoint: %s detected difference:\n", gChkName); \
			_CrtMemDumpAllObjectsSince(&inState); \
		} \
		else \
		{ \
			bug("Leaving auto checkpoint: %s\n", gChkName); \
		} \
	}

#define A_CHKENDBRK(name) \
	if(aCkg##name##First) \
	{ \
		aCkg##name##First = FALSE; \
	} \
	else \
	{ \
		_CrtMemCheckpoint(&outState); \
		if(_CrtMemDifference(&difState, &inState, &outState) ) \
		{ \
			bug("Auto checkpoint: %s detected difference:\n", gChkName); \
			DebugBreak(); \
			_CrtMemDumpAllObjectsSince(&inState); \
		} \
		else \
		{ \
			bug("Leaving auto checkpoint: %s\n", gChkName); \
		} \
	}

#else

#define L_CHKSTART(name)
#define L_CHKEND()
#define L_CHKENDBRK()
#define G_CHKSTART(name)
#define G_CHKEND()
#define G_CHKENDBRK()
#define A_CHKSTART(name)
#define A_CHKEND(name)
#define A_CHKENDBRK(name)

#endif

#endif	/* #if !defined(MEMCHK_H) */
