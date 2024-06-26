#include <algorithm>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <functional>
#include <future>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

using ConfidenceInterval = std::pair<double, double>;  // TODO: more general
using Sample = std::vector<double>;  // TODO: more general
using Statistic = std::function<double(const Sample&)>;  // TODO: more general

template <typename T>
constexpr std::pair<std::size_t, std::size_t> find_percentile_indices(
  const std::vector<T>& v,  // TODO: more general
  const double confidence_level
) {
  const auto len{static_cast<double>(v.size())};
  return {
    static_cast<std::size_t>((1 - confidence_level) * len),
    static_cast<std::size_t>(confidence_level * len)
  };
}

// TODO: constexpr
[[nodiscard]] double var(const Sample& x) {
  const auto n{static_cast<double>(x.size())};
  const auto x_bar{std::reduce(std::cbegin(x), std::cend(x), 0.0) / n};
  return std::transform_reduce(
    std::cbegin(x),
    std::cend(x),
    0.0,
    std::plus<>(),
    [x_bar](const std::floating_point auto xi) { return std::pow(xi - x_bar, 2); }
  ) / (n - 1);
}

void resample(
  const Sample& x,
  const std::size_t num_replicates,
  const Sample::iterator start,
  const Statistic statistic
) {
  if (x.empty()) {
    throw std::length_error("sample can't have 0 observations");
  }
  Sample replicate(x.size());
  std::uniform_int_distribution<std::size_t> distribution(0ul, x.size() - 1);  // TODO: uz
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
    *current_position = statistic(replicate);
  }
}

[[nodiscard]] std::pair<double, ConfidenceInterval> bootstrap(
  const Sample& x,
  const std::size_t num_replicates,
  const std::size_t num_threads,
  const Statistic statistic,
  const double confidence_level = 0.95
) {
  std::vector<std::future<void>> futures(num_threads);
  Sample results(num_replicates);

  const auto num_replicates_per_thread{std::max(1ul, num_replicates / num_threads)};  // TODO: uz

  for (
    std::size_t i{}, num_replicates_so_far{}, num_replicates_this_thread{}, num_leftover{num_replicates % num_threads};
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
      std::begin(results) + num_replicates_so_far,
      statistic
    );
    num_replicates_so_far += num_replicates_this_thread;
  }

  for (const auto& f: futures) {
    f.wait();
  }

  std::sort(std::begin(results), std::end(results));

  const auto variance{var(results)};
  const auto [lower_idx, upper_idx] = find_percentile_indices(results, confidence_level);

  return {variance, {results[lower_idx], results[upper_idx]}};
}

int main(const int, const char** argv) {
  std::stringstream stream;
  std::size_t n, num_replicates;
  stream << argv[1] << ' ' << argv[2];
  stream >> n >> num_replicates;

  Sample x(n);
  for (std::size_t i{1}; i < x.size(); ++i) {
    x[i] = x[i - 1] + 1.0;
  }

  const auto boot{bootstrap(x, num_replicates, 5, var)};

  std::cout << "variance: " << boot.first << "; confidence_interval: (" << boot.second.first << ", " << boot.second.second << ")\n";
}
