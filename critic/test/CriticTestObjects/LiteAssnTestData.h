#ifndef critic_test_CriticTestObjects_LiteAssnTestData_h
#define critic_test_CriticTestObjects_LiteAssnTestData_h

#include <cstddef>
#include <ostream>
#include <string>

namespace critictest {
  struct LiteAssnTestData {
    LiteAssnTestData() = default;
    LiteAssnTestData(size_t d1, size_t d2, std::string const& label)
      : d1(d1), d2(d2), label(label)
    {}
    size_t d1{};
    size_t d2{};
    std::string label;
  };

  std::ostream& operator<<(std::ostream& os, LiteAssnTestData const& item);
  bool operator==(LiteAssnTestData const& left, LiteAssnTestData const& right);
  bool operator!=(LiteAssnTestData const& left, LiteAssnTestData const& right);
}
#endif /* critic_test_CriticTestObjects_LiteAssnTestData_h */

// Local Variables:
// mode: c++
// End:
