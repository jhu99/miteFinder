#! /bin/bash

WORKFOLDER=/Users/jialu/Research/miteFinder
DATAFOLDER=/Users/jialu/Research/datasets/
#$WORKFOLDER/miteFinderTest -input $DATAFOLDER/OSgenomeV6.man -pattern_scoring $WORKFOLDER/profile/pattern_scoring.txt -threshold 0.5 -output $DATAFOLDER/mite/OS_mite_candidates.fsa
$WORKFOLDER/miteFinderTest -input $DATAFOLDER/ftp.ncbi.nlm.nih.gov/genomes/all/GCF/GCF_000001405.37_GRCh38.p11_genomic.fna -pattern_scoring $WORKFOLDER/profile/pattern_scoring.txt -threshold 0.5 -output $DATAFOLDER/mite/Human_mite_candidates.fsa
