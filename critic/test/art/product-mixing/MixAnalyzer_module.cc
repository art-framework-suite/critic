////////////////////////////////////////////////////////////////////////
// Class:       MixAnalyzer
// Module Type: analyzer
// File:        MixAnalyzer_module.cc
//
// Generated at Mon May 16 10:45:57 2011 by Chris Green using artmod
// from art v0_06_02.
////////////////////////////////////////////////////////////////////////

#include "boost/test/unit_test.hpp"

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/IO/ProductMix/MixTypes.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/test/TestObjects/ProductWithPtrs.h"
#include "art/test/TestObjects/ToyProducts.h"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace arttest {
  class MixAnalyzer;
}

class arttest::MixAnalyzer : public art::EDAnalyzer {
public:
  explicit MixAnalyzer(fhicl::ParameterSet const& p);

private:
  void analyze(art::Event const& e) override;

  using mv_t = cet::map_vector<unsigned int>;
  using mvv_t = mv_t::value_type;
  using mvm_t = mv_t::mapped_type;

  size_t eventCounter_{};
  size_t nSecondaries_;
  std::string mixFilterLabel_;
};

arttest::MixAnalyzer::MixAnalyzer(fhicl::ParameterSet const& p)
  : art::EDAnalyzer{p}
  , nSecondaries_{p.get<size_t>("numSecondaries", 1)}
  , mixFilterLabel_{p.get<std::string>("mixFilterLabel", "mixFilter")}
{}

void
arttest::MixAnalyzer::analyze(art::Event const& e)
{
  ++eventCounter_;

  // Double
  double d = e.getProduct<double>({mixFilterLabel_, "doubleLabel"});
  double dExpected =
    ((2 * eventCounter_ - 1) * nSecondaries_ + 1) * nSecondaries_ / 2;
  assert(d == dExpected);

  // IntProduct
  auto ip = e.getProduct<IntProduct>({mixFilterLabel_, "IntProductLabel"});
  BOOST_TEST_REQUIRE(ip.value == dExpected + 1000000 * nSecondaries_);

  // String
  auto const& s = e.getProduct<std::string>({mixFilterLabel_, "stringLabel"});
  std::ostringstream sExp;
  for (size_t i = 1; i <= nSecondaries_; ++i) {
    sExp << "string value: " << std::setfill('0') << std::setw(7)
         << (eventCounter_ - 1) * nSecondaries_ + i << "\n";
  }
  BOOST_TEST_REQUIRE(s == sExp.str());

  // 1. std::vector<double>
  auto const& vd = e.getProduct<std::vector<double>>(
    {mixFilterLabel_, "doubleCollectionLabel"});
  BOOST_TEST_REQUIRE(size(vd) == 10 * nSecondaries_);
  for (size_t i = 0; i < nSecondaries_; ++i) {
    for (size_t j = 1; j < 11; ++j) {
      BOOST_TEST_REQUIRE(vd[i * 10 + j - 1] ==
                         j + 10 * (i + (eventCounter_ - 1) * nSecondaries_));
    }
  }

  // 2. std::vector<art::Ptr<double> >
  // 3. art::PtrVector<double>
  // 4. ProductWithPtrs

  auto const& vpd = e.getProduct<std::vector<art::Ptr<double>>>(
    {mixFilterLabel_, "doubleVectorPtrLabel"});
#ifndef ART_NO_MIX_PTRVECTOR
  auto const& pvd = e.getProduct<art::PtrVector<double>>(
    {mixFilterLabel_, "doublePtrVectorLabel"}); // 3.
#endif
  art::Handle<arttest::ProductWithPtrs> pwpH;
  BOOST_TEST_REQUIRE(
    e.getByLabel(mixFilterLabel_, "ProductWithPtrsLabel", pwpH)); // 4.

  for (size_t i = 0; i < nSecondaries_; ++i) {
    BOOST_TEST_REQUIRE(*vpd[i * 3 + 0] == vd[(i * 10) + 0]); // 2.
    BOOST_TEST_REQUIRE(*vpd[i * 3 + 1] == vd[(i * 10) + 4]); // 2.
    BOOST_TEST_REQUIRE(*vpd[i * 3 + 2] == vd[(i * 10) + 8]); // 2.
#ifndef ART_NO_MIX_PTRVECTOR
    BOOST_TEST_REQUIRE(*pvd[i * 3 + 0] == vd[(i * 10) + 1]); // 3.
    BOOST_TEST_REQUIRE(*pvd[i * 3 + 1] == vd[(i * 10) + 5]); // 3.
    BOOST_TEST_REQUIRE(*pvd[i * 3 + 2] == vd[(i * 10) + 9]); // 3.
#endif
    BOOST_TEST_REQUIRE(*(pwpH->vectorPtrDouble())[i * 3 + 0] ==
                       *vpd[i * 3 + 0]); // 4.
    BOOST_TEST_REQUIRE(*(pwpH->vectorPtrDouble())[i * 3 + 1] ==
                       *vpd[i * 3 + 1]); // 4.
    BOOST_TEST_REQUIRE(*(pwpH->vectorPtrDouble())[i * 3 + 2] ==
                       *vpd[i * 3 + 2]); // 4.
#ifndef ART_NO_MIX_PTRVECTOR
    BOOST_TEST_REQUIRE(*(pwpH->ptrVectorDouble())[i * 3 + 0] ==
                       *pvd[i * 3 + 0]); // 4.
    BOOST_TEST_REQUIRE(*(pwpH->ptrVectorDouble())[i * 3 + 1] ==
                       *pvd[i * 3 + 1]); // 4.
    BOOST_TEST_REQUIRE(*(pwpH->ptrVectorDouble())[i * 3 + 2] ==
                       *pvd[i * 3 + 2]); // 4.
#endif
  }

  // map_vector<unsigned int>
  auto const& mv = e.getProduct<mv_t>({mixFilterLabel_, "mapVectorLabel"});
  BOOST_TEST_REQUIRE(mv.size() == 5 * nSecondaries_);
  {
    auto it = mv.begin();
    size_t delta = 0;
    size_t index = 0;
    for (size_t i = 0; i < nSecondaries_; ++i, delta = index + 1) {
      for (size_t j = 0; j < 5; ++j, ++it) {
        index =
          1 + j * 2 + delta + 10 * (i + nSecondaries_ * (eventCounter_ - 1));
        BOOST_TEST_REQUIRE(it->first ==
                           static_cast<cet::map_vector_key>(index));
        size_t answer = j + 1 + 5 * i + (eventCounter_ - 1) * 5 * nSecondaries_;
        BOOST_TEST_REQUIRE(it->second == answer);
        BOOST_TEST_REQUIRE(*mv.getOrNull(cet::map_vector_key(index)) == answer);
      }
    }
    std::cerr << "\n";
  }

  // Ptrs into map_vector
  art::Handle<std::vector<art::Ptr<mvv_t>>> mvvp;
  BOOST_TEST_REQUIRE(e.getByLabel(mixFilterLabel_, "intVectorPtrLabel", mvvp));
  BOOST_TEST_REQUIRE(mvvp->size() == 5 * nSecondaries_);
  {
    auto it = mvvp->begin();
    size_t delta = 0;
    size_t index_base = 0;
    for (size_t i = 0; i < nSecondaries_; ++i, delta = index_base + 9, ++it) {
      std::cerr << "delta = " << delta << "\n";
      index_base = delta + 1 + 10 * (i + nSecondaries_ * (eventCounter_ - 1));
      size_t answer_base = (eventCounter_ - 1) * 5 * nSecondaries_ + i * 5 + 1;
      BOOST_TEST_REQUIRE((*it)->first ==
                         static_cast<cet::map_vector_key>(index_base + 6));
      BOOST_TEST_REQUIRE((*it)->second == answer_base + 3);
      ++it;
      BOOST_TEST_REQUIRE((*it)->first ==
                         static_cast<cet::map_vector_key>(index_base + 0));
      BOOST_TEST_REQUIRE((*it)->second == answer_base + 0);
      ++it;
      BOOST_TEST_REQUIRE((*it)->first ==
                         static_cast<cet::map_vector_key>(index_base + 2));
      BOOST_TEST_REQUIRE((*it)->second == answer_base + 1);
      ++it;
      BOOST_TEST_REQUIRE((*it)->first ==
                         static_cast<cet::map_vector_key>(index_base + 8));
      BOOST_TEST_REQUIRE((*it)->second == answer_base + 4);
      ++it;
      BOOST_TEST_REQUIRE((*it)->first ==
                         static_cast<cet::map_vector_key>(index_base + 4));
      BOOST_TEST_REQUIRE((*it)->second == answer_base + 2);
    }
  }

  // Bookkeeping.
  auto const& bs = e.getProduct<std::string>(mixFilterLabel_);
  BOOST_TEST_REQUIRE(bs == "BlahBlahBlah");

  auto const& eids = e.getProduct<art::EventIDSequence>(mixFilterLabel_);
  for (size_t i = 0; i < nSecondaries_; ++i) {
    BOOST_TEST_REQUIRE(eids[i].event() + (eids[i].subRun() * 100) +
                         ((eids[i].run() - 1) * 500) ==
                       (eventCounter_ - 1) * nSecondaries_ + i + 1);
  }
}

DEFINE_ART_MODULE(arttest::MixAnalyzer)
