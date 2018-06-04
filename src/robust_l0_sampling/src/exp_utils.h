// utils for exp
#ifndef __EXP_UTILS__
#define __EXP_UTILS__

#include "point.h"


#include <string>
#include <vector>

Point parse(const std::string& line);
std::vector<Point> read_data(std::string file_name);
std::pair<std::vector<Point>, double> read_data_with_alpha(std::string file_name);

#endif




