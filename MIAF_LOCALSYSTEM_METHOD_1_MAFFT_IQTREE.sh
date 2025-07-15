#!/bin/bash

#Program format can be any order after program path

#MIAF.out -i [Fasta_File_Folder] -r [OUTPUT_FOLDER] -m [PATH_TO_MAFFT] -q [PATH_TO_IQTREE] -c [NUMBER_OF_SIMULTEANOUSLY_JOBS] -x [THREADS_PER_JOB] -s [n] [-a [PATH_TO_APPPF]] [-f [PATH_TO_TAXONOMIC_GROUPFILE]]

#-i <Fasta File Directory>\t\t Set the Path to the directory containing your Fasta files: REQUIRED
#-r <OutputFolderPath>\t\t Set the Output Folder Path where your scripts are moved into: REQUIRED
#-m <Mafft Path>\t\t Path to Mafft. If installed you can also just use 'mafft': REQUIRED
#-q <IQTree Path> \t\t Path to IQTree. If Installed you can also use for e.g. iqtree: REQUIRED
#-a <APPPF_Path> \t\t path to Apply PhyloPyPruner Format.: NOT REQUIRED
#-f <Taxonomic_Group_File_Path> \t\t Path to the Taxonomic Group file: REQUIRED IF -a is set 
#-c <CPU Limit> \t\t How much threads can be run simulteanously: NOT REQUIRED
#-x <IQThread Limit> \t\t How much threads you want Mafft and IQTree to run on. Keep in mind that the Thread limit value x the CPU value can not exceed the total amount of threads, unless you want to slow down the machine tremendously
#-b <Path to Script>\t\t Path to the original script its' running it. Only needed if you have a time limit: NOT REQUIRED.
#-t <Time limit in seconds> \t\t Time limit of the current job.: NOT REQUIRED
#-s <System Type> \t\t  Either -s s (for Slurm) or -s n (for running it locally). Default is normally: NOT REQUIRED
#-p \t\t By setting this feature, prequal/qinsi/clipkit will be run (prequal and clipkit has to be installed on the system! Leaving this one out will run mafft/iqtree: NOT REQUIRED
#-pi \t\t same as -p but it also runs iqtree on the aligned file.: NOT REQUIRED

#Program Path
MIAF_PATH= #MIAF.out
#Folder to fasta files
FASTA_FILE_FOLDER=  #Change this

APPPF_PATH=~/APPPFormat_Static_Debian.out #https://github.com/mjbieren/ApplyPPPFormat
TAXONOMICGROUP_FILE= #https://github.com/mjbieren/ApplyPPPFormat see for examples
#Output Folder
OUTPUT= #Change this

#IQThree Path
IQTREE= #path to iqtree, if installed you can write down just iqtree or iqtree3 etc

#MAFFT
MAFFT= #path to mafft, if installed you can just use mafft

SYTEMTYPE=n #Don't touch for local, slurm is s (but currently not working yet)

#JOB Limit
JOB_LIMIT=5 #will run 5 jobs at the same time, increase if you can

#threads used in MAFFT and IQTree
THREAD_PER_JOB=2 #meaning 5x2 threads are running (aka 11, 1 main, 10 for the workers)


${MIAF_PATH} -i ${FASTA_FILE_FOLDER} -r ${OUTPUT} -m ${MAFFT} -q ${IQTREE} -c ${JOB_LIMIT} -x ${THREAD_PER_JOB} -s ${SYTEMTYPE} -a ${APPPF_PATH} -f ${TAXONOMICGROUP_FILE}
