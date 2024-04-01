#include <algorithm>
#include <cmath>
#include <functional>
#include <future>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

constexpr std::size_t N{500};
constexpr std::size_t NUM_REPLICATES{100'000};
constexpr std::size_t HALF{NUM_REPLICATES / 2};  // FIXME: more robust

auto var(const auto& x) {  // TODO: more careful about parameter type
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

// TODO: more general types
void resample(const std::vector<double>& x, const std::vector<double>::iterator start, const std::size_t num_replicates) {
  std::uniform_int_distribution<std::size_t> distribution(0, x.size() - 1);
  std::random_device random_device;
  std::vector<double> replicate(x.size());
  auto current_position{start};

  for (; current_position < start + num_replicates; ++current_position) {
    // adapted from https://stackoverflow.com/questions/42926209/equivalent-function-to-numpy-random-choice-in-c
    std::generate_n(
      std::begin(replicate),
      replicate.size(),
      [&x = std::as_const(x), &distribution, generator = std::default_random_engine{random_device()}]() mutable {
        return x[distribution(generator)];
      }
    );
    *current_position = var(replicate);
  }
}

int main() {
  std::vector<double> x(N), results(NUM_REPLICATES);
  //std::iota(std::begin(x), std::end(x), 0);
  for (std::size_t i{1}; i < x.size(); ++i) {
    x[i] = x[i - 1] + 1.0;
  }

  auto first_half{std::async(std::launch::async, resample, x, std::begin(results), HALF)};
  auto second_half{std::async(std::launch::async, resample, x, std::begin(results) + HALF, HALF)};

  first_half.get();
  second_half.get();

  std::cout << var(results) << '\n';
}
