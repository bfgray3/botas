#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

auto var(const auto& x) {
  const auto x_bar{std::reduce(std::cbegin(x), std::cend(x), 0.0) / x.size()};
  return std::transform_reduce(
    std::cbegin(x),
    std::cend(x),
    0.0,
    std::plus<>(),
    [x_bar](const auto xi) { return std::pow(xi - x_bar, 2); }
  ) / (x.size() - 1);
}

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

  std::vector<double> results(5);

  for (std::size_t i{}; i < results.size(); ++i) {  // TODO: split this into pieces, do on different cores
    const auto current_sample{resample(x)};
    for (const auto& e: current_sample) std::cout << e << '\n';
    results[i] = var(current_sample);
    std::cout << '\n';
  }
  std::cout << "variance: " << var(results) << '\n';
}
