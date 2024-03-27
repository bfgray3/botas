#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

auto resample(const auto& x) {
  // adapted from https://stackoverflow.com/questions/42926209/equivalent-function-to-numpy-random-choice-in-c
  std::default_random_engine generator;  // TODO: don't make a new one on every resample
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

/*
psuedocode
for i=1...500    <---- eventually split this into pieces and do on different cores
  use function above to resample
  calculate statistic on this replicate
  add statistic to vector
done

look at distribution of the above
*/

}
