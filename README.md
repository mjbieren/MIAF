
# 💻 MIAF: MAFFT and IQ-TREE Automation for FASTA Files

MIAF automates the alignment and phylogenetic analysis of multiple FASTA files using **MAFFT** and **IQ-TREE**, or alternatively **prequal**, **MAFFT**, and **ClipKIT**. It is designed to streamline resource-intensive steps within the PhyloRSeq++ pipeline.

Currently, MIAF supports **local execution**, with **SLURM cluster support** under active development. The tool is functional but still being optimized and parameterized.

![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)  
![Platform](https://img.shields.io/badge/platform-Linux-lightgrey)

---

## 📦 Features

- Batch process FASTA files with two flexible methods  
- **Method 1**: Uses [MAFFT](https://mafft.cbrc.jp/alignment/software/) and [IQ-TREE](https://iqtree.github.io/) with optional output reformatting via [ApplyPPPFormat](https://github.com/mjbieren/ApplyPPPFormat)  
- **Method 2**: Uses [prequal](https://github.com/simonwhelan/prequal), MAFFT (QInSi), [ClipKIT](https://github.com/JLSteenwyk/ClipKIT), and optionally IQ-TREE  
- Runs on local systems (SLURM support coming soon)  
- Configurable CPU job and thread limits  
- Automatic self-restart when approaching job time limits  

---

## 🚀 Quickstart

Run Method 1 on local system:  
```
MIAF.out -i [FASTA_FOLDER] -r [OUTPUT_FOLDER] -m mafft -q iqtree -c [CONCURRENT_JOBS] -x [THREADS_PER_JOB] -s n
```

Run Method 2 with prequal and ClipKIT:  
```
MIAF.out -i [FASTA_FOLDER] -r [OUTPUT_FOLDER] -m mafft -c [CONCURRENT_JOBS] -x [THREADS_PER_JOB] -s n -p
```

See example scripts for detailed usage:  
- [Method 1 example](https://github.com/mjbieren/MIAF/blob/main/MIAF_LOCALSYSTEM_METHOD_1_MAFFT_IQTREE.sh)  
- [Method 2 example](https://github.com/mjbieren/MIAF/blob/main/MIAF_LOCALSYSTEM_METHOD_2_PREQUAL_MAFFT_CLIPKIT.sh)

---

## 🧬 Workflow Overview

![Pipeline](https://github.com/mjbieren/MIAF/blob/main/Sources/Images/miaf_flow.png?raw=True)

---

## 🧪 Methods Explained

### Method 1: MAFFT + IQ-TREE

For each FASTA file:  
```
mafft --thread [THREAD_LIMIT] --maxiterate 1000 --quiet --globalpair [FastaFile.fa] > [FastaFile.mafft]
iqtree -fast -nt [THREAD_LIMIT] -s [FastaFile.mafft] -st AA -m TEST -alrt 1000
cp [InputFolder]/[FastaFile.mafft] [OutputFolder/MSA_and_Trees]/[FastaFile.mafft]
cp [InputFolder]/[FastaFile.mafft.treefile] [OutputFolder/MSA_and_Trees]/[FastaFile.treefile]
mv [InputFolder]/[FastaFile.fa] [OutputFolder/Fasta_Files_Processed]/[FastaFile.fa]
```

*Optional tree reformatting:*  
```
ApplyPPPFormat.out -i [OutputFolder/MSA_and_Trees/] -t treefile -g [TAXONOMIC_GROUP_FILE] -m [OutputFolder/MSA_and_Trees/] -r [OutputFolder/MSA_and_Corrected_Trees/]
```

### Method 2: Prequal + MAFFT + ClipKIT (+ optional IQ-TREE)

For each FASTA file:  
```
prequal [FastaFile.fa]
mafft --thread [THREAD_LIMIT] --maxiterate 1000 --quiet --globalpair --allowshift --unalignlevel 0.8 [FastaFile.fa.filtered] > [FastaFile.fa.filtered.qinsi]
clipkit -m gappy -g 0.75 -log -o [FastaFile.fa.filtered.qinsi.g075] [FastaFile.fa.filtered.qinsi]
```

*Optional IQ-TREE on filtered alignments:*  
```
iqtree -fast -nt [THREAD_LIMIT] -s [FastaFile.fa.filtered.qinsi.g075] -st AA -m TEST -msub nuclear -alrt 1000
```

Copy and move files as appropriate to output folders (see example scripts).

---

## 🖥️ System Requirements & Execution

### Local Execution

- Configure CPU job and thread limits carefully to avoid resource overload.  
- MIAF uses a manager-worker model: e.g., `-c 10` runs 1 manager + 10 worker jobs concurrently.  
- Disk I/O can be a bottleneck; consider using `tmux` or `screen` for long runs.  
- IQ-TREE is resource-intensive; local desktops may struggle with large datasets.

### SLURM Execution

- Currently under development; planned for future releases.  
- Supports automatic job restart on time limits.  
- Runs on one compute node per job; multi-node support coming.

---

## 🔧 Command-Line Options

| Option | Description |
|--------|-------------|
| `-i <FASTA_DIR>` | **Required**: Directory with input FASTA files. |
| `-r <OUTPUT_DIR>` | **Required**: Directory for outputs. |
| `-m <MAFFT_PATH>` | **Required**: Path to MAFFT executable or `mafft` if in PATH. |
| `-q <IQTREE_PATH>` | **Required for Method 1; optional for Method 2**: IQ-TREE executable path or `iqtree`. |
| `-a <APPLYPPPF_PATH>` | Optional: Path to ApplyPPPFormat script. |
| `-f <TAXONOMIC_GROUP_FILE>` | Required if `-a` used: Taxonomic group file for pruning. |
| `-c <CONCURRENT_JOBS>` | Optional: Number of simultaneous jobs. Default: 1. |
| `-x <THREADS_PER_JOB>` | Optional: Threads per job for MAFFT/IQ-TREE. |
| `-b <SCRIPT_PATH>` | Optional: Needed if using a time limit. |
| `-t <TIME_LIMIT>` | Optional: Time limit (seconds) per job. |
| `-s <SYSTEM>` | Optional: `n` for local (default), `s` for SLURM. |
| `-p` | Required for Method 2: Run prequal + ClipKIT before alignment. |
| `-pi` | Optional for Method 2: Also run IQ-TREE on processed alignments. |

---

## ⚙️ Executables

Precompiled static executables available:  

- Debian 12: [MIAF_Static_Debian.out](https://github.com/mjbieren/MIAF/blob/main/Sources/Executables/MIAF_Static_Debian.out)  
- Rocky Linux 8: [MIAF_Static_HPC.out](https://github.com/mjbieren/MIAF/blob/main/Sources/Executables/MIAF_Static_HPC.out)  

Or compile from source.

---

## 🔗 References

- [MAFFT](https://mafft.cbrc.jp/alignment/software/) — Fast multiple sequence alignment  
- [IQ-TREE](https://iqtree.github.io/) — Phylogenetic inference via maximum likelihood  
- [ClipKIT](https://github.com/JLSteenwyk/ClipKIT) — Alignment trimming tool  
- [Prequal](https://github.com/simonwhelan/prequal) — Filters unreliable alignment regions  
- [ApplyPPPFormat](https://github.com/mjbieren/ApplyPPPFormat) — Reformats outputs for PhyloPyPruner pipelines
