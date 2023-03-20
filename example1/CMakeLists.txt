add_executable(example1)

target_link_libraries(example1 plotter)

target_sources(example1
  PRIVATE
    ./cache.cpp
)