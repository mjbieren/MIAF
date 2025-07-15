#include "GlobalHelper.h"

long ParseFastaFiles(std::vector<CJob*> *pvecFastaFileJob, std::vector<std::string> *pvecDirList, size_t sizeDirList, std::string strFolderPath, long lCutOffStrainName, size_t * pIdxFastaFile)
{
	long lReturn = ERR_NOERROR;

	CJob* pFastaJob = nullptr;
	
	std::string strFileFullPath;
	std::string strFastaFileName;

	const char* pStart = nullptr;
	const char* pEnd = nullptr;
	for (size_t idx = 0; idx < sizeDirList; idx++)
	{
		pFastaJob = new CJob;
		strFastaFileName = pvecDirList->at(idx);
		//We want to copy the fasta file name, but we do not want to add .fasta/.fa/etc so we remove that.
		lReturn = pFastaJob->SetFastaFileName(strFastaFileName.c_str(), (strFastaFileName.c_str() + (strFastaFileName.length() - lCutOffStrainName)));
		if (lReturn != ERR_NOERROR)
		{
			delete pFastaJob;
			return lReturn;
		}

		//Set the full path
		strFileFullPath = strFolderPath + strFastaFileName;
		
		//Parse the fasta file into the object
		printf_s("Working on Fasta File Nr:%zu %s %s", *pIdxFastaFile,strFastaFileName.c_str(), EOL);
		*pIdxFastaFile = *pIdxFastaFile + 1;
		pStart = strFileFullPath.c_str();
		pEnd = pStart + strFileFullPath.size();
		lReturn = pFastaJob->SetFastaFilePath(pStart, pEnd);
		//Check if there is an error;
		if (lReturn != ERR_NOERROR)
		{
			delete pFastaJob;
			return lReturn;
		}

		//It's sorted alphabetically now and there are no errors, we can add the file into the vector.
		pvecFastaFileJob->push_back(pFastaJob);
		//NExt one :)
	}
	return lReturn;

}


long ParseInputFiles(std::vector<CJob*>* pvecJobsBase)
{
	//Parse all fasta files into fasta file object.
	/*
	1) Get the Path of the fasta files
	2) Get a dir list of all files in there, with .fasta, .fna, .ffn, .faa, .frn, .fa, .pep in them (7 in total).
	3) Check if there are some fasta files in them if not return error.
	
	*/
	long lReturn = ERR_NOERROR;

	size_t sizeFastaFilesTotal = 0;
	size_t sizeFastaDirList = 0;

	std::string strFastaFileDir;
	lReturn = glb.propertyBag.GetInputFolderPath(&strFastaFileDir);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_GETINPUTFOLDER_MIAF;


	//.FASTA
	std::vector<std::string> vecFastaFilesPaths;
	sizeFastaDirList = GetFilteredDirListBoost(strFastaFileDir.c_str(), "*.fasta", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;

	size_t idxFasta = 1;

	if (sizeFastaDirList != 0) //There are fasta files wtih the format .fasta
	{
		lReturn = ParseFastaFiles(pvecJobsBase, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 6, &idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();

	//.FNA
	sizeFastaDirList = GetFilteredDirListBoost(strFastaFileDir.c_str(), "*.fna", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;
	if (sizeFastaDirList != 0) //There are fasta files wtih the format .fna
	{
		lReturn = ParseFastaFiles(pvecJobsBase, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4,&idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();

	//.FFN
	sizeFastaDirList = GetFilteredDirListBoost(strFastaFileDir.c_str(), "*.ffn", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;
	if (sizeFastaDirList != 0) //There are fasta files wtih the format .ffn
	{
		lReturn = ParseFastaFiles(pvecJobsBase, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4,&idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();


	//.FAA
	sizeFastaDirList = GetFilteredDirListBoost(strFastaFileDir.c_str(), "*.faa", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;
	if (sizeFastaDirList != 0) //There are fasta files wtih the format .faa
	{
		lReturn = ParseFastaFiles(pvecJobsBase, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4, &idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();
	

	//.FRN
	sizeFastaDirList = GetFilteredDirListBoost(strFastaFileDir.c_str(), "*.frn", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;
	if (sizeFastaDirList != 0) //There are fasta files wtih the format .frn
	{
		lReturn = ParseFastaFiles(pvecJobsBase, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4, &idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();

	//.FA
	sizeFastaDirList = GetFilteredDirListBoost(strFastaFileDir.c_str(), "*.fa", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;
	if (sizeFastaDirList != 0) //There are fasta files wtih the format .fa
	{
		lReturn = ParseFastaFiles(pvecJobsBase, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 3, &idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();


	//.PEP
	sizeFastaDirList = GetFilteredDirListBoost(strFastaFileDir.c_str(), "*.pep", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;
	if (sizeFastaDirList != 0) //There are fasta files wtih the format .pep
	{
		lReturn = ParseFastaFiles(pvecJobsBase, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4, &idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();

	if (sizeFastaFilesTotal == 0)
		return DIRLIST_FAIL;


	//Sort the fasta files alphabetically
	//lReturn = SortFastaFilesAlphabeticallyMatchingName(pvecFastaFilesSource);
	return lReturn;
}