#pragma once

#include "stdafx.h"
#include "Management.h"

enum SYSTEMTYPE
{
	SLURM,
	NORMAL,
};

class CMIAFPropertyBag : CPropertyBag
{
protected:
	char m_szGetMafftIQTreeAllFastaErrMessage[255];
public:
	CMIAFPropertyBag() {};
	~CMIAFPropertyBag() {};
	const char* GetLastMIAFErrorMessage() { return m_szGetMafftIQTreeAllFastaErrMessage; }

	/*
	Script Input File
	Script Output File
	Running Script Path
	MAFFT Path
	IQTree Path
	MAX Thread
	MAX Time
	Type

	*/

	long SetInputFolderPath(const char* szInputPath)
	{
		std::string strInputPath = szInputPath;
		AppendSlash(&strInputPath);
		return SetValueString("Paths", "Input", strInputPath.c_str());
	}

	long GetInputFolderPath(std::string* pstrInputPath)
	{
		return GetValueString("Paths", "Input", pstrInputPath);
	}

	long SetOutputFolderPath(const char* szOutputPath)
	{
		std::string strOutputPath = szOutputPath;
		AppendSlash(&strOutputPath);
		return SetValueString("Paths", "Output", strOutputPath.c_str());
	}

	long GetOutputFolderPath(std::string* pstrOutputPath)
	{
		return GetValueString("Paths", "Output", pstrOutputPath);
	}

	long SetMAFFTPath(const char* szMAFFTPath)
	{
		return SetValueString("Paths", "MAFFT", szMAFFTPath);
	}

	long GetMAFFTPath(std::string* pstrMAFFTPath)
	{
		return GetValueString("Paths", "MAFFT", pstrMAFFTPath);
	}

	long SetIQTreePath(const char* szIQTreePath)
	{
		return SetValueString("Paths", "IQTree", szIQTreePath);
	}

	long GetIQTreePath(std::string* pstrIQTreePath)
	{
		return GetValueString("Paths", "IQTree", pstrIQTreePath);
	}

	long SetScriptPath(const char* szScriptPath)
	{
		return SetValueString("Paths", "Script", szScriptPath);
	}

	long GetScriptPath(std::string* pstrScriptPath)
	{
		long lReturn = GetValueString("Paths", "Script", pstrScriptPath, "NO_SCRIPT");
		if (lReturn == PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED)
			return ERR_NOERROR;
		return ERR_NOERROR;
	}

	long SetTaxonomicGroupFile(const char* szTaxonomicGroupFile)
	{
		return SetValueString("Paths", "TaxonomicGroupFile", szTaxonomicGroupFile);
	}

	long GetTaxonomicGroupFile(std::string* pstrTaxonomicGroupFile)
	{
		return GetValueString("Paths", "TaxonomicGroupFile", pstrTaxonomicGroupFile);
	}


	long SetAPPPFPath(const char* szAPPPFPath)
	{
		return SetValueString("Paths", "APPPF", szAPPPFPath);
	}

	long GetAPPPFPath(std::string* pstrAPPPFPath)
	{
		return GetValueString("Paths", "APPPF", pstrAPPPFPath);
	}

	long SetBoolAPPPF(bool bSetAPPPF = true)
	{
		return SetValueBool("Bool", "APPPF", bSetAPPPF);
	}

	long GetBoolAPPPF(bool* pbGetAPPPF)
	{
		long lReturn = ERR_NOERROR;
		lReturn = GetValueBool("Bool", "APPPF", pbGetAPPPF, false);
		if (lReturn == PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED || lReturn == PROP_ERR_KEY_NOT_FOUND_DEFAULT_USED)
		{
			printf_s("%s %s", "pbGetAPPPF Bool Not Found: Default (false) used", EOL);
			return ERR_NOERROR;
		}
		return lReturn;
	}


	long SetSystemType(const char * szSystemType)
	{
		return SetValueString("System", "Type", szSystemType);
	}

	long GetSystemType(enum SYSTEMTYPE * peSYSTEMTYPE)
	{
		long lReturn = ERR_NOERROR;
		std::string strSystemType;
		lReturn = GetValueString("System", "Type", &strSystemType);

		if (lReturn != ERR_NOERROR)
			strSystemType = "x";

		if (lReturn != ERR_NOERROR)
		{
			switch (strSystemType.at(0))
			{
			case 's':
				*peSYSTEMTYPE = SYSTEMTYPE::SLURM;
				break;
			case 'n':
				*peSYSTEMTYPE = SYSTEMTYPE::NORMAL;
				break;
			default:
				printf_s("No running type found, pick s for slurn and n for running it on the current system, currently the default is used, which is the normal");
				*peSYSTEMTYPE = SYSTEMTYPE::NORMAL;
			}
		}
		

		return lReturn;

	}

	long SetCPULimitation(long lCPULimit)
	{

		return SetValueLong("CPU", "Limit", lCPULimit);
	}

	long GetCPULimitation(unsigned long* pulCPULimit)
	{
		long lReturn = ERR_NOERROR;
		long lCPULimit = 0;

		lReturn = GetValueLong("CPU", "Limit", &lCPULimit, 2);
		if (lReturn == PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED || lReturn == PROP_ERR_KEY_NOT_FOUND_DEFAULT_USED)
		{
			printf_s("%s %s", "CPU Limit not Not Found: Default of 2 is used", EOL);
			return ERR_NOERROR;
		}
		if (lReturn != ERR_NOERROR)
			return lReturn;

		*pulCPULimit = (unsigned long)lCPULimit;

		return ERR_NOERROR;

	}

	long SetIQThreadLimitation(long lIQthreadLimit)
	{
		return SetValueLong("CPU", "IQTree", lIQthreadLimit);
	}


	long GetIQThreadLimitation(unsigned long* pulIQThreadLimit)
	{
		long lReturn = ERR_NOERROR;
		long lIQThread = 0;


		lReturn = GetValueLong("CPU", "IQTree", &lIQThread, 1);
		if (lReturn == PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED || lReturn == PROP_ERR_KEY_NOT_FOUND_DEFAULT_USED)
		{
			printf_s("%s %s", "IQThread Limit not Not Found: Default of 1 is used", EOL);
			return ERR_NOERROR;
		}
		if (lReturn != ERR_NOERROR)
			return lReturn;

		*pulIQThreadLimit = (unsigned long)lIQThread;

		return lReturn;

	}


	long SetJobTimeLimitation(long lTimeLimit)
	{

		return SetValueLong("Time", "Limit", lTimeLimit);
	}

	long GetJobTimeLimitation(unsigned long* pulTimeLimit)
	{
		long lReturn = ERR_NOERROR;
		long lTimeLimit = 0;

		lReturn = GetValueLong("Time", "Limit", &lTimeLimit, 0);
		if (lReturn == PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED || lReturn == PROP_ERR_KEY_NOT_FOUND_DEFAULT_USED)
		{
			printf_s("%s %s", "Time Limit not Not Found: Default used", EOL);
			return ERR_NOERROR;
		}
		if (lReturn != ERR_NOERROR)
			return lReturn;

		*pulTimeLimit = (unsigned long)lTimeLimit;

		return ERR_NOERROR;

	}

	long SetBoolPrequal(bool bSetPrequal = true)
	{
		return SetValueBool("Bool", "Prequal", bSetPrequal);
	}

	long GetBoolPrequal(bool* pbSetPrequal)
	{
		long lReturn = ERR_NOERROR;
		lReturn = GetValueBool("Bool", "Prequal", pbSetPrequal, false);
		if (lReturn == PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED || lReturn == PROP_ERR_KEY_NOT_FOUND_DEFAULT_USED)
		{
			printf_s("%s %s", "bSetPrequal Bool Not Found: Default (false) used", EOL);
			return ERR_NOERROR;
		}
		return lReturn;
	}

	long SetBoolIQTree(bool bSetIQtree = true)
	{
		return SetValueBool("Bool", "IQTree", bSetIQtree);
	}

	long GetBoolIQtree(bool* pbGetIQtree)
	{
		long lReturn = ERR_NOERROR;
		lReturn = GetValueBool("Bool", "IQTree", pbGetIQtree, false);
		if (lReturn == PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED || lReturn == PROP_ERR_KEY_NOT_FOUND_DEFAULT_USED)
		{
			printf_s("%s %s", "pbGetIQtree Bool Not Found: Default (false) used", EOL);
			return ERR_NOERROR;
		}
		return lReturn;
	}

};