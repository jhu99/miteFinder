#! /bin/bash

WORKFOLDER=/Users/jialu/Research/miteFinder
DATAFOLDER=/Users/jialu/Research/datasets/
$WORKFOLDER/miteFinderTest -input $DATAFOLDER/OSgenomeV6.man -pattern_scoring $WORKFOLDER/profile/pattern_scoring.txt -threshold 0.5 -output $DATAFOLDER/mite/OS_mite_candidates.fsa
