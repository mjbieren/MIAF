# Mafft and IQtree for All FastaFiles (MIAF)
This repository hosts the updated version of Mafft and IQtree for all FASTA files (MIAF). It is developed as a job manager for the PhyloRSeq++ pipeline, where heavy computational and parallel jobs are necessary.
It can currently run two different jobs on a single heavy duty machine or on a HPC (SLURM):
1) Run Mafft and IQtree on all Fasta Files in a folder
2) Run prequal, mafft and clipkit on all Fasta files (with the option to also run iqtree on the differently aligned fasta files)


