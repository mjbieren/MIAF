#include "GlobalHelper.h"
void threadMafftIQTree(CJob* pInfo)
{
	static bool bPrequal;
	static bool bIQtree;
	static int iMafftIQTreeCheck = -99;

	if (iMafftIQTreeCheck == -99)
	{
		glb.propertyBag.GetBoolPrequal(&bPrequal);
		glb.propertyBag.GetBoolIQtree(&bIQtree);
		iMafftIQTreeCheck = 5;
	}

	//Set the Variables
	static std::string strInputPath;
	if (strInputPath.length() == 0)
	{
		glb.propertyBag.GetInputFolderPath(&strInputPath);
	}

	static std::string strOutputPath;
	if (strOutputPath.length() == 0)
	{
		glb.propertyBag.GetOutputFolderPath(&strOutputPath);
	}

	static std::string strMafftPath;
	if (strMafftPath.length() == 0)
	{
		glb.propertyBag.GetMAFFTPath(&strMafftPath);
	}

	
	static std::string strIQtreePath;
	if (strIQtreePath.length() == 0)
	{
		glb.propertyBag.GetIQTreePath(&strIQtreePath);
	}

	static std::string strOutputFolderForMSAandTrees;
	if (strOutputFolderForMSAandTrees.length() == 0)
	{
		strOutputFolderForMSAandTrees = strOutputPath + "MSA_and_Trees/";
	}
	static std::string strOutputFolderPrequal;
	static std::string strOutputFolderPrequal_IQTree;
	if (strOutputFolderPrequal.length() == 0)
	{
		strOutputFolderPrequal = strOutputPath + "Prequal_Processed/";
		if (bIQtree == true) //We also have to make the IQTree output file
			strOutputFolderPrequal_IQTree = strOutputPath + "Prequal_MSA_and_Trees/";

	}

	static std::string strMoveFastaFileWhenDoneFolder;
	if (strMoveFastaFileWhenDoneFolder.length() == 0)
	{
		strMoveFastaFileWhenDoneFolder = strOutputPath + "Fasta_Files_Processed/";
	}

	static unsigned long ulIQThreadLimit = 0;
	if (ulIQThreadLimit == 0)
	{
		glb.propertyBag.GetIQThreadLimitation(&ulIQThreadLimit);
	}

	static std::string strIQThreadlimit;
	if (strIQThreadlimit.length() == 0)
	{
		strIQThreadlimit = std::to_string(ulIQThreadLimit);
	}
	
	std::string strFastaFilePath = pInfo->GetFastaFilePath();
	std::string strFastaFileName = pInfo->GetFastaFileName();

	if (bPrequal == false)
	{
		std::string strMafftCommand = strMafftPath + " --thread " + strIQThreadlimit + " --maxiterate 1000 --quiet --globalpair " + strFastaFilePath + " > " + strInputPath + pInfo->GetFastaFileName() + ".mafft";
#ifdef _DEBUG
		printf_s("%s%s", strMafftCommand.c_str(), EOL);
#endif

		std::string strIQTreeCommand = strIQtreePath + " -fast -nt " + strIQThreadlimit + " -s " + strInputPath + strFastaFileName + ".mafft" + " -st AA -m TEST -alrt 1000";
#ifdef _DEBUG
		printf_s("%s%s", strIQTreeCommand.c_str(), EOL);
#endif
		std::string strCopyMSACommand = "cp " + strInputPath + strFastaFileName + ".mafft " + strOutputFolderForMSAandTrees + strFastaFileName + ".mafft";
		std::string strCopyTreeCommand = "cp " + strInputPath + strFastaFileName + ".mafft.treefile " + strOutputFolderForMSAandTrees + pInfo->GetFastaFileName() + ".treefile";
		std::string strMVOldFastaFile = "mv " + strFastaFilePath + " " + strMoveFastaFileWhenDoneFolder + strFastaFileName + ".fa";

		/*Order of Commands
			mafft --thread 1 --maxiterate 1000 --globalpair ${WORK_DIR}/$f.fa > ${WORK_DIR}/$f.mafft
			IQtree -fast -nt 1 -s ${WORK_DIR}/$f.mafft -st AA -m TEST -alrt 1000"

			cp ${WORK_DIR}/$f.mafft strOutputFolderForMSAandTrees/$f.fa
			cp ${WORK_DIR}/$f.mafft.treefile strOutputFolderForMSAandTrees/$f.treefile

		*/

		std::string strTotalCommand = "{ "+strMafftCommand + "; " + strIQTreeCommand + "; " + strCopyMSACommand + "; " + strCopyTreeCommand + "; " + strMVOldFastaFile + "; } >>" + strInputPath + strFastaFileName + ".MIAF_Mafft_IQtree.log 2>&1";
		//1 MAFFT, 2 IQTree, 3 Copy MSA file to output, 4 Copy Tree file to ouput, 5 Move Original Fasta file to processed files done
		system(strTotalCommand.c_str());

	}
	else //Prequal/mafft/clipkit
	{
		if (bIQtree == false)
		{
			std::string strPrequalCommand = "prequal " + strFastaFilePath;
			std::string strMafftCommand = strMafftPath + " --thread " + strIQThreadlimit + " --maxiterate 1000 --quiet --globalpair --allowshift --unalignlevel 0.8 " + strFastaFilePath + ".filtered > " + strFastaFilePath + ".filtered.qinsi";
			std::string strClipkitCommand = "clipkit -m gappy -g 0.75 -log -o " + strFastaFilePath + ".filtered.qinsi.g075 " + strFastaFilePath + ".filtered.qinsi";
			std::string strMVOldFastaFile = "mv " + strFastaFilePath + " " + strMoveFastaFileWhenDoneFolder + strFastaFileName + ".fa";
			std::string strCopyFilteredFile = "cp " + strFastaFilePath + ".filtered.qinsi.g075 " + strOutputFolderPrequal + strFastaFileName + ".fa";

			std::string strTotalCommand = "{ " + strPrequalCommand + "; " + strMafftCommand + "; " + strClipkitCommand + "; " + strMVOldFastaFile + "; " + strCopyFilteredFile + "; } >>" + strFastaFilePath + ".MIAF_Prequal.log 2>&1";

			//1 prequal, 2 qinsi, 3 clipkit, 4 copy filtered file to output folder
			system(strTotalCommand.c_str());
		}
		else
		{
			std::string strPrequalCommand = "prequal " + strFastaFilePath;
			std::string strMafftCommand = strMafftPath + " --thread " + strIQThreadlimit + " --maxiterate 1000 --quiet --globalpair --allowshift --unalignlevel 0.8 " + strInputPath + pInfo->GetFastaFileName() + ".fa.filtered > " + strInputPath + pInfo->GetFastaFileName() + ".fa.filtered.qinsi";
			std::string strIQTreeCommand = strIQtreePath + " -fast -nt " + strIQThreadlimit + " -s " + strInputPath + pInfo->GetFastaFileName() + ".fa.filtered.qinsi -st AA -m TEST -msub nuclear -alrt 1000";
			std::string strClipkitCommand = "clipkit -m gappy -g 0.75 -log -o " + strFastaFilePath + ".filtered.qinsi.g075 " + strFastaFilePath+ ".filtered.qinsi";
			std::string strIQTreeCommandClipKit = strIQtreePath + " -fast -nt " + strIQThreadlimit + " -s " + strFastaFilePath + ".filtered.qinsi.g075 -st AA -m TEST -msub nuclear -alrt 1000";
			std::string strMVOldFastaFile = "mv " + strFastaFilePath + " " + strMoveFastaFileWhenDoneFolder + strFastaFileName + ".fa";
			std::string strCopyFilteredFile = "cp " + strFastaFilePath + ".filtered.qinsi.g075 " + strOutputFolderPrequal + strFastaFileName + ".fa";
			std::string strCopyTreeFile = "cp " + strFastaFilePath + ".filtered.qinsi.treefile " + strOutputFolderPrequal_IQTree + strFastaFileName + ".qinsi.treefile";
			std::string strCopyMSAFile = "cp " + strFastaFilePath + ".filtered.qinsi " + strOutputFolderPrequal_IQTree + strFastaFileName + "qinsi.fa";
			std::string strCopyTreeFileClipKit = "cp " + strFastaFilePath + ".filtered.qinsi.g075.treefile " + strOutputFolderPrequal_IQTree + strFastaFileName + ".qinsi_g075.treefile";
			std::string strCopyMSAFileClipKit = "cp " + strFastaFilePath + ".filtered.qinsi.g075 " + strOutputFolderPrequal_IQTree + strFastaFileName + ".qinsi_g075.fa";

			std::string strTotalCommand = "{ "+ strPrequalCommand + "; " + strMafftCommand + "; " + strIQTreeCommand + "; "+ strClipkitCommand + ";" + strIQTreeCommandClipKit + "; "+ strMVOldFastaFile + "; " + strCopyFilteredFile + "; " + strCopyTreeFile + "; " + strCopyMSAFile + "; " + strCopyTreeFileClipKit + "; " + strCopyMSAFileClipKit + "; } >>" + strFastaFilePath + ".MIAF_Prequal.log 2>&1";

			//1 prequal, 2 qinsi, 3 clipkit, 4 copy filtered file to output folder
			system(strTotalCommand.c_str());

		}
	}

	//We are done and can set the thread to finish.
	pInfo->ThreadIsFinished();

}


long WaitForAllThreads(std::vector<CJob*>* pvecJobsThreadInfo, unsigned long * pulJobsDone, unsigned long ulTimeJobLimit, std::chrono::time_point<std::chrono::steady_clock> TimeStart, SYSTEMTYPE eSystemtype, std::string strScriptPath)
{
	long lReturn = ERR_NOERROR;

	std::vector<CJob*>::iterator iter = pvecJobsThreadInfo->begin();
	long ulThreadsRunning = 0;

	std::chrono::time_point<std::chrono::steady_clock> TimeNow;

	std::string strScriptRestartCommand;

	while (true)
	{
		ulThreadsRunning = 0;
		while (iter != pvecJobsThreadInfo->end())
		{
			if ((*iter)->IsRunning())
				ulThreadsRunning++;
			else
			{
				pvecJobsThreadInfo->erase(iter);
				iter = pvecJobsThreadInfo->begin();
				ulThreadsRunning = 0;
				*pulJobsDone = *pulJobsDone + 1;
				continue;
			}
			iter++;
		}

		//If we reach the time limit (we have to restart it!)
		if (ulTimeJobLimit > 0) //If there is a time limit.
			{
				TimeNow = std::chrono::steady_clock::now();
				if (std::chrono::duration_cast<std::chrono::seconds>(TimeNow - TimeStart) >= (std::chrono::seconds)ulTimeJobLimit * 0.9)
				{
					switch (eSystemtype)
					{
					case SYSTEMTYPE::NORMAL:
						strScriptRestartCommand = strScriptPath + " &";
						system(strScriptRestartCommand.c_str());
						break;
					case SYSTEMTYPE::SLURM:
						strScriptRestartCommand = "sbatch " + strScriptPath;
						system(strScriptRestartCommand.c_str());
						break;
					default:
						printf_s("No System type selected, can not restart the script, you have to do this manually %s", EOL);

					}
					
					return JOB_TIMELIMIT_REACHED_MIAF;
				}
			}


		if (ulThreadsRunning == 0) //just wait for a thread to finish to continue
			return ERR_NOERROR;

		boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));

		iter = pvecJobsThreadInfo->begin();

	}

	return lReturn;
}

long WaitForFreeThread(std::vector<CJob*>  *pvecJobsThreadInfo,unsigned long * pulJobsDone)
{
	long lReturn = ERR_NOERROR;
	//Get the CPU Thread limit if not given, the default is 2 (1 for main thread 1 for the sub threads)

	static unsigned long ulCPUThreadLimit = 0;
	if (!ulCPUThreadLimit)
	{

		lReturn = glb.propertyBag.GetCPULimitation(&ulCPUThreadLimit);
		if (lReturn != ERR_NOERROR)
			return PROPERTYBAG_GETCPUTLIMIT_MIAF;
	}
	
	CJob* pThreadInfo = nullptr;
	std::vector<CJob*>::iterator iter = pvecJobsThreadInfo->begin();
	unsigned long ulThreadsRunning = 0;
	while (true)
	{
		ulThreadsRunning = 0;
		while (iter != pvecJobsThreadInfo->end())
		{
			pThreadInfo = (*iter);
			if (pThreadInfo->IsRunning())
				ulThreadsRunning++;
			else
			{
				//delete pThreadInfo; //Gets deleted later we just want it to be removed from the running job vector!!!
				pvecJobsThreadInfo->erase(iter);
				iter = pvecJobsThreadInfo->begin();
				ulThreadsRunning = 0;
				*pulJobsDone = *pulJobsDone + 1;
				continue;
			}
			iter++;
		}

		if (ulThreadsRunning < ulCPUThreadLimit) //just wait for a thread to finish to continue
			return ERR_NOERROR; //Thread is free


		boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));

		iter = pvecJobsThreadInfo->begin();
	}

	return lReturn;
}


/*
step three run MAFFT and IQTree for each file
    Main thread is the manager thread
        For an x number threads will be created (there is a limit how many threads we can have)
            In those threads:
                Mafft
                IQTree
                Copy mafft file to output folder
                Copy treefile to output folder
                Move original fasta file to temp folder (in case the program has to restart).
                Set the thread to finished
    Check after every "sleep 1 second" if the 90% time consumed is match.
        If yes stop all the threads start the script again.
*/

long RunMafftIQTreeJobs(std::vector<CJob*>* pvecJobs, unsigned long * pulJobsDone)
{
	long lReturn = ERR_NOERROR;

	std::chrono::time_point<std::chrono::steady_clock> TimeStart = std::chrono::steady_clock::now();

	//Get the max time limit of the job. If reaching 90% of the time restart the job if it's 0 there is no limit

	unsigned long ulJobTimeLimit = 0;
	lReturn = glb.propertyBag.GetJobTimeLimitation(&ulJobTimeLimit);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_GETJOBTIMELIMIT_MIAF;


	//=================== Set and Create Output Folders ==================================
	std::string strSystemCommand;
	
	std::string strInputPath;
	lReturn = glb.propertyBag.GetInputFolderPath(&strInputPath);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_GETINPUTFOLDER_MIAF;

	bool bPrequalJobs = false;
	lReturn = glb.propertyBag.GetBoolPrequal(&bPrequalJobs);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_BOOLPREQUAL_MIAF;

	bool bIQtree = false;
	lReturn = glb.propertyBag.GetBoolIQtree(&bIQtree);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_BOOL_PREQUAL_IQTREE_MIAF;

	bool bAPPPF = false;
	lReturn = glb.propertyBag.GetBoolAPPPF(&bAPPPF);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_BOOL_APPPF_MIAF;

	std::string strOutputPath;
	lReturn = glb.propertyBag.GetOutputFolderPath(&strOutputPath);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_GETOUTPUTFOLDER_MIAF;
	strSystemCommand = "mkdir -p " + strOutputPath;
	system(strSystemCommand.c_str());

	std::string strOutputFolderForMSAandTrees;
	strOutputFolderForMSAandTrees = strOutputPath + "MSA_and_Trees/";
	strSystemCommand = "mkdir -p " + strOutputFolderForMSAandTrees;
	if(bPrequalJobs == false) //Prequal doesn't have MSA_Trees
		system(strSystemCommand.c_str());

	std::string strOutputFolderForMSAandTreesCorrected;
	strOutputFolderForMSAandTreesCorrected = strOutputPath + "MSA_and_Corrected_Trees/";
	strSystemCommand = "mkdir -p " + strOutputFolderForMSAandTreesCorrected;
	if(bAPPPF == true) //Prequal doesn't have MSA_Trees
		system(strSystemCommand.c_str());

	std::string strOutputFolderPrequal;
	strOutputFolderPrequal = strOutputPath + "Prequal_Processed/";
	strSystemCommand = "mkdir -p " + strOutputFolderPrequal;
	if(bPrequalJobs == true) //Prequal needs a separate output folder,Fasta_Files_Processed not!
		system(strSystemCommand.c_str());

	std::string strOutputFolderPrequal_MSA_Trees;
	strOutputFolderPrequal_MSA_Trees = strOutputPath + "Prequal_MSA_and_Trees/";
	strSystemCommand = "mkdir -p " + strOutputFolderPrequal_MSA_Trees;
	if (bIQtree == true)
		system(strSystemCommand.c_str());

	std::string strMoveFastaFileWhenDoneFolder;
	strMoveFastaFileWhenDoneFolder = strOutputPath + "Fasta_Files_Processed/";
	strSystemCommand = "mkdir -p " + strMoveFastaFileWhenDoneFolder;
	system(strSystemCommand.c_str()); //Both Use this

	//=========================================================================================
	//Current Jobs Running Vector
	std::vector<CJob*> vecCurrentJobsRunning;





	size_t sizeJobsToRun = pvecJobs->size();

	CJob* pCurrentThread = nullptr;

	std::chrono::time_point<std::chrono::steady_clock> TimeNow;

	unsigned long ulJobsDone = 0;

	SYSTEMTYPE eSystemtype;
	lReturn = glb.propertyBag.GetSystemType(&eSystemtype);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_GETSYSTEMTYPE_MIAF;

	std::string strScriptPath;
	lReturn = glb.propertyBag.GetScriptPath(&strScriptPath);
	if (lReturn != ERR_NOERROR && lReturn != PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED)
		return PROPERTYBAG_GETSCRIPTPATH_MIAF;

	std::string strScriptRestartCommand;

	for (size_t idxJobObject = 0; idxJobObject < sizeJobsToRun; idxJobObject++)
	{
			lReturn = WaitForFreeThread(&vecCurrentJobsRunning, &ulJobsDone);
			if (lReturn != ERR_NOERROR)
			{
				*pulJobsDone = ulJobsDone;
				return lReturn;
			}

			if (ulJobTimeLimit > 0) //If there is a time limit.
			{
				TimeNow = std::chrono::steady_clock::now();
				if (std::chrono::duration_cast<std::chrono::seconds>(TimeNow - TimeStart) >= (std::chrono::seconds)ulJobTimeLimit * 0.9)
				{
					switch (eSystemtype)
					{
					case SYSTEMTYPE::NORMAL:
						strScriptRestartCommand = strScriptPath + " &";
						system(strScriptRestartCommand.c_str());
						break;
					case SYSTEMTYPE::SLURM:
						strScriptRestartCommand = "sbatch " + strScriptPath;
						system(strScriptRestartCommand.c_str());
						break;
					default:
						printf_s("No System type selected, can not restart the script, you have to do this manually %s", EOL);

					}
					
					return JOB_TIMELIMIT_REACHED_MIAF;
				}
			}

			pCurrentThread = pvecJobs->at(idxJobObject);

			if (idxJobObject % 5 == 0)
			{
				printf_s("Starting job %s: Job %zu out of %zu %s", pCurrentThread->GetFastaFileName(), idxJobObject+1, sizeJobsToRun, EOL);
			}

			pCurrentThread->SetThreadProc(new boost::thread(threadMafftIQTree, pCurrentThread));
			ASSERT(pCurrentThread->GetThreadProc());

			//Give is time to initialize
			boost::this_thread::sleep_for(boost::chrono::milliseconds(250));
			
			//And add the info to the vector of running threads
			vecCurrentJobsRunning.push_back(pCurrentThread);
		
	}

	printf_s("Now we wait till all processes are done %s", EOL);
	lReturn = WaitForAllThreads(&vecCurrentJobsRunning, &ulJobsDone, ulJobTimeLimit,TimeStart,eSystemtype,strScriptPath);
	if (lReturn != ERR_NOERROR)
	{
		*pulJobsDone = ulJobsDone;
		return lReturn;
	}

	//Now we need to to run APPPF if the user wants it
	std::string strAPPPPath;
	std::string strTaxonomicGroupPath;
	std::string strAPPPFCommand;
	if (bAPPPF == true)
	{
		lReturn = glb.propertyBag.GetAPPPFPath(&strAPPPPath);
		if (lReturn != ERR_NOERROR)
			return PROPERTYBAG_GETAPPPFPATH_MIAF;

		lReturn = glb.propertyBag.GetTaxonomicGroupFile(&strTaxonomicGroupPath);
		if (lReturn != ERR_NOERROR)
			return PROPERTYBAG_TAXONOMICGROUPFILE_MIAF;

		strAPPPFCommand = strAPPPPath + " -i " + strOutputFolderForMSAandTrees + " -t treefile -g " + strTaxonomicGroupPath + " -m " + strOutputFolderForMSAandTrees + " -r " + strOutputFolderForMSAandTreesCorrected;
		system(strAPPPFCommand.c_str());
	}


	*pulJobsDone = ulJobsDone;
	return lReturn;
}