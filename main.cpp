#include <iostream>
#include <numeric>
#include <vector>

int main() {

  std::vector<int> x(100);
  std::iota(std::begin(x), std::end(x), 0);

}
