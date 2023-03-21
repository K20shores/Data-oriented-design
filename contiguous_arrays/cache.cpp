#include <vector>
#include <chrono>
#include <iostream>
#include <cmath>
#include <matplot/matplot.h>

double save = 0;

struct Point {
    Point() : x_(0), y_(0) {}
    Point(double x, double y): x_(x), y_(y) {}
    double x_;
    double y_;
};

std::chrono::duration<long, std::nano> slow(int N = 10000) {
  std::vector<Point *> points;

  for(int i = 0; i < N; ++i) {
      Point * p = new Point(i*N, i*i*N);
      points.push_back(p);
  }

  double sum;
  auto start = std::chrono::high_resolution_clock::now();
  for(int i = 0; i < N; ++i) {
      sum += points[i]->x_ + points[i]->y_;
  }
  auto stop = std::chrono::high_resolution_clock::now();

  save += sum;

  for(int i = 0; i < N; ++i) {
      delete points[i];
  }

  return std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
}

std::chrono::duration<long, std::nano> fast(int N = 10000) {
  Point* points = new Point[N];

  for(int i = 0; i < N; ++i) {
      points[i] = Point(i*N, i*i*N);
  }

  double sum;
  auto start = std::chrono::high_resolution_clock::now();
  for(int i = 0; i < N; ++i) {
      sum += points[i].x_ + points[i].y_;
  }
  auto stop = std::chrono::high_resolution_clock::now();

  save += sum;

  delete[] points;

  return std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
}

int main(int argv, char** argc){
  int N = 10;
  int n_iter = 4;
  int start = 2;

  if (argv == 2){
      n_iter = std::stoi(argc[1]);
  }

  std::vector<double> ns;
  std::vector<double> speedup;
  for(int i = start; i < start + n_iter; ++i)
  {
      auto n = std::pow(N, i);
      ns.push_back(n);
      auto slow_time = slow(n);
      auto fast_time = fast(n);
      speedup.push_back(double(slow_time.count()) / double(fast_time.count()));

      n *= 5;
      ns.push_back(n);
      slow_time = slow(n);
      fast_time = fast(n);
      speedup.push_back(double(slow_time.count()) / double(fast_time.count()));
  }

  matplot::semilogx(ns, speedup, "-o");
  matplot::title("Contiguous arrays vs pointers");
  matplot::xlabel("Number of array elements");
  matplot::ylabel("Speedup");
  matplot::show();
}
