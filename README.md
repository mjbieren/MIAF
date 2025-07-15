# 💻 Mafft and IQtree for All FastaFiles (MIAF)
This repository hosts the MIAF tool, which automates the alignment and phylogenetic analysis of multiple FASTA files using **MAFFT** and **IQ-TREE**, or **prequal**, **MAFFT**, and **ClipKit**. And was created to smoothen the heavily resource demanding steps within the PhyloRSeq++ pipeline. 
This tool supports **local execution**, and in the near future, **SLURM cluster environments**. It is currently functional but under active development for optimization and parameter tuning.


![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)
![Platform](https://img.shields.io/badge/platform-Linux-lightgrey)

---

## 📦 Features

- Batch process FASTA files using two flexible methods
- **Method 1**: Integrates [MAFFT](https://mafft.cbrc.jp/alignment/software/) and [IQ-TREE](https://iqtree.github.io/), with optional output reformatting via [ApplyPPPFormat](https://github.com/mjbieren/ApplyPPPFormat)
- **Method 2**: Integrates [prequal](https://github.com/simonwhelan/prequal), MAFFT (QInSi), [ClipKIT](https://github.com/JLSteenwyk/ClipKIT), and optionally IQ-TREE
- Local system support (SLURM is a work in progress)
- Configurable CPU job and thread limits
- Automatic self-restart when job time reaches 90%

---

## 🧬 Workflow Overview

![Pipeline_Diagram](https://github.com/mjbieren/MIAF/blob/main/Sources/Images/miaf_flow.png?raw=True_"Pipeline")

## 🧪 Methods Explained

### Method 1

For each FASTA file:
```
mafft --thread [THREAD_LIMIT] --maxiterate 1000 --quiet --globalpair [FastaFile.fa] > [FastaFile.mafft]
iqtree -fast -nt [THREAD_LIMIT] -s [FastaFile.mafft] -st AA -m TEST -alrt 1000
cp [InputFolder]/[FastaFile.mafft] [OutputFolder/MSA_and_Trees]/[FastaFile.mafft]
cp [InputFolder]/[FastaFile.mafft.treefile] [OutputFolder/MSA_and_Trees]/[FastaFile.treefile]
mv [InputFolder]/[FastaFile.fa] [OutputFolder/Fasta_Files_Processed]/[FastaFile.fa]
```
*Optionally reformat output trees:*
```
ApplyPPPFormat.out -i [OutputFolder/MSA_and_Trees/] -t treefile -g [TAXONOMIC_GROUP_FILE_PATH] -m [OutputFolder/MSA_and_Trees/] -r [OutputFolder/MSA_and_Corrected_Trees/]
```

See [MIAF_LOCALSYSTEM_METHOD_1_MAFFT_IQTREE.sh](https://github.com/mjbieren/MIAF/blob/main/MIAF_LOCALSYSTEM_METHOD_1_MAFFT_IQTREE.sh) for an example and quickstart.

### Method 2
For each FASTA file (items in <> are optional):
```
prequal [FastaFile].fa
mafft --thread [THREAD_LIMIT] --maxiterate 1000 --quiet --globalpair --allowshift --unalignlevel 0.8 [FastaFile].fa.filtered > [FastaFile].fa.filtered.qinsi
<iqtree -fast -nt [THREAD_LIMIT] -s [FastaFile].fa.filtered.qinsi -st AA -m TEST -msub nuclear -alrt 1000>
clipkit -m gappy -g 0.75 -log -o [FastaFile].fa.filtered.qinsi.g075 [FastaFile].fa.filtered.qinsi
<iqtree -fast -nt [THREAD_LIMIT] -s [FastaFile].fa.filtered.qinsi.g075 -st AA -m TEST -msub nuclear -alrt 1000>
mv [InputFolder]/[FastaFile].fa [OutputFolder/Fasta_Files_Processed]/[FastaFile.fa]
cp [InputFolder]/[FastaFile].fa.filtered.qinsi.g075 [OutputFolder/Prequal_Processed]/[FastaFile].fa
<cp [InputFolder]/[FastaFile].fa.filtered.qinsi.treefile [OutputFolder/Prequal_MSA_and_Trees]/[FastaFile].qinsi.treefile>
<cp [InputFolder]/[FastaFile].fa.filtered.qinsi [OutputFolder/Prequal_MSA_and_Trees]/[FastaFile].qinsi.fa>
<cp [InputFolder]/[FastaFile].fa.filtered.qinsi.g075.treefile [OutputFolder/Prequal_MSA_and_Trees]/[FastaFile].qinsi_g075.treefile>
<cp [InputFolder]/[FastaFile].fa.filtered.qinsi.g075 [OutputFolder/Prequal_MSA_and_Trees]/[FastaFile].qinsi_g075.fa>
```

See [MIAF_LOCALSYSTEM_METHOD_2_PREQUAL_MAFFT_CLIPKIT.sh](https://github.com/mjbieren/MIAF/blob/main/MIAF_LOCALSYSTEM_METHOD_2_PREQUAL_MAFFT_CLIPKIT.sh) for an example and quickstart.

## 🖥️ Systems Explained
### Local Execution
When running MIAF locally, be mindful of CPU/thread limits. IQ-TREE is especially resource-intensive. The program uses one main manager thread and spawns several worker threads (based on the `-c` parameter). For example, setting `-c 10` means 1 main thread and up to 10 simultaneous worker threads.

Also not:
- Local disk I/O can become a bottleneck.
- Prefer use tools like `tmux` or `screen` to manage long-running sessions.

When running MIAF on a local system, be mindful of CPU and thread limitations. **IQ-TREE**, in particular, is resource-intensive, so it's not recommended to run this pipeline on machines primarily used as personal desktops or workstations.
MIAF uses a **manager-worker model**, where the main thread (manager) tracks multiple worker threads. For example, setting a job limit of `10` will result in **11 concurrent processes** (1 manager + 10 workers). Each worker processes a FASTA file, and new jobs begin as others complete. You can also assign the number of threads each worker may use to optimize runtime across multiple files.
Even on high-end local systems, this process may take considerable time. One major bottleneck is **disk I/O**: since many intermediate files are written to disk, performance is often limited by serial disk access. In contrast, HPC environments typically manage I/O more efficiently through parallelized or distributed file systems, reducing this overhead.

Method 1
```
MIAF.out -i [Fasta_File_Folder] -r [OUTPUT_FOLDER] -m [PATH_TO_MAFFT] -q [PATH_TO_IQTREE] -c [NUMBER_OF_SIMULTEANOUSLY_JOBS] -x [THREADS_PER_JOB] -s [n] [-a [PATH_TO_APPPF]] [-f [PATH_TO_TAXONOMIC_GROUPFILE]]
```


Method 2
```
MIAF.out -i [Fasta_File_Folder] -m ${MAFFT} -r [OUTPUT_FOLDER] -c [NUMBER_OF_SIMULTEANOUSLY_JOBS] -x [THREADS_PER_JOB] -s [n] -p[i] [-q [PATH_TO_IQTREE]]
```

### SLURM Execution
⚠️ **Work in progress** – Not yet supported.
MIAF is compatible with SLURM-based high-performance computing (HPC) systems and can automatically restart itself if the job limitation is reached. Currently, it runs on a single compute node at a time. Support for running across multiple nodes in parallel is not yet available, but it is planned for future versions.


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

## Executables

This tool was created using the Boost library (version 1.88). It is developed in Visual Studio 2019 with the GCC compiler (For remote Linux). I've compiled two different (static) executables (they are portable!) under:

- **Debian 12:**  
  [MIAF_Static_Debian.out](https://github.com/mjbieren/MIAF/blob/main/Sources/Executables/MIAF_Static_Debian.out)

- **Rocky Linux 8 (Red Hat-based):**  
  [MIAF_STATIC_HPC.out](https://github.com/mjbieren/MIAF/blob/main/Sources/Executables/MIAF_Static_HPC.out)


Or compile your own from source.

---

## 🔗 References

Below are the main tools and resources that have to be pre-installed for MIAF to be able to work:

- **[MAFFT](https://mafft.cbrc.jp/alignment/software/)**  
  A fast and accurate multiple sequence alignment program for amino acid or nucleotide sequences.

- **[IQ-TREE](https://iqtree.github.io/)**  
  Efficient phylogenetic inference software using maximum likelihood with advanced model selection.

- **[ClipKIT](https://github.com/JLSteenwyk/ClipKIT)**  
  A tool for intelligently trimming multiple sequence alignments for accurate phylogenomic inference.

- **[Prequal](https://github.com/simonwhelan/prequal)**  
  Filters unreliable regions in multiple sequence alignments before downstream analysis.

- **[ApplyPPPFormat](https://github.com/mjbieren/ApplyPPPFormat)**  
  Reformats tree and alignment outputs for compatibility with PhyloPyPruner pipelines.

