#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Core/SharedAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Services/System/TriggerNamesService.h"

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

    std::string processName_;
    std::vector<unsigned> expectedEvents_;
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
    assert(size(results) == 3ull);
    assert(results.at("p1").accept() == should_pass_event);
    assert(results.at("p2").accept());
    assert(results.at("p3").accept() == not should_pass_event);
  }
}

DEFINE_ART_MODULE(CheckPathResults)
