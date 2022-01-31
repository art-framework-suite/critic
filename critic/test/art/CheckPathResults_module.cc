#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Core/SharedAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Services/System/TriggerNamesService.h"
#include "oneapi/tbb/concurrent_vector.h"

#include <algorithm>
#include <cassert>

namespace {
  class CheckPathResults : public art::SharedAnalyzer {
  public:
    struct Config {
      fhicl::Atom<std::string> process_name{fhicl::Name{"process_name"}};
      fhicl::Sequence<unsigned> expected_events{fhicl::Name{"expected_events"}};
    };
    using Parameters = Table<Config>;
    CheckPathResults(Parameters const& p, art::ProcessingFrame const&);

  private:
    void analyze(art::Event const& e, art::ProcessingFrame const&) override;
    void endJob(art::ProcessingFrame const&) override;

    std::string processName_;
    std::vector<unsigned> expectedEvents_;
    tbb::concurrent_vector<unsigned> passed_;
    art::ServiceHandle<art::TriggerNamesService const> triggerNames_{};
  };

  CheckPathResults::CheckPathResults(Parameters const& p,
                                     art::ProcessingFrame const&)
    : SharedAnalyzer{p}
    , processName_{p().process_name()}
    , expectedEvents_{p().expected_events()}
  {
    if (not is_sorted(begin(expectedEvents_), end(expectedEvents_))) {
      throw art::Exception{art::errors::OtherArt}
        << "The 'expected_events' parameter must contain a list of sorted "
           "event numbers.";
    }
    async();
  }

  void
  CheckPathResults::analyze(art::Event const& e, art::ProcessingFrame const&)
  {
    auto const& tr = triggerNames_->triggerResults(e, processName_);
    assert(tr.size() == 3ull);
    assert(tr.accept());

    auto const results = triggerNames_->pathResults(e, processName_);
    bool const should_pass_event = std::binary_search(
      begin(expectedEvents_), end(expectedEvents_), e.event());
    if (should_pass_event) {
      passed_.push_back(e.event());
    }
    assert(size(results) == 3ull);
    assert(results.at("p1").accept() == should_pass_event);
    assert(results.at("p2").accept());
    assert(results.at("p3").accept() == not should_pass_event);
  }

  void
  CheckPathResults::endJob(art::ProcessingFrame const&)
  {
    if (passed_.size() == size(expectedEvents_))
      return;

    std::set<unsigned> const passed(passed_.cbegin(), passed_.cend());
    std::set<unsigned> const expected(cbegin(expectedEvents_),
                                      cend(expectedEvents_));
    std::vector<unsigned> missing;
    set_difference(cbegin(expected),
                   cend(expected),
                   cbegin(passed),
                   cend(passed),
                   back_inserter(missing));
    art::Exception e{art::errors::LogicError};
    e << "The following events were not found: ";
    for (auto const num : missing)
      e << num << ' ';
    throw e << '\n';
  }
}

DEFINE_ART_MODULE(CheckPathResults)
