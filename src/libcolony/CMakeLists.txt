set(target_name lcolony)


add_library(${target_name} STATIC
  libcolony/ant.cpp
  libcolony/ant.hpp
)


include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

set(target_name_test test_lcolony)

add_executable(${target_name_test})

include(CompileOptions)
set_compile_options(${target_name_test})

target_sources(
${target_name_test}
PRIVATE
  libcolony/ant.test.cpp
)

target_link_libraries(
${target_name_test}
PRIVATE
  gtest
  lcolony
)

add_test(${target_name_test}
  ${CMAKE_BINARY_DIR}/bin/$
  {target_name_test}
)
