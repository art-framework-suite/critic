#ifndef critic_test_art_Wanted_h
#define critic_test_art_Wanted_h
// vim: set sw=2 expandtab :

#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"

namespace fhicl {
  class ParameterSet;
}

namespace art::test {

  class Wanted {
  public:
    explicit Wanted(fhicl::ParameterSet const&) {}

    int
    getCachedValue() const
    {
      return cached_value_;
    }

    void
    setValue(int const value)
    {
      cached_value_ = value;
    }

  private:
    int cached_value_;
  };

} // namespace art::test

DECLARE_ART_SERVICE(art::test::Wanted, LEGACY)

#endif /* critic_test_art_Wanted_h */

// Local Variables:
// mode: c++
// End:
