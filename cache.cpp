#include <vector>
#include <chrono>
#include <iostream>
#include <cmath>

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

  delete points;

  return std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
}

int main(int argv, char** argc){
  int N = 10;
  int n_iter = 4;

  if (argv == 2){
      n_iter = std::stoi(argc[1]);
  }

  std::vector<std::pair<int, int>> times;
  for(int i = 0; i < n_iter; ++i)
  {
      std::cout << std::pow(N, i) << std::endl;
      auto slow_time = slow(std::pow(N, i));
      auto fast_time = fast(std::pow(N, i));
      std::cout << "slow: " << slow_time.count() << std::endl;
      std::cout << "fast: " << fast_time.count() << std::endl;
      std::cout << "speedup: " << (double)slow_time.count() / (double)fast_time.count() << std::endl;
      times.push_back(std::make_pair(slow_time.count(), fast_time.count()));
  }
  std::cout << save << std::endl;
}
