#ifndef test_TestObjects_LiteAssnTestData_h
#define test_TestObjects_LiteAssnTestData_h

#include <cstddef>
#include <string>

namespace critictest {
  struct LiteAssnTestData;
}

struct critictest::LiteAssnTestData {
  LiteAssnTestData() : d1(0), d2(0), label() {}
  LiteAssnTestData(size_t d1, size_t d2, std::string const &label)
    : d1(d1), d2(d2), label(label) {}
  size_t d1;
  size_t d2;
  std::string label;
};
#endif /* test_TestObjects_LiteAssnTestData_h */

// Local Variables:
// mode: c++
// End:
