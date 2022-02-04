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
#include <sstream>
#include <string>

namespace art::test {
  class MixAnalyzer : public EDAnalyzer {
  public:
    struct Config {
      fhicl::Atom<size_t> numSecondaries{fhicl::Name{"numSecondaries"}, 1};
      fhicl::Atom<std::string> mixFilterLabel{fhicl::Name{"mixFilterLabel"},
                                              "mixFilter"};
    };
    using Parameters = Table<Config>;
    explicit MixAnalyzer(Parameters const& p);

  private:
    void analyze(Event const& e) override;

    using mv_t = cet::map_vector<unsigned int>;
    using mvv_t = mv_t::value_type;

    size_t eventCounter_{};
    size_t nSecondaries_;
    std::string mixFilterLabel_;

    InputTag
    tag_for(std::string const& instanceName) const
    {
      return InputTag{mixFilterLabel_, instanceName};
    }
  };
}

art::test::MixAnalyzer::MixAnalyzer(Parameters const& p)
  : EDAnalyzer{p}
  , nSecondaries_{p().numSecondaries()}
  , mixFilterLabel_{p().mixFilterLabel()}
{}

void
art::test::MixAnalyzer::analyze(Event const& e)
{
  ++eventCounter_;

  // Double
  double const d = e.getProduct<double>(tag_for("doubleLabel"));
  double const dExpected =
    ((2 * eventCounter_ - 1) * nSecondaries_ + 1) * nSecondaries_ / 2;
  BOOST_TEST(d == dExpected);

  // IntProduct
  auto const ip = e.getProduct<arttest::IntProduct>(tag_for("IntProductLabel"));
  BOOST_TEST_REQUIRE(ip.value == dExpected + 1'000'000 * nSecondaries_);

  // String
  auto const& s = e.getProduct<std::string>(tag_for("stringLabel"));
  std::ostringstream sExp;
  for (size_t i = 1; i <= nSecondaries_; ++i) {
    sExp << "string value: " << std::setfill('0') << std::setw(7)
         << (eventCounter_ - 1) * nSecondaries_ + i << "\n";
  }
  BOOST_TEST(s == sExp.str());

  // 1. std::vector<double>
  auto const vdH =
    e.getValidHandle<std::vector<double>>(tag_for("doubleCollectionLabel"));
  auto const& vd = *vdH;
  BOOST_TEST_REQUIRE(size(vd) == 10 * nSecondaries_);
  for (size_t i = 0; i < nSecondaries_; ++i) {
    for (size_t j = 1; j < 11; ++j) {
      BOOST_TEST(vd[i * 10 + j - 1] ==
                 j + 10 * (i + (eventCounter_ - 1) * nSecondaries_));
    }
  }

  // 2. std::vector<Ptr<double> >
  // 3. PtrVector<double>
  // 4. ProductWithPtrs

  auto const& vpd =
    e.getProduct<std::vector<Ptr<double>>>(tag_for("doubleVectorPtrLabel"));
  auto const& pvd =
    e.getProduct<PtrVector<double>>(tag_for("doublePtrVectorLabel")); // 3.
  auto const& pwp =
    e.getProduct<arttest::ProductWithPtrs>(tag_for("ProductWithPtrsLabel"));

  for (size_t i = 0; i < nSecondaries_; ++i) {
    BOOST_TEST(*vpd[i * 3 + 0] == vd[(i * 10) + 0]);     // 2.
    BOOST_TEST(*vpd[i * 3 + 1] == vd[(i * 10) + 4]);     // 2.
    BOOST_TEST(*vpd[i * 3 + 2] == vd[(i * 10) + 8]);     // 2.
    BOOST_TEST(*pvd[i * 3 + 0] == vd[(i * 10) + 1]);     // 3.
    BOOST_TEST(*pvd[i * 3 + 1] == vd[(i * 10) + 5]);     // 3.
    BOOST_TEST(*pvd[i * 3 + 2] == vd[(i * 10) + 9]);     // 3.
    BOOST_TEST(*pwp.vpd_[i * 3 + 0] == *vpd[i * 3 + 0]); // 4.
    BOOST_TEST(*pwp.vpd_[i * 3 + 1] == *vpd[i * 3 + 1]); // 4.
    BOOST_TEST(*pwp.vpd_[i * 3 + 2] == *vpd[i * 3 + 2]); // 4.
    BOOST_TEST(*pwp.pvd_[i * 3 + 0] == *pvd[i * 3 + 0]); // 4.
    BOOST_TEST(*pwp.pvd_[i * 3 + 1] == *pvd[i * 3 + 1]); // 4.
    BOOST_TEST(*pwp.pvd_[i * 3 + 2] == *pvd[i * 3 + 2]); // 4.
  }

  // Embedded ProductPtr<std::vector<double>>
  BOOST_TEST(vdH.id() == pwp.ppvd_.id());
  BOOST_TEST(vd == *pwp.ppvd_);

  // map_vector<unsigned int>
  auto const& mv = e.getProduct<mv_t>(tag_for("mapVectorLabel"));
  BOOST_TEST_REQUIRE(mv.size() == 5 * nSecondaries_);
  {
    auto it = mv.begin();
    size_t delta = 0;
    unsigned index = 0;
    for (size_t i = 0; i < nSecondaries_; ++i, delta = index + 1) {
      for (size_t j = 0; j < 5; ++j, ++it) {
        index =
          1 + j * 2 + delta + 10 * (i + nSecondaries_ * (eventCounter_ - 1));
        BOOST_TEST(it->first == cet::map_vector_key{index});

        size_t answer = j + 1 + 5 * i + (eventCounter_ - 1) * 5 * nSecondaries_;
        BOOST_TEST(it->second == answer);
        BOOST_TEST(*mv.getOrNull(cet::map_vector_key(index)) == answer);
      }
    }
  }

  // Ptrs into map_vector
  auto const& mvvp =
    e.getProduct<std::vector<Ptr<mvv_t>>>(tag_for("intVectorPtrLabel"));
  BOOST_TEST_REQUIRE(mvvp.size() == 5 * nSecondaries_);
  {
    auto it = mvvp.begin();
    size_t delta = 0;
    unsigned index_base = 0;

    auto verify_key_and_value =
      [](auto const& ptr, auto expected_key, auto expected_value) {
        BOOST_TEST(ptr->first == cet::map_vector_key{expected_key});
        BOOST_TEST(ptr->second == expected_value);
      };

    for (size_t i = 0; i < nSecondaries_; ++i, delta = index_base + 9) {
      index_base = delta + 1 + 10 * (i + nSecondaries_ * (eventCounter_ - 1));
      size_t answer_base = (eventCounter_ - 1) * 5 * nSecondaries_ + i * 5 + 1;

      verify_key_and_value(*it++, index_base + 6, answer_base + 3);
      verify_key_and_value(*it++, index_base + 0, answer_base + 0);
      verify_key_and_value(*it++, index_base + 2, answer_base + 1);
      verify_key_and_value(*it++, index_base + 8, answer_base + 4);
      verify_key_and_value(*it++, index_base + 4, answer_base + 2);
    }
  }

  // Bookkeeping.
  auto const& bs = e.getProduct<std::string>(mixFilterLabel_);
  BOOST_TEST(bs == "BlahBlahBlah");

  auto const& eids = e.getProduct<EventIDSequence>(mixFilterLabel_);
  for (size_t i = 0; i < nSecondaries_; ++i) {
    BOOST_TEST(eids[i].event() + (eids[i].subRun() * 100) +
                 ((eids[i].run() - 1) * 500) ==
               (eventCounter_ - 1) * nSecondaries_ + i + 1);
  }
}

DEFINE_ART_MODULE(art::test::MixAnalyzer)
