#pragma once
#include <ctime>
#include <unistd.h>
#include <sys/time.h>
#include <vector> 
#include <list> 
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <algorithm> 
#include <tuple>
#include <omp.h>
#include <numeric> 
#include <math.h>
#include <map> 
#include <set>

using namespace std;

typedef std::tuple<int, int> idpair;
typedef std::tuple<int, int, int> idthree;

void recordtime();
void updatetime(int i);
void outputtime();
int edit_distance(const char *x, const int x_len, const  char *y, const int y_len, int k);


struct compare {
	bool operator()(const std::string& first, const std::string& second) {
		return first.size() < second.size();
	}
};



