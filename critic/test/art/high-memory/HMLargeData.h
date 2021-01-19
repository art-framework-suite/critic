#ifndef critic_test_art_high_memory_HMLargeData_h
#define critic_test_art_high_memory_HMLargeData_h

#include <cassert>
#include <vector>

namespace arttest {
  class HMLargeData;
  constexpr unsigned short N_BLOCKS = 28;
}

class arttest::HMLargeData {
public:
  HMLargeData& operator+=(HMLargeData const& other);
  static constexpr size_t size();
  float* data();
  float const* data() const;

  void aggregate(HMLargeData const& other);

private:
  static constexpr size_t data_size_ = 32 * 12 * 32 * 100 * 3 * 5;
  std::vector<float> data_;
};

arttest::HMLargeData&
arttest::HMLargeData::operator+=(HMLargeData const& other)
{
  auto o = cbegin(other.data_);
  for (auto i = begin(data_), e = end(data_); i != e; ++i, ++o) {
    *i += *o;
  }
  return *this;
}

void
arttest::HMLargeData::aggregate(HMLargeData const& other)
{
  (void)operator+=(other);
}

constexpr size_t
arttest::HMLargeData::size()
{
  return data_size_;
}

float*
arttest::HMLargeData::data()
{
  if (data_.size() < data_size_) {
    data_.resize(data_size_);
  }
  return data_.data();
}

float const*
arttest::HMLargeData::data() const
{
  assert(!(data_.size() < data_size_));
  return data_.data();
}

#endif /* critic_test_art_high_memory_HMLargeData_h */

// Local Variables:
// mode: c++
// End:
