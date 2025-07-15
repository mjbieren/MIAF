#include "GlobalHelper.h"

#define TOTALREQUIREMENTS 12

long ParseCommandLine(int argc, char* argv[], char* envp[])
{
	long lRet = ERR_NOERROR;
	bool bIQtree = false;

	int aiRequirements[TOTALREQUIREMENTS] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
	for (int i = 1; i < argc; i++)
	{

		printf_s("arg %i, %s %s", i, argv[i], EOL);



		if (*argv[i] == '-') //switch detected
		{
			switch (argv[i][1])
			{
			case 'i': //Input files dir path
				lRet = glb.propertyBag.SetInputFolderPath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_INPUTFOLDER_MIAF;
				aiRequirements[0] = 1;
				break;
			case 'r': //Output dir Path
				lRet = glb.propertyBag.SetOutputFolderPath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_OUTPUTFOLDER_MIAF;
				aiRequirements[1] = 1;
				break;
			case 'm': //MAFFT PATH
				lRet = glb.propertyBag.SetMAFFTPath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_MAFFTPATH_MIAF;
				aiRequirements[2] = 1;
				break;
			case 'q': //IQTree Path
				lRet = glb.propertyBag.SetIQTreePath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_IQTREEPATH_MIAF;
				aiRequirements[3] = 1;
				break; //(atol(&argv[i][3]));
			case 'c': //Nr of jobs limitation
				lRet = glb.propertyBag.SetCPULimitation(atol(&argv[i][3]));
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_CPULIMIT_MIAF;
				aiRequirements[4] = 1;
				break;
			case 'x': //IQthread limitation
				lRet = glb.propertyBag.SetIQThreadLimitation(atol(&argv[i][3]));
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_IQTHREADLIMIT_MIAF;
				aiRequirements[8] = 1;
				break;
			case 'b': //Script Path
				lRet = glb.propertyBag.SetScriptPath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_SCRIPTPATH_MIAF;
				aiRequirements[5] = 1;
				break;
			case 't': //Time limitation
				lRet = glb.propertyBag.SetJobTimeLimitation(atol(&argv[i][3]));
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_JOBTIMELIMIT_MIAF;
				aiRequirements[6] = 1;
				break;
			case 's': //System Type
				lRet = glb.propertyBag.SetSystemType(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_SYSTEMTYPE_MIAF;
				aiRequirements[7] = 1;
				break;
			case 'p': //Set program to run Prequal/qinsi/clipkit instead of mafft/iqtree
				lRet = glb.propertyBag.SetBoolPrequal();
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_BOOLPREQUAL_MIAF;
				aiRequirements[9] = 1;
				if (*&argv[i][2] == 'i') //we also have to run IQTree :)
				{
					lRet = glb.propertyBag.SetBoolIQTree();
					if (lRet != ERR_NOERROR)
						return PARSECOMMANDLINE_BOOLPREQUAL_IQTREE_MIAF;
				}
				break;
			case 'a': //Set path to the program Apply PhyloPyPruner Format
				lRet = glb.propertyBag.SetAPPPFPath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_APPPFPATH_MIAF;
				lRet = glb.propertyBag.SetBoolAPPPF();
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_BOOL_APPPF_MIAF;
				aiRequirements[10] = 1;
				break;
			case 'f': //Set the path to the taxonomic group file
				lRet = glb.propertyBag.SetTaxonomicGroupFile(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_TAXONOMICGROUPFILE_MIAF;
				aiRequirements[11] = 1;
				break;
			default:
			{
				printf_s("Invalid command line parameter no %i detected: %s %s", i, argv[i], EOL);
				return PARSE_COMMAND_LINE;
			}
			}

			/*

E-value

*/

		}
		else if (*argv[i] == '?')
		{
			//print arguments: ToDO change it
			printf_s(
				"%s"
				"-i <Fasta File Directory>\t\t Set the Path to the directory containing your Fasta files: REQUIRED %s"
				"-r <OutputFolderPath>\t\t Set the Output Folder Path where your scripts are moved into: REQUIRED %s"
				"-m <Mafft Path>\t\t Path to Mafft. If installed you can also just use 'mafft': REQUIRED %s"
				"-q <IQTree Path> \t\t Path to IQTree. If Installed you can also use for e.g. iqtree: REQUIRED %s"
				"-a <APPPF_Path> \t\t path to Apply PhyloPyPruner Format.: NOT REQUIRED %s"
				"-f <Taxonomic_Group_File_Path> \t\t Path to the Taxonomic Group file: REQUIRED IF -a is set %s"
				"-c <CPU Limit> \t\t How much threads can be run simulteanously: NOT REQUIRED %s"
				"-x <IQThread Limit> \t\t How much threads you want Mafft and IQTree to run on. Keep in mind that the Thread limit value x the CPU value can not exceed the total amount of threads, unless you want to slow down the machine tremendously %s"
				"-b <Path to Script>\t\t Path to the original script its' running it. Only needed if you have a time limit: NOT REQUIRED.%s"
				"-t <Time limit in seconds> \t\t Time limit of the current job.: NOT REQUIRED %s"
				"-s <System Type> \t\t  Either -s s (for Slurm) or -s n (for running it locally). Default is normally: NOT REQUIRED %s"
				"-p \t\t By setting this feature, prequal/qinsi/clipkit will be run (prequal and clipkit has to be installed on the system! Leaving this one out will run mafft/iqtree: NOT REQUIRED %s"
				"-pi \t\t same as -p but it also runs iqtree on the aligned file.: NOT REQUIRED %s"
				"%s"
				, EOL, EOL, EOL, EOL, EOL, EOL, EOL, EOL, EOL, EOL, EOL,EOL, EOL,EOL, EOL);
			return PARSE_COMMAND_LINE_MAN;
		}
		if (lRet != ERR_NOERROR)
			return lRet;
	}


	//First check if the taxonomic group file is set in the case APPPF is set.

	if (aiRequirements[10] == 1) //10 is APPP
	{
		if (aiRequirements[11] == 0) //11 is taxonomic group file
			return PARSECOMMANDLINE_TAXONOMICGROUPFILE_MIAF;
	}



	//Check if the other requiprements are met, if not sent a message to the user like if they would use ?. And sent back an error message.
	int iSum = 0;
	size_t idx = 0;

	if (aiRequirements[4] == 0) //CPU limit
	{
		lRet = glb.propertyBag.SetCPULimitation(2);
		if (lRet != ERR_NOERROR)
			return PARSECOMMANDLINE_CPULIMIT_MIAF;
		aiRequirements[4] = 1;
	}
	if (aiRequirements[5] == 0) //Script path xx
	{
		aiRequirements[5] = 1;
	}

	if (aiRequirements[6] == 0) //Time limit in seconds
	{
		aiRequirements[6] = 1;
	}
	if (aiRequirements[7] == 0) //System type
	{
		lRet = glb.propertyBag.SetSystemType("n");
		if (lRet != ERR_NOERROR)
			return PARSECOMMANDLINE_SYSTEMTYPE_MIAF;
		aiRequirements[7] = 1;
	}

	if (aiRequirements[8] == 0)
	{
		lRet = glb.propertyBag.SetIQThreadLimitation(1);
		if (lRet != ERR_NOERROR)
			return PARSECOMMANDLINE_IQTHREADLIMIT_MIAF;
		aiRequirements[8] = 1;
	}

	if (aiRequirements[9] == 1)
	{
		lRet = glb.propertyBag.GetBoolIQtree(&bIQtree);
		if (lRet != ERR_NOERROR)
			return PROPERTYBAG_BOOL_PREQUAL_IQTREE_MIAF;

		if(bIQtree == false)
			aiRequirements[3] = 1; //Since we won't be running iqtree
		else
		{
			if (aiRequirements[3] == 0) ///Set -pi but didn't set IQTree path
				return JOB_CANT_RUN_NOIQTREE_PATH;
		}

	}

	if (aiRequirements[9] == 0)
	{
		aiRequirements[9] = 1;
	}

	if (aiRequirements[10] == 0)
	{
		aiRequirements[10] = 1;
		aiRequirements[11] = 1; //Taxonomic group file is only set when Apppf is set. But both are not required to run the program.
	}

	while (idx < TOTALREQUIREMENTS)
	{
		iSum += aiRequirements[idx];
		idx++;
	}

	if (iSum == TOTALREQUIREMENTS)
		return lRet;
	else
	{
		//print arguments
		printf_s(
				"%s"
				"-i <Fasta File Directory>\t\t Set the Path to the directory containing your Fasta files: REQUIRED %s"
				"-r <OutputFolderPath>\t\t Set the Output Folder Path where your scripts are moved into: REQUIRED %s"
				"-m <Mafft Path>\t\t Path to Mafft. If installed you can also just use 'mafft': REQUIRED %s"
				"-q <IQTree Path> \t\t Path to IQTree. If Installed you can also use for e.g. iqtree: REQUIRED %s"
				"-a <APPPF_Path> \t\t path to Apply PhyloPyPruner Format.: NOT REQUIRED %s"
				"-f <Taxonomic_Group_File_Path> \t\t Path to the Taxonomic Group file: REQUIRED IF -a is set %s"
				"-c <CPU Limit> \t\t How much threads can be run simulteanously: NOT REQUIRED %s"
				"-x <IQThread Limit> \t\t How much threads you want Mafft and IQTree to run on. Keep in mind that the Thread limit value x the CPU value can not exceed the total amount of threads, unless you want to slow down the machine tremendously %s"
				"-b <Path to Script>\t\t Path to the original script its' running it. Only needed if you have a time limit: NOT REQUIRED.%s"
				"-t <Time limit in seconds> \t\t Time limit of the current job.: NOT REQUIRED %s"
				"-s <System Type> \t\t  Either -s s (for Slurm) or -s n (for running it locally). Default is normally: NOT REQUIRED %s"
				"-p \t\t By setting this feature, prequal/qinsi/clipkit will be run (prequal and clipkit has to be installed on the system! Leaving this one out will run mafft/iqtree: NOT REQUIRED %s"
				"-pi \t\t same as -p but it also runs iqtree on the aligned file.: NOT REQUIRED %s"
				"%s"
				, EOL, EOL, EOL, EOL, EOL, EOL, EOL, EOL, EOL, EOL, EOL,EOL, EOL,EOL, EOL);
		return PARSE_COMMAND_LINE_MAN;
	}

	return lRet;
}