#include "Jobs.h"


long CJob::SetFastaFilePath(const char* pStart, const char* pEnd)
{
	long lReturn = ERR_NOERROR;
	size_t sizeInputFile = pEnd - pStart;

	if (sizeInputFile == 0 || sizeInputFile > SIZE_BUFFER_ONE_GIG)
		return JOB_SETFASTAFILEPATH_POSITION_MIAF;

	if (m_szFastaFilePath)
		delete m_szFastaFilePath;

	m_szFastaFilePath = new char[sizeInputFile + 10];
	memset(m_szFastaFilePath, '\0', sizeInputFile + 10);
	memcpy(m_szFastaFilePath, pStart, sizeInputFile);

	return lReturn;
}

long CJob::GetFastaFilePath(char** ppInputFile)
{
	boost::lock_guard<boost::mutex> lock{ m_mutexIsRunningInThread };
	if (!m_szFastaFilePath)
		return JOB_GETFASTAFILEPATH_MIAF;

	*ppInputFile = m_szFastaFilePath;
	return ERR_NOERROR;
}

char* CJob::GetFastaFilePath()
{
	boost::lock_guard<boost::mutex> lock{ m_mutexIsRunningInThread };
	return m_szFastaFilePath;
}

long CJob::SetFastaFileName(const char* pStart, const char* pEnd)
{
	long lReturn = ERR_NOERROR;
	size_t sizeFastaFileName = pEnd - pStart;

	if (sizeFastaFileName == 0 || sizeFastaFileName > SIZE_BUFFER_ONE_GIG)
		return JOB_SETINPUTFILENAME_POSITION_MIAF;

	if (m_szFastaFileName)
		delete m_szFastaFileName;

	m_szFastaFileName = new char[sizeFastaFileName + 10];
	memset(m_szFastaFileName, '\0', sizeFastaFileName + 10);
	memcpy(m_szFastaFileName, pStart, sizeFastaFileName);

	return lReturn;
}

long CJob::GetFastaFileName(char** ppFastaFileName)
{
	boost::lock_guard<boost::mutex> lock{ m_mutexIsRunningInThread };
	if (!m_szFastaFileName)
		return JOB_GETFASTAFILENAME_MIAF;

	*ppFastaFileName = m_szFastaFileName;
	return ERR_NOERROR;
}

char* CJob::GetFastaFileName()
{
	boost::lock_guard<boost::mutex> lock{ m_mutexIsRunningInThread };
	return m_szFastaFileName;
}



