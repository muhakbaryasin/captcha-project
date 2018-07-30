#include <iostream>
#include "preprocessing.h"
#include "preprocessing.cpp"

using namespace std;

int main( int argc, char** argv) {
	cv::Mat source = cv::imread(argv[1]);
	//cv::imshow("anu", source);
	Preprocessing *prep;
	prep->eroding(source);
}