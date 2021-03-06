////////////////////////////////////////////////////////////////////////
// Class:       PtrmvAnalyzer
// Module Type: analyzer
// File:        PtrmvAnalyzer_module.cc
//
// Generated at Tue May 31 08:01:04 2011 by Chris Green using artmod
// from art v0_07_07.
////////////////////////////////////////////////////////////////////////

#include "boost/test/unit_test.hpp"

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "canvas/Persistency/Common/PtrVector.h"

#include "cetlib/map_vector.h"

#include <string>

namespace arttest {
  class PtrmvAnalyzer;
}

namespace {
  using mv_t = cet::map_vector<std::string>;
  using mvp_t = mv_t::value_type;
} // namespace

class arttest::PtrmvAnalyzer : public art::EDAnalyzer {
public:
  struct Config {
    fhicl::Atom<std::string> input_label{fhicl::Name{"input_label"}};
  };
  using Parameters = Table<Config>;
  explicit PtrmvAnalyzer(Parameters const& p);

private:
  void analyze(art::Event const& e) override;
  std::string inputLabel_;
};

arttest::PtrmvAnalyzer::PtrmvAnalyzer(Parameters const& p)
  : art::EDAnalyzer{p}, inputLabel_{p().input_label()}
{}

void
arttest::PtrmvAnalyzer::analyze(art::Event const& e)
{
  // map_vector retrieval.
  art::Handle<mv_t> mv;
  BOOST_TEST_REQUIRE(e.getByLabel(inputLabel_, mv));
  std::string const* item;
  item = mv->getOrNull(cet::map_vector_key(0));
  BOOST_TEST(*item == "ONE");
  item = mv->getOrNull(cet::map_vector_key(3));
  BOOST_TEST(*item == "TWO");
  item = mv->getOrNull(cet::map_vector_key(5));
  BOOST_TEST(*item == "THREE");
  item = mv->getOrNull(cet::map_vector_key(7));
  BOOST_TEST(*item == "FOUR");
  item = mv->getOrNull(cet::map_vector_key(9));
  BOOST_TEST(item == nullptr);

  // Ptr<std::string> retrieval.
  art::Handle<art::Ptr<std::string>> ptr;
  assert(e.getByLabel(inputLabel_, ptr));
  assert(**ptr == "TWO");

  // PtrVector<std::string> retrieval.
  art::Handle<art::PtrVector<std::string>> pv;
  BOOST_TEST_REQUIRE(e.getByLabel(inputLabel_, pv));
  BOOST_TEST(*(*pv)[0] == "THREE");
  BOOST_TEST(*(*pv)[1] == "ONE");
  BOOST_TEST(*(*pv)[2] == "FOUR");
  BOOST_TEST(*(*pv)[3] == "TWO");

  // Ptr<std::string> retrieval.
  art::Handle<art::Ptr<mvp_t>> ptr_p;
  BOOST_TEST_REQUIRE(e.getByLabel(inputLabel_, ptr_p));
  BOOST_TEST((*ptr_p)->first == cet::map_vector_key(3));
  BOOST_TEST((*ptr_p)->second == "TWO");

  // PtrVector<std::string> retrieval.
  art::Handle<art::PtrVector<mvp_t>> pvp;
  BOOST_TEST_REQUIRE(e.getByLabel(inputLabel_, pvp));
  BOOST_TEST((*pvp)[0]->first == cet::map_vector_key(5));
  BOOST_TEST((*pvp)[0]->second == "THREE");
  BOOST_TEST((*pvp)[1]->first == cet::map_vector_key(0));
  BOOST_TEST((*pvp)[1]->second == "ONE");
  BOOST_TEST((*pvp)[2]->first == cet::map_vector_key(7));
  BOOST_TEST((*pvp)[2]->second == "FOUR");
  BOOST_TEST((*pvp)[3]->first == cet::map_vector_key(3));
  BOOST_TEST((*pvp)[3]->second == "TWO");
}

DEFINE_ART_MODULE(arttest::PtrmvAnalyzer)
