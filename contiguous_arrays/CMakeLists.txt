add_executable(contiguous_arrays)
set_target_properties(contiguous_arrays PROPERTIES OUTPUT_NAME contiguous_arrays_example)

target_link_libraries(contiguous_arrays
  PUBLIC
    Matplot++::matplot
)

target_sources(contiguous_arrays
  PRIVATE
    ./cache.cpp
)