#include "../lib/expected.hpp"
#include <iostream>
#include <stdint.h>

using i32 = int32_t;
using std::string;
using result = tl::expected<i32, string>;
result f(result a, result b) { a. }
int main() {
  result r = f(result(1), tl::make_unexpected("era"));
  r.and_then([](i32 r) { std::cout << r << std::endl; });
  std::cout << r << std::endl;
  return 0;
}
