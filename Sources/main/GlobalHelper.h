#pragma once
#include "stdafx.h"
#include "ClearVector.h"
#include "Management.h"
#include "MIAF_PropertyBag.h"
#include "Jobs.h"


class CGlobal
{
protected:
	boost::mutex m_mutexIsRunningInThread;
	boost::thread* m_pThreadProc = nullptr;
	bool m_bThreadIsRunning = false;
	bool m_bInitVector = false;
	bool m_bAllJobsParcedToSlurm = false;
public:
	CGlobal() {};
	~CGlobal() {};

	CMIAFPropertyBag propertyBag;
	CManagement management;




};

extern CGlobal glb;


#define CHECK_RETURNVALUE(value)        \
if(value!=ERR_NOERROR)                  \
{                                       \
   glb.management.ErrorChecker(value);  \
   return (int) value;                        \
}                              

#define CHECK_RETURNVALUE_REL_PTR(value, ptr)        \
if(value!=ERR_NOERROR)                  \
{                                       \
    delete ptr;                         \
     glb.management.ErrorChecker(value); \
   return (int) value;                  \
}
