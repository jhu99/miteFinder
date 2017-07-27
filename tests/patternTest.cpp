//
//  patternTest.cpp
//  miteFinder
//
//  Created by Jialu Hu on 2017/5/25.
//  Copyright © 2017年 Jialu Hu. All rights reserved.
//

#include <stdio.h>
#include <fstream>
#include "pattern_learning.h"

int main(int argc, const char * argv[]) {
	std::string filename="/Users/jialu/Research/datasets/OSgenomeV6.man";
	std::fstream output("/Users/jialu/Research/datasets/kmer_count.txt",std::ios::out);
	patternLearning("/Users/jialu/Research/datasets/matchresult.fsa", "/Users/jialu/Research/datasets/mismatchresult.fsa",output);
	return 0;
}
