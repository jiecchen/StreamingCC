#include "../src/point.h"

#include <iostream>

using namespace std;

int main() {
  Point p{1, 2, 3, 4.4};
  Point pp = 10 * p;
  cout << to_str(p) << endl;
  cout << to_str(p + 0.5) << endl;
  cout << to_str(p * 10) << endl;
  cout << to_str(pp - p) << endl;
  return 0;
}
