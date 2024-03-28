#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

constexpr std::size_t N{500};
constexpr std::size_t NUM_REPLICATES{100'000};

auto var(const auto& x) {
  const auto n{static_cast<double>(x.size())};
  const auto x_bar{std::reduce(std::cbegin(x), std::cend(x), 0.0) / n};
  return std::transform_reduce(
    std::cbegin(x),
    std::cend(x),
    0.0,
    std::plus<>(),
    [x_bar](const auto xi) { return std::pow(xi - x_bar, 2); }
  ) / (n - 1);
}

template <typename T>
auto resample(const T& x, auto& dev) {
  // adapted from https://stackoverflow.com/questions/42926209/equivalent-function-to-numpy-random-choice-in-c
  std::uniform_int_distribution<std::size_t> distribution(0, x.size() - 1);
  std::vector<typename T::value_type> replicate(x.size());

  std::generate_n(
    std::begin(replicate),
    replicate.size(),
    [&x, &distribution, generator = std::default_random_engine{dev()}]() mutable {
      return x[distribution(generator)];
    }
  );

  return replicate;
}

int main() {
  std::vector<int> x(N);
  std::iota(std::begin(x), std::end(x), 0);

  std::vector<double> results(NUM_REPLICATES);
  std::random_device random_device;

  for (std::size_t i{}; i < results.size(); ++i) {  // TODO: split this into pieces, do on different cores
    const auto current_sample{resample(x, random_device)};
    results[i] = var(current_sample);
  }
  std::cout << var(results) << '\n';
}
