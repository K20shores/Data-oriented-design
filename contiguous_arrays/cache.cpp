#include <vector>
#include <chrono>
#include <iostream>
#include <cmath>
#include <matplot/matplot.h>

double save = 0;

struct Point
{
  Point() : x_(0), y_(0) {}
  Point(double x, double y) : x_(x), y_(y) {}
  double x_;
  double y_;
  int color = 1;
  int size = 2;
  double alpha = 0.5;
};

std::chrono::duration<long, std::nano> vec_pointers(int N = 10000)
{
  std::vector<Point *> points;

  for (int i = 0; i < N; ++i)
  {
    points.push_back(new Point(i * N, i * i * N));
  }

  double sum;
  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i)
  {
    sum += points[i]->x_ + points[i]->y_;
  }
  auto stop = std::chrono::high_resolution_clock::now();

  save += sum;

  for (int i = 0; i < N; ++i)
  {
    delete points[i];
  }

  return std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
}

std::chrono::duration<long, std::nano> vec_objects(int N = 10000)
{
  std::vector<Point> points;

  for (int i = 0; i < N; ++i)
  {
    points.push_back(Point(i * N, i * i * N));
  }

  double sum;
  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i)
  {
    sum += points[i].x_ + points[i].y_;
  }
  auto stop = std::chrono::high_resolution_clock::now();

  save += sum;

  return std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
}

std::chrono::duration<long, std::nano> array_of_objects(int N = 10000)
{
  Point *points = new Point[N];

  for (int i = 0; i < N; ++i)
  {
    points[i] = Point(i * N, i * i * N);
  }

  double sum;
  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i)
  {
    sum += points[i].x_ + points[i].y_;
  }
  auto stop = std::chrono::high_resolution_clock::now();

  save += sum;

  delete[] points;

  return std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
}

int main(int argv, char **argc)
{
  int N = 10;
  int n_iter = 4;
  int start = 2;
  int n_trials = 10;

  if (argv == 2)
  {
    n_iter = std::stoi(argc[1]);
  }

  std::vector<double> ns(n_iter);
  for (int i = start; i < start + n_iter; ++i)
  {
    auto n = std::pow(N, i);
    ns.push_back(n);
    ns.push_back(n * 5);
  }

  std::vector<std::vector<double>> speedup_over_pointers;
  std::vector<std::vector<double>> speedup_over_vector;
  for (int trial = 0; trial < n_trials; ++trial)
  {
    std::vector<double> trial_speedup_over_pointers;
    std::vector<double> trial_speedup_over_vectors;
    for (auto n : ns)
    {
      auto vec_obj_time = vec_objects(n);
      auto vec_pointer_time = vec_pointers(n);
      auto fast_time = array_of_objects(n);
      trial_speedup_over_pointers.push_back(double(vec_pointer_time.count()) / double(fast_time.count()));
      trial_speedup_over_vectors.push_back(double(vec_obj_time.count()) / double(fast_time.count()));

      vec_obj_time = vec_objects(n);
      vec_pointer_time = vec_pointers(n);
      fast_time = array_of_objects(n);
      trial_speedup_over_pointers.push_back(double(vec_pointer_time.count()) / double(fast_time.count()));
      trial_speedup_over_vectors.push_back(double(vec_obj_time.count()) / double(fast_time.count()));
    }
    speedup_over_pointers.push_back(trial_speedup_over_pointers);
    speedup_over_vector.push_back(trial_speedup_over_vectors);
  }

/*
[
  [1, 2, 3]
  [4, 5, 6]
]
*/
  std::vector<double> avg_speedup_over_pointers;
  for (int i = 0; i < speedup_over_pointers.size(); ++i) {
    int acc = 0;
    for(int j = 0; j < ns.size(); ++j){
      acc += speedup_over_pointers[i][j];
    }
    avg_speedup_over_pointers.push_back(acc / n_trials);
  }

  std::vector<double> avg_speedup_over_vectors;
  for (int i = 0; i < speedup_over_vector.size(); ++i) {
    int acc = 0;
    for(int j = 0; j < ns.size(); ++j){
      acc += speedup_over_vector[i][j];
    }
    avg_speedup_over_vectors.push_back(acc / n_trials);
  }

  matplot::semilogx(ns, avg_speedup_over_pointers, "-o");
  matplot::semilogx(ns, avg_speedup_over_vectors, "-x");
  matplot::title("Contiguous arrays vs pointers");
  matplot::xlabel("Number of array elements");
  matplot::ylabel("Speedup");
  auto lgd = matplot::legend(std::vector<std::string>({"Line 1", "Line 2"}));
  matplot::save("contiguoys_arrays.png");
}
