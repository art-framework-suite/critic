#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "canvas/Utilities/Exception.h"
#include "critic/test/art/EnsureNoModulesAtEndOfJob.h"
#include "critic/test/art/event-processor/ThrowAfterConfig.h"
#include "fhiclcpp/types/Atom.h"

namespace art::test {
  class ThrowingAnalyzer;
}

class art::test::ThrowingAnalyzer : public EDAnalyzer {
  unsigned count_{};
  unsigned threshold_;

public:
  using Parameters = EDAnalyzer::Table<ThrowAfterConfig>;

  ThrowingAnalyzer(Parameters const& p)
    : EDAnalyzer{p}, threshold_{p().throwAfter()}
  {
    if (p().throwFromCtor()) {
      throw Exception{errors::OtherArt} << "Throw from c'tor.\n";
    }
    ServiceHandle<EnsureNoModulesAtEndOfJob>()->increment();
  }

  ~ThrowingAnalyzer()
  {
    ServiceHandle<EnsureNoModulesAtEndOfJob>()->decrement();
  }

  void
  analyze(Event const&) override
  {
    if (++count_ > threshold_) {
      throw Exception{errors::OtherArt} << "Throw from analyze.\n";
    }
  }
};

DEFINE_ART_MODULE(art::test::ThrowingAnalyzer)
