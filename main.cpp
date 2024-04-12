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

[[nodiscard]] constexpr auto var(const auto& x) {  // TODO: more careful about parameter type
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
  const std::size_t num_replicates,
  const std::vector<double>::iterator start
) {
  std::vector<double> replicate(x.size());
  std::uniform_int_distribution<std::size_t> distribution(0, x.size() - 1);
  std::random_device random_device;
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

  for (
    std::size_t i{}, num_replicates_so_far{}, num_replicates_per_thread{std::max(1ul, num_replicates / num_threads)}, num_replicates_this_thread{}, num_leftover{num_replicates % num_threads};
    i < futures.size() && num_replicates_so_far < num_replicates;
    ++i
  ) {
    num_replicates_this_thread = num_replicates_per_thread;

    if (num_leftover) {
      ++num_replicates_this_thread;
      --num_leftover;
    }

    futures[i] = std::async(
      std::launch::async,
      resample,
      x,
      num_replicates_this_thread,
      std::begin(results) + num_replicates_so_far
    );
    num_replicates_so_far += num_replicates_this_thread;
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
