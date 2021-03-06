//========================================================================
// ConcurrentEngineRetrieval
//
// This is module that is intended to test the parallel retrieval/use
// of a RandomNumberGenerator engine.  Two tests are performed:
//
//  (1) Create multiple distributions in the c'tor, but do not
//      directly call RandomNumberGenerator::getEngine during the
//      testing of the random-number-generator.
//
//  (2) Create multiple distributions simultaneously be calling
//      RandomNumberGenerator::getEngine in multiple threads.
// ========================================================================

#include "boost/test/unit_test.hpp"

#include "art/Framework/Core/ReplicatedAnalyzer.h"

#include "CLHEP/Random/RandFlat.h"

#include <algorithm>
#include <vector>

namespace art::test {

  class ConcurrentEngineRetrieval : public ReplicatedAnalyzer {
  public:
    explicit ConcurrentEngineRetrieval(fhicl::ParameterSet const& p,
                                       ProcessingFrame const&);

  private:
    void analyze(art::Event const&, art::ProcessingFrame const&) override;
    CLHEP::RandFlat dist_;
  };

  ConcurrentEngineRetrieval::ConcurrentEngineRetrieval(
    fhicl::ParameterSet const& p,
    ProcessingFrame const& frame)
    : ReplicatedAnalyzer{p, frame}, dist_{createEngine(p.get<int>("seed"))}
  {}

  void
  ConcurrentEngineRetrieval::analyze(art::Event const&,
                                     art::ProcessingFrame const&)
  {
    std::vector<std::size_t> numbers(5);
    std::size_t constexpr random_range{1000};
    std::generate_n(numbers.begin(), numbers.size(), [this] {
      return dist_.fireInt(random_range);
    });
    // Find way to verify this for all schedules?
  }

} // namespace art::test

DEFINE_ART_MODULE(art::test::ConcurrentEngineRetrieval)
