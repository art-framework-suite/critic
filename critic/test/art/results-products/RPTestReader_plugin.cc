////////////////////////////////////////////////////////////////////////
// Class:       RPTestReader
// Plugin Type: resultsproducer (art v1_15_02)
// File:        RPTestReader_plugin.cc
//
// Generated at Wed Sep 16 16:58:28 2015 by Christopher Green using cetskelgen
// from cetlib version v1_14_00.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/ResultsProducer.h"
#include "art/Framework/Principal/Results.h"
#include "art/Framework/Principal/fwd.h"
#include "art/test/TestObjects/ToyProducts.h"
#include "canvas/Utilities/Exception.h"
#include "fhiclcpp/types/Atom.h"

namespace arttest {
  class RPTestReader;
}

class arttest::RPTestReader : public art::ResultsProducer {
public:
  struct Config {
    fhicl::Atom<std::string> intResultsLabel{fhicl::Name{"intResultsLabel"}};
    fhicl::Atom<std::size_t> nResultsExpected{fhicl::Name{"nResultsExpected"},
                                              1ul};
  };

  using Parameters = art::ResultsProducer::Table<Config>;
  explicit RPTestReader(Parameters const& p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  RPTestReader(RPTestReader const&) = delete;
  RPTestReader(RPTestReader&&) = delete;
  RPTestReader& operator=(RPTestReader const&) = delete;
  RPTestReader& operator=(RPTestReader&&) = delete;

  // Required functions.
  void clear() override;
  void writeResults(art::Results& res) override;

  // Selected optional functions.
  void event(art::Event const& e) override;
  void readResults(art::Results const& res) override;

private:
  bool maybeAccumulateData_(art::Results const& res);

  std::string intResultsLabel_;
  std::size_t expectedResultsSeen_;
  std::size_t resultsSeen_{};
  int total_{};
};

arttest::RPTestReader::RPTestReader(Parameters const& p)
  : intResultsLabel_{p().intResultsLabel()}
  , expectedResultsSeen_{p().nResultsExpected()}
{}

void
arttest::RPTestReader::clear()
{
  resultsSeen_ = 0;
  total_ = 0;
}

void
arttest::RPTestReader::writeResults(art::Results& res)
{
  maybeAccumulateData_(res);
  if (resultsSeen_ != expectedResultsSeen_) {
    throw art::Exception(art::errors::LogicError)
      << "Expected to see " << expectedResultsSeen_ << ", saw " << resultsSeen_
      << ".\n";
  }
}

void
arttest::RPTestReader::event(art::Event const& e [[maybe_unused]])
{
  // NOP.
}

void
arttest::RPTestReader::readResults(art::Results const& res)
{
  maybeAccumulateData_(res);
}

bool
arttest::RPTestReader::maybeAccumulateData_(art::Results const& res)
{
  if (auto hi = res.getHandle<IntProduct>(intResultsLabel_)) {
    ++resultsSeen_;
    if (hi->value != 5.0) {
      throw art::Exception(art::errors::LogicError)
        << "Unexcepted value in results int product: " << hi->value << ".\n";
    }
    total_ += hi->value;
    return true;
  }
  return false;
}

DEFINE_ART_RESULTS_PLUGIN(arttest::RPTestReader)
