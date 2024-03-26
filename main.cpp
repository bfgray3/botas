#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

auto resample(const auto& x) {
  // adapted from https://stackoverflow.com/questions/33802205/how-to-sample-without-replacement-using-c-uniform-int-distribution
  std::default_random_engine generator;
  std::uniform_int_distribution<> distribution(0, x.size() - 1);

  std::vector<int> replicate(x.size()); // TODO: get type from x
  std::generate_n(
    std::begin(replicate),
    replicate.size(),
    // TODO: make actually random
    [&x, &distribution, generator = std::default_random_engine{}]() mutable {
      return x[distribution(generator)];
    }
  );
  return replicate;
}

int main() {
  std::vector<int> x(10);
  std::iota(std::begin(x), std::end(x), 0);

  for (const auto& e: resample(x)) std::cout << e << '\n';
}
