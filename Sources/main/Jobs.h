#pragma once
#include "GlobalHelper.h"

class CJob
{
	friend void threadMafftIQTree(CJob * pInfo);
protected:
	bool m_bIsRunning;
	char* m_szFastaFilePath = nullptr;
	char* m_szFastaFileName = nullptr;
	boost::mutex m_mutexIsRunningInThread;
	boost::thread * m_pThreadProc = nullptr;
public:
	CJob()
	{	
		m_bIsRunning = true;
	}
	~CJob()
	{
		if (m_szFastaFileName)
		{
			delete m_szFastaFileName;
		}

		if (m_szFastaFilePath)
		{
			delete m_szFastaFilePath;
		}

		if (m_pThreadProc)
		{
			delete m_pThreadProc;
		}
	}

	long SetFastaFilePath(const char* pStart, const char* pEnd);
	long GetFastaFilePath(char** ppFastaFileName);
	char* GetFastaFilePath();

	long SetFastaFileName(const char* pStart, const char* pEnd);
	long GetFastaFileName(char** ppFastaFileName);
	char* GetFastaFileName();


	void SetThreadProc(boost::thread * pThreadProc) {m_pThreadProc = pThreadProc;}
	boost::thread *  GetThreadProc() { return m_pThreadProc; }
	void ThreadIsFinished()
	{
		boost::lock_guard<boost::mutex> lock{ m_mutexIsRunningInThread };
		m_bIsRunning = false;
	}
	bool IsRunning()
	{
		boost::lock_guard<boost::mutex> lock{ m_mutexIsRunningInThread };
		return m_bIsRunning;
	}
};