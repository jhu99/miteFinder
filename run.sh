#! /bin/bash

input_filename = $1
output_filename = $2
./miteFinder -input $1 -output $2 -pattern_scoring ./profile/pattern_scoring.txt -threshold 0.5
