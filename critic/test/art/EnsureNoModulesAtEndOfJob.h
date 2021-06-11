#ifndef critic_test_art_EnsureNoModulesAtEndOfJob_h
#define critic_test_art_EnsureNoModulesAtEndOfJob_h
// vim: set sw=2 expandtab :

#include "art/Framework/Services/Registry/ServiceDeclarationMacros.h"
#include "fhiclcpp/fwd.h"

#include <cassert>

namespace art::test {

  class EnsureNoModulesAtEndOfJob {
  public:
    struct Config {};
    using Parameters = ServiceTable<Config>;
    explicit EnsureNoModulesAtEndOfJob(Parameters const&) {}

    ~EnsureNoModulesAtEndOfJob() noexcept { assert(count_ == 0ull); }

    void
    increment()
    {
      ++count_;
    }
    void
    decrement()
    {
      --count_;
    }

  private:
    std::atomic<size_t> count_{};
  };

} // namespace art::test

DECLARE_ART_SERVICE(art::test::EnsureNoModulesAtEndOfJob, SHARED)

#endif /* critic_test_art_EnsureNoModulesAtEndOfJob_h */

// Local Variables:
// mode: c++
// End:
