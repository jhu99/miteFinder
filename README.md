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
Description
We have geneated binary code fro Unix-like, Mac, and Windows users (see in $miteFinder/bin)
Normally, it will works on machines with x86_64.

If it doesn't work, you need to compile the source code for your own.
################################################################################
Compile
To compile the source code, the latest compilers which supports the standard language C++11, also known as C++0x, is needed. Other older compiler may not support it.

Use the code as the following:
cd $miteFinder
make

Finished :)
################################################################################
Example
1. Download dataset

Our datasets are freely available at our website: https://code.google.com/p/netcoffee/downloads/list.
Download it into the folder of $NETCOFFEE and uncompress it with command:
tar -zxvf dataset.tar.gz
2. Compile source code
Compile the source code with command:
make MODE=Release
3. Run NetCoffee on our test dataset-2 with command:
./bin/netcoffee -alignment -task 1 -out -alpha ${ALPHA} -numspecies ${num} -numthreads ${thread num} -alignmentfile ./result/alignment_netcoffee.data -resultfolder ./result/
${ALPHA} is the parameter you want to specify for alpha.
Then you can find the all the involved output files in ./result/ .
There are many other functions which you can see with "-help" option.

################################################################################
END
THANKS FOR READING
If you have any questions regarding to the program, please don't hesitate to contact us through email.
