////////////////////////////////////////////////////////////////////////
// Class:       DropTestAnalyzer
// Module Type: analyzer
// File:        DropTestAnalyzer_module.cc
//
// Generated at Mon Aug  1 13:28:48 2011 by Chris Green using artmod
// from art v0_07_12.
////////////////////////////////////////////////////////////////////////

#include "boost/test/unit_test.hpp"

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "fhiclcpp/types/Atom.h"

#include <string>

namespace arttest {
  class DropTestAnalyzer;
}

class arttest::DropTestAnalyzer : public art::EDAnalyzer {
public:
  struct Config {
    fhicl::Atom<std::string> input_label{fhicl::Name("input_label")};
    fhicl::Atom<bool> keepString{fhicl::Name("keepString"), false};
    fhicl::Atom<bool> keepMapVector{fhicl::Name("keepMapVector"), true};
  };
  using Parameters = Table<Config>;
  explicit DropTestAnalyzer(Parameters const& p);

private:
  void analyze(art::Event const& e) override;

  art::ProductToken<art::Ptr<std::string>> ptrToken_;
  using mv_t = cet::map_vector<std::string>;
  art::ProductToken<mv_t> mapToken_;
  bool keepString_;
  bool keepMapVector_;
};

arttest::DropTestAnalyzer::DropTestAnalyzer(Parameters const& p)
  : art::EDAnalyzer{p}
  , ptrToken_{consumes<art::Ptr<std::string>>(p().input_label())}
  , mapToken_{consumes<mv_t>(p().input_label())}
  , keepString_{p().keepString()}
  , keepMapVector_{p().keepMapVector()}
{}

void
arttest::DropTestAnalyzer::analyze(art::Event const& e)
{
  auto const sh = e.getHandle(ptrToken_);
  BOOST_TEST_REQUIRE(sh.isValid() == keepString_);
  if (keepString_ && keepMapVector_) {
    BOOST_TEST(**sh == "TWO");
  }

  auto const mvth = e.getHandle(mapToken_);
  BOOST_TEST_REQUIRE(mvth.isValid() == keepMapVector_);

  if (!keepMapVector_)
    return;

  mv_t const& mapvec = *mvth;
  BOOST_TEST_REQUIRE(mapvec[cet::map_vector_key(7)] == "FOUR");
  BOOST_TEST_REQUIRE(mapvec[cet::map_vector_key(5)] == "THREE");
  BOOST_TEST_REQUIRE(mapvec[cet::map_vector_key(3)] == "TWO");
  BOOST_TEST_REQUIRE(mapvec[cet::map_vector_key(0)] == "ONE");
}

DEFINE_ART_MODULE(arttest::DropTestAnalyzer)
