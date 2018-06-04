#include "../algorithms/data.h"

int main() {
  for (const auto& a : data::gauss(10, 5, 0.0001, 1)) {
    a.print(" ");
  }
  return 0;
}
