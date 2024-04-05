#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <future>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>
#include <utility>
#include <vector>

[[nodiscard]] auto var(const auto& x) {  // TODO: more careful about parameter type
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
void resample(
  const std::vector<double>& x,
  const std::vector<double>::iterator start,
  const std::size_t num_replicates
) {
  std::uniform_int_distribution<std::size_t> distribution(0, x.size() - 1);
  std::random_device random_device;
  std::vector<double> replicate(x.size());
  auto generator{std::mt19937{random_device()}};

  for (auto current_position{start}; current_position < start + num_replicates; ++current_position) {
    // adapted from https://stackoverflow.com/questions/42926209/equivalent-function-to-numpy-random-choice-in-c
    std::generate_n(
      std::begin(replicate),
      replicate.size(),
      [&x = std::as_const(x), &distribution, &generator]() mutable {
        return x[distribution(generator)];
      }
    );
    *current_position = var(replicate);
  }
}

//TODO: more careful type for x
[[nodiscard]] double bootstrap(const auto& x, const std::size_t num_replicates, const std::size_t num_threads) {
  std::vector<std::future<void>> futures(num_threads);
  std::vector<double> results(num_replicates);

  if (num_replicates % num_threads) {
    throw "need to handle case where num_threads doesn't divide num_replicates evenly";
  }

  const auto replicates_per_thread{num_replicates / num_threads};  // TODO: more careful

  for (std::size_t i{}; i < futures.size(); ++i) {
    futures[i] = std::async(
      std::launch::async,
      resample,
      x,
      std::begin(results) + replicates_per_thread * i,
      replicates_per_thread
    );
  }

  for (const auto& f: futures) {
    f.wait();
  }

  return var(results);
}

int main(const int, const char** argv) {
  std::stringstream stream;
  std::size_t n, num_replicates;
  stream << argv[1] << ' ' << argv[2];
  stream >> n >> num_replicates;

  std::vector<double> x(n);
  for (std::size_t i{1}; i < x.size(); ++i) {
    x[i] = x[i - 1] + 1.0;
  }

  std::cout << bootstrap(x, num_replicates, 5) << '\n';
}
