# 🧬 Mafft and IQtree for All FastaFiles (MIAF)

Automate the alignment and phylogenetic analysis of multiple FASTA files using **MAFFT**, **IQ-TREE**, and optional tools like **PhyloPyPruner** and **ClipKIT**. This tool supports **local execution** and, in the near future **SLURM cluster environments**, and is highly configurable for parallel processing. This tool is working as is, but is still under development for optimization and parameterization.

![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)
![Platform](https://img.shields.io/badge/platform-Linux-lightgrey)

---

## 📦 Features

- Batch process FASTA files
- Method 1: Integrate with [MAFFT](https://mafft.cbrc.jp/alignment/server/index.html), [IQ-TREE](https://iqtree.github.io/) (optional to reformat the output treefiles with [ApplyPPPFormat](https://github.com/mjbieren/ApplyPPPFormat)
- Method 2: Integrate [prequal](https://github.com/simonwhelan/prequal), ([MAFFT](https://mafft.cbrc.jp/alignment/server/index.html) (qinsi), [ClipKIT](https://github.com/JLSteenwyk/ClipKIT), and potentialy [IQ-TREE](https://iqtree.github.io/) if the user wishes to.
- Running Method 2 requires preinstalled prequal and clipkit
- Supports local systems (SLURM currently WIP)
- Configurable thread and job limits
- Restarts itself when job time reaches 90%.

---

## Methods Explained
### Method 1
Runs for each fasta file
```
mafft --thread [THREAD_LIMIT] --maxiterate 1000 --quiet --globalpair [FastaFile.fa] > [FastaFile.mafft]
iqtree -fast -nt [THREAD_LIMIT] -s FastaFile.mafft -st AA -m TEST -alrt 1000
cp [InputFolder/][FastaFile.mafft] [OutputFolder/MSA_and_Trees/][FastaFile.mafft]
cp [InputFolder/][FastaFile.mafft.treefile] [OutputFolder/MSA_and_Trees/][FastaFile.treefile]
mv [InputFolder/][FastaFile.fa] [OutputFolder/Fasta_Files_Processed/][FastaFile.fa]
```
*optional for the whole set*
```
ApplyPPPFormat.out -i [OutputFolder/MSA_and_Trees/] -t treefile -g [TAXONOMIC_GROUP_FILE_PATH] -m [OutputFolder/MSA_and_Trees/] -r [OutputFolder/MSA_and_Corrected_Trees/]
```

### Method 2
Runs for each fasta file (between <> is optional)
```
prequal [FastaFile].fa
mafft --thread [THREAD_LIMIT] --maxiterate 1000 --quiet --globalpair --allowshift --unalignlevel 0.8 [FastaFile].fa.filtered > [FastaFile].fa.filtered.qinsi
<iqtree -fast -nt [TREADLIMIT] -s [FastaFile].fa.filtered.qinsi -st AA -m TEST -msub nuclear -alrt 1000>
clipkit -m gappy -g 0.75 -log -o [FastaFile].fa.filtered.qinsi.g075 " + [FastaFile].fa.filtered.qinsi
<iqtree -fast -nt [TREADLIMIT] -s [FastaFile].fa.filtered.qinsi.g075 -st AA -m TEST -msub nuclear -alrt 1000>
mv [InputFolder/][FastaFile].fa [OutputFolder/Fasta_Files_Processed/][FastaFile.fa]
cp [InputFolder/][FastaFile].fa.filtered.qinsi.g075 [OutputFolder/Prequal_Processed/][FastaFile].fa
<cp [InputFolder/][FastaFile].fa.filtered.qinsi.treefile [OutputFolder/Prequal_MSA_and_Trees/][FastaFile].qinsi.treefile>
<cp [InputFolder/][FastaFile].fa.filtered.qinsi [OutputFolder/Prequal_MSA_and_Trees/][FastaFile].qinsi.fa>
<cp [InputFolder/][FastaFile].fa.filtered.qinsi.g075.treefile [OutputFolder/Prequal_MSA_and_Trees/][FastaFile].qinsi_g075.treefile>
<cp [InputFolder/][FastaFile].fa.filtered.qinsi.g075 [OutputFolder/Prequal_MSA_and_Trees/][FastaFile].qinsi_g075.fa>
```


## Systems explained
### Local
When you run MIAF on a local system, you should be aware of the CPU/thread limitation. As especially IQTree can be very resource demanding I do not recommend to run this on a local machine that is generally used as a desktop or PC.
The programs runs things in a main thread (manager) and keeps track of it's workers (worker threads). When you set a job limitation of 10. You will run 11 processes simulteanously. (1 main thread and 10 worker threads). When one thread gets done, the next one will be started. Additionally you can asign how many threads per worker threads are available to increase the processing time of all the fasta files.
Still even with the most advanced local system, these steps can take quite some time as writing to disk (as a lot of different files are generated) can take quite some time as generally local systems are not set up to streamline the writing to disk (as write to disk is generally setup in a serial process and not a parallel process), unlike with HPC systems where generally this process is taken over in a different way to streamline the process.

General command to run MIAF locally (I recommend a session tool for this, like tmux):

Method 1
```
MIAF.out -i [Fasta_File_Folder] -r [OUTPUT_FOLDER] -m [PATH_TO_MAFFT] -q [PATH_TO_IQTREE] -c [NUMBER_OF_SIMULTEANOUSLY_JOBS] -x [THREADS_PER_JOB] -s [n] [-a [PATH_TO_APPPF]] [-f [PATH_TO_TAXONOMIC_GROUPFILE]]
```


Method 2
```
MIAF.out -i [Fasta_File_Folder] -m ${MAFFT} -r [OUTPUT_FOLDER] -c [NUMBER_OF_SIMULTEANOUSLY_JOBS] -x [THREADS_PER_JOB] -s [n] -p[i] [-q [PATH_TO_IQTREE]]
```

### SLURM
*WIP*


## 🔧 Command-Line Options

| Option | Description |
|--------|-------------|
| `-i <Fasta File Directory>` | **(REQUIRED)** Path to the directory containing your FASTA files. |
| `-r <Output Folder Path>` | **(REQUIRED)** Directory where all generated scripts and outputs will be stored. |
| `-m <MAFFT Path>` | **(REQUIRED)** Full path to MAFFT, or just `'mafft'` if it’s in your system’s PATH. |
| `-q <IQ-TREE Path>` | **(REQUIRED METHOD 1, *(Optional)* METHOD 2)** Full path to IQ-TREE, or just `'iqtree'` if it’s available globally. |
| `-a <APPPF Path>` | *(Optional)* Path to the **ApplyPhyloPyPrunerFormat** script. |
| `-f <Taxonomic Group File>` | **(REQUIRED if `-a` is used)** Path to a file specifying taxonomic groups for pruning. |
| `-c <CPU Limit>` | *(Optional)* Number of concurrent jobs (threads) to run. Controls parallelism. Default is 1 job |
| `-x <IQThread Limit>` | *(Optional)* Number of threads for **MAFFT** and **IQ-TREE**. Ensure that `CPU × Thread Limit` does **not** exceed the total available threads. |
| `-b <Path to Script>` | *(Optional)* Full path to the original script. Needed **only** if you use a time limit (`-t`). |
| `-t <Time Limit (seconds)>` | *(Optional)* Time limit (in seconds) for the current job. |
| `-s <System Type>` | *(Optional)* Specify `-s s` for **SLURM** systems or `-s n` for **local** execution. Default is `n`. |
| `-p` | **(REQUIRED METHOD 2 Run **Prequal**, **QInSi**, and **ClipKIT** before alignment. These tools must be installed. If omitted, the default run will include **MAFFT** and **IQ-TREE** only. |
| `-pi` | *(Optional)* Same as `-p`, but also runs **IQ-TREE** on the aligned output. |
