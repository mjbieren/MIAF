# 🧬 Mafft and IQtree for All FastaFiles (MIAF)

Automate the alignment and phylogenetic analysis of multiple FASTA files using **MAFFT**, **IQ-TREE**, and optional tools like **PhyloPyPruner** and **ClipKIT**. This tool supports both **local execution** and **SLURM cluster environments**, and is highly configurable for parallel processing.

![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)
![Platform](https://img.shields.io/badge/platform-Linux-lightgrey)

---

## 📦 Features

- Batch process FASTA files
- Method 1: Integrate with [MAFFT](https://mafft.cbrc.jp/alignment/server/index.html), [IQ-TREE](https://iqtree.github.io/) (optional to reformat the output treefiles with [ApplyPPPFormat](https://github.com/mjbieren/ApplyPPPFormat)
- Method 2: Integrate [prequal](https://github.com/simonwhelan/prequal), ([MAFFT](https://mafft.cbrc.jp/alignment/server/index.html) (qinsi), [ClipKIT](https://github.com/JLSteenwyk/ClipKIT), and potentialy [IQ-TREE](https://iqtree.github.io/) if the user wishes to.
- Supports SLURM and local systems
- Configurable thread and job limits
- Restarts itself when job time reaches 90%.

---

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
