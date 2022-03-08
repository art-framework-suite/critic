#include "critic/test/CriticTestObjects/LiteAssnTestData.h"

std::ostream&
critictest::operator<<(std::ostream& os, LiteAssnTestData const& item)
{
  os << "label: \"" << item.label << "\", d1: " << item.d1
     << ", d2: " << item.d2;
  return os;
}

bool
critictest::operator==(LiteAssnTestData const& left,
                       LiteAssnTestData const& right)
{
  return (left.d1 == right.d1) && (left.d2 == right.d2) &&
         (left.label == right.label);
}

inline bool
critictest::operator!=(LiteAssnTestData const& left,
                       LiteAssnTestData const& right)
{
  return !(left == right);
}
