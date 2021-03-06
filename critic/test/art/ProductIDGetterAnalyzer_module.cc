////////////////////////////////////////////////////////////////////////
// Class:       ProductIDGetterAnalyzer
// Module Type: analyzer
// File:        ProductIDGetterAnalyzer_module.cc
//
// Generated at Thu Jun 16 11:18:23 2011 by Chris Green using artmod
// from art v0_07_09.
////////////////////////////////////////////////////////////////////////

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include "boost/test/unit_test.hpp"

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/SubRun.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "fhiclcpp/types/Atom.h"

namespace art::test {
  class ProductIDGetterAnalyzer;
}

class art::test::ProductIDGetterAnalyzer : public EDAnalyzer {
  struct Config {
    fhicl::Atom<std::string> input_label{fhicl::Name{"input_label"}};
  };

public:
  using Parameters = Table<Config>;
  explicit ProductIDGetterAnalyzer(Parameters const& p);

private:
  void beginSubRun(SubRun const& sr) override;
  void analyze(Event const& e) override;
  std::string const input_label_;
};

art::test::ProductIDGetterAnalyzer::ProductIDGetterAnalyzer(Parameters const& p)
  : EDAnalyzer{p}, input_label_{p().input_label()}
{
  consumes<art::Ptr<int>>(input_label_);
  consumes<art::Ptr<int>, art::InSubRun>(input_label_);
}

void
art::test::ProductIDGetterAnalyzer::beginSubRun(SubRun const& sr)
{
  Handle<Ptr<int>> h;
  BOOST_TEST_REQUIRE(sr.getByLabel(input_label_, h));
  BOOST_TEST_REQUIRE(**h == 5);
  // Retrieve Ptr by its ProductID, *NOT* by the ProductID of the
  // pointed-to product.
  auto const pid = h.id();
  BOOST_TEST_REQUIRE(sr.get(pid, h));
  BOOST_TEST_REQUIRE(**h == 5);
}

void
art::test::ProductIDGetterAnalyzer::analyze(Event const& e)
{
  Handle<Ptr<int>> h;
  BOOST_TEST_REQUIRE(e.getByLabel(input_label_, h));
  BOOST_TEST_REQUIRE(**h == 4);
  // Retrieve Ptr by its ProductID, *NOT* by the ProductID of the
  // pointed-to product.
  auto const pid = h.id();
  BOOST_TEST_REQUIRE(e.get(pid, h));
  BOOST_TEST_REQUIRE(**h == 4);
}

DEFINE_ART_MODULE(art::test::ProductIDGetterAnalyzer)
