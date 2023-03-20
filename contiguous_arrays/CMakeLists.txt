add_executable(contiguous_arrays)
set_target_properties(contiguous_arrays PROPERTIES OUTPUT_NAME contiguous_arrays_example)

target_link_libraries(contiguous_arrays plotter)

target_sources(contiguous_arrays
  PRIVATE
    ./cache.cpp
)