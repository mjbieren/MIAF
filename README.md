# 🧬 Mafft and IQtree for All FastaFiles (MIAF)

Automate the alignment and phylogenetic analysis of multiple FASTA files using **MAFFT**, **IQ-TREE**, and optional tools like **PhyloPyPruner** and **ClipKIT**. This tool supports both **local execution** and **SLURM cluster environments**, and is highly configurable for parallel processing.

![License](https://img.shields.io/badge/license-MIT-blue)
![Platform](https://img.shields.io/badge/platform-Linux-lightgrey)

---

## 📦 Features

- Batch process FASTA files
- Integrate with MAFFT, IQ-TREE, and ClipKIT
- Optional phylogenetic pruning with PhyloPyPruner
- Supports SLURM and local systems
- Configurable thread and job limits

---

## 🔧 Command-Line Options

| Option | Description |
|--------|-------------|
| `-i <Fasta File Directory>` | **(REQUIRED)** Path to the directory containing your FASTA files. |
| `-r <Output Folder Path>` | **(REQUIRED)** Directory where all generated scripts and outputs will be stored. |
| `-m <MAFFT Path>` | **(REQUIRED)** Full path to MAFFT, or just `'mafft'` if it’s in your system’s PATH. |
| `-q <IQ-TREE Path>` | **(REQUIRED)** Full path to IQ-TREE, or just `'iqtree'` if it’s available globally. |
| `-a <APPPF Path>` | *(Optional)* Path to the **ApplyPhyloPyPrunerFormat** script. |
| `-f <Taxonomic Group File>` | **(REQUIRED if `-a` is used)** Path to a file specifying taxonomic groups for pruning. |
| `-c <CPU Limit>` | *(Optional)*
