#include "GlobalHelper.h"
CGlobal glb;

//Program functions

long ParseCommandLine(int argc, char* argv[], char* envp[]);
long ParseInputFiles(std::vector<CJob*>* pvecJobsBase);
long RunMafftIQTreeJobs(std::vector<CJob*>* pvecJobs, unsigned long* pulJobsDone);


/*
This tool reads in all the orthogroup files (Fasta files) and do a Mafft + IQtree combi for each one of them afterwards it copies the alignment file and the tree file in the output folder.

Step one parse command line
       Input dir containing fasta files
       Output directory
       IQTree Input Path
       Mafft input path
       Max Thread count
       Max time in seconds
       Type system n/s
Step two read in the amount of fasta files
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

int main(int argc, char* argv[], char* envp[])
{
    long lReturn = ERR_NOERROR;

	//STEP ONE
    printf_s("Parsing the command line %s", EOL);
    lReturn = ParseCommandLine(argc, argv, envp);
    CHECK_RETURNVALUE(lReturn);
    printf_s("Done with parsing the command line %s", EOL);

    std::vector<CJob*> vecJobsBase;
    
    //STEP TWO
    printf_s("Parsing the fasta file names from the input directory %s", EOL);
    lReturn = ParseInputFiles(&vecJobsBase);
    if (lReturn != ERR_NOERROR)
    {
        ClearVector(&vecJobsBase);
    }
	CHECK_RETURNVALUE(lReturn);
    printf_s("Done with getting all the fasta file locations %s", EOL);

    unsigned long ulJobsDone = 0;

    //STEP THREE
    printf_s("Start the different MAFFT/IQTree Jobs on the system %s", EOL);
    lReturn = RunMafftIQTreeJobs(&vecJobsBase, &ulJobsDone);
    if (lReturn == JOB_TIMELIMIT_REACHED_MIAF)
    {
		ClearVector(&vecJobsBase);
        printf_s("Time limit of Job reached restarting the program/resubmitting the script to the SLURM System %s", EOL);
    }
    else if (lReturn != ERR_NOERROR)
        ClearVector(&vecJobsBase);
    CHECK_RETURNVALUE(lReturn);
    printf_s("Done with the Mafft/IQTree jobs %s", EOL);
	
    printf_s("A total %zu jobs had to be run, we finished %lu jobs, we still have to do %zu jobs %s", vecJobsBase.size(), ulJobsDone, vecJobsBase.size()-((size_t)ulJobsDone), EOL);
	ClearVector(&vecJobsBase);
    return 0;
}