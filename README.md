# miteFinder
################################################################################

AUTHOR: JIALU HU
EMAIL: jhu@nwpu.edu.cn

Copyright (C) <2019>  <Jialu Hu>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

################################################################################

# Description

We have geneated binary code fro Unix-like, Mac, and Windows users (see in $miteFinder/bin)
Normally, it will works on machines with x86_64.

If it doesn't work, you need to compile the source code for your own.
################################################################################

# Compile

To compile the source code, the latest compilers which supports the standard language C++11, also known as C++0x, is needed. Other older compiler may not support it.

Use the code as the following:

cd $miteFinder

make

Then the binary code "miteFidner" will be generated and moved to $miteFidner/bin. 
Finished :)
################################################################################

# Example

./bin/miteFinder -input ${your_input_file} -output ${your_output_file} -pattern_scoring ./profile/pattern_scoring.txt -threshold 0.5

Warning: Your input file should be in sequences or genomes in fasta format.
################################################################################

# Result
The description of the MITE is just like this：
>mite|6|4131|4140|4194|4203|t3|4138|m1|ave_score:0.649614
GTTGCTCACCCCTGCTCTTGAGCCTTTGAAACATCTACACCAATTTTTTATTGTTTTCAT
CTATCCGTTTAAGTGGATTAAAATGATGTTTTTTAATTTTTTTTTATATTTTTTGGGCCG
AAAAAACGGACAGCATTGAAAAAAGCCAAGTTTTATTTAATTTAAGAAAAAATAGTCCAA
CCAAATGGTTTAA

The 6 means the serial number of chromosome and 4131,4140,4194,4203 is the position of TIR. t3 means the length of TSD is 3. m1 means the TIR is the imperfect inverted repeats and 4138 is the mismatch base. The ave_score:0.649614 is the score of MITE sequence(The more details can be seen in the below citation).
################################################################################

# Citation

Hu, Jialu and Zheng, Yan and Shang, Xuequn, "MiteFinderII: a novel tool to identify miniature inverted-repeat transposable elements hidden in eukaryotic genomes", BMC Medical Genomics, 2018, 11(5), 101.

################################################################################
# END
THANKS FOR READING

If you have any questions regarding to the program, please don't hesitate to contact us through email.
