////////////////////////////////////////////////////////////////////////
// Class:       AssnsReaderTest
// Module Type: analyzer
// File:        AssnsReaderTest_module.cc
//
// Generated at Wed Jul 13 14:36:05 2011 by Chris Green using artmod
// from art v0_07_12.
////////////////////////////////////////////////////////////////////////

#include "boost/test/unit_test.hpp"

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "art/test/TestObjects/AssnTestData.h"
#include "canvas/Persistency/Common/Assns.h"

#include <iostream>
#include <regex>
#include <type_traits>

namespace art::test {
  class AssnsReaderTest;
}

class art::test::AssnsReaderTest : public EDAnalyzer {
public:
  explicit AssnsReaderTest(fhicl::ParameterSet const& p);

  void analyze(art::Event const& e) override;

private:
  template <typename A, typename B, typename D>
  void checkExpectedSize_(std::vector<Handle<Assns<A, B, D>>> const& v) const;

  std::string const inputLabel_;
  std::string const process_;
  std::string const wantVoid_; // "ALL," "NONE," or "SOME."
  bool const wantMV_;          // Expect mapvector and derived Assns.
  bool const wantMany_;        // Expect many-to-many associations.
  bool const wantAmbiguous_;   // Expect an extra ABD, causing ambiguity.
};

namespace {
  using A_t = size_t;
  using B_t = std::string;
  using AssnsABX_t = art::Assns<size_t, std::string, arttest::AssnTestData>;
  using AssnsBAX_t = art::Assns<std::string, size_t, arttest::AssnTestData>;
  using AssnsABY_t = art::Assns<size_t, std::string, std::string>;
  using AssnsBAY_t = art::Assns<std::string, size_t, std::string>;
  using AssnsABV_t = art::Assns<size_t, std::string>;
  using AssnsBAV_t = art::Assns<std::string, size_t>;
} // namespace

using namespace std::string_literals;

art::test::AssnsReaderTest::AssnsReaderTest(fhicl::ParameterSet const& ps)
  : EDAnalyzer(ps)
  , inputLabel_(ps.get<std::string>("inputLabel"))
  , process_(ps.get<std::string>("process", {}))
  , wantVoid_(ps.get<std::string>("wantVoid", "ALL"))
  , wantMV_(ps.get<bool>("wantMV", true))
  , wantMany_(ps.get<bool>("wantMany", true))
  , wantAmbiguous_(ps.get<bool>("wantAmbiguous", false))
{
  // consumes<AssnsABV_t>(inputLabel_);
  // consumes<AssnsBAV_t>(inputLabel_);
  // consumes<AssnsABV_t>({inputLabel_, "many"s});
  // consumes<AssnsBAV_t>({inputLabel_, "many"s});
  // consumes<AssnsABV_t>({inputLabel_, "mapvec"s});
  // consumes<AssnsBAV_t>({inputLabel_, "mapvec"s});
  // consumes<AssnsABV_t>({inputLabel_, "manymapvec"s});
  // consumes<AssnsBAV_t>({inputLabel_, "manymapvec"s});

  // consumes<AssnsABX_t>(inputLabel_);
  // consumes<AssnsBAX_t>(inputLabel_);
  // consumes<AssnsABX_t>({inputLabel_, "many"s});
  // consumes<AssnsBAX_t>({inputLabel_, "many"s});
  // consumes<AssnsABX_t>({inputLabel_, "mapvec"s});
  // consumes<AssnsBAX_t>({inputLabel_, "mapvec"s});
  // consumes<AssnsABX_t>({inputLabel_, "manymapvec"s});
  // consumes<AssnsBAX_t>({inputLabel_, "manymapvec"s});

  // consumesMany<AssnsABV_t>();
  // consumesMany<AssnsBAV_t>();
  // consumesMany<AssnsABX_t>();
  // consumesMany<AssnsBAX_t>();
}

namespace {
  template <typename A, typename B, typename D>
  std::size_t
  expectedSize(std::string const& wantVoid,
               bool wantMV,
               bool wantMany,
               bool wantAmbiguous,
               std::string const& process)
  {
    using namespace std::string_literals;
    static constexpr bool isVoid [[maybe_unused]] =
      std::is_same<D, void>::value;
    std::size_t wanted = 0ull;
    wanted += 1ull;
    if (isVoid && wantVoid != "ALL"s && wantAmbiguous) {
      // Have an Assns<A, B, Y> to deal with too.
      wanted += 1ull;
    }
    if (wantMany) {
      wanted += 1ull;
    }
    if (wantMV) {
      wanted += 1ull;
      if (wantMany) {
        wanted += 1ull;
      }
    }
    wanted *= 2ull; // Two writer modules.
    if (process == "ASSNSW2") {
      // FIXME: Should pull out the config for this process and
      // recursively call ourselves with the right options.
      wanted *= 2ull;
    }
    return wanted;
  }

} // namespace

template <typename A, typename B, typename D>
void
art::test::AssnsReaderTest::checkExpectedSize_(
  std::vector<Handle<Assns<A, B, D>>> const& v) const
{
  BOOST_TEST(v.size() ==
             (expectedSize<A, B, D>(
               wantVoid_, wantMV_, wantMany_, wantAmbiguous_, process_)));
}
void
art::test::AssnsReaderTest::analyze(Event const& e)
{
  std::size_t const vSize = (wantVoid_ == "ALL"s) ? 4ull : 3ull;
  std::size_t const mvVSize = (wantVoid_ != "NONE"s) ? 4ull : 3ull;

  auto const vSizeM = vSize + ((wantVoid_ == "ALL"s) ? 2ull : 1ull);
  auto const mvVSizeM = mvVSize + ((wantVoid_ != "NONE"s) ? 2ull : 1ull);

  static std::regex const re{
    "Found 2 products rather than one that match all criteria"};

  // Check <A, B> and <B, A>.
  Handle<AssnsABV_t> hABV;
  try {
    e.getByLabel(inputLabel_, hABV);
    BOOST_TEST(hABV->size() == vSize);
    if (!process_.empty()) {
      BOOST_TEST(hABV.provenance()->processName() == process_);
    }
  }
  catch (Exception const& e) {
    if (!wantAmbiguous_ || wantVoid_ == "ALL") {
      throw; // Shouldn't have gotten here.
    } else { // Expected exception.
      BOOST_TEST_REQUIRE(e.categoryCode() == errors::ProductNotFound);
      BOOST_TEST(std::regex_search(e.what(), re));
    }
  }

  Handle<AssnsBAV_t> hBAV;
  try {
    e.getByLabel(inputLabel_, hBAV);
    BOOST_TEST(hBAV->size() == vSize);
    if (!process_.empty()) {
      BOOST_TEST(hBAV.provenance()->processName() == process_);
    }
  }
  catch (Exception const& e) {
    if (!wantAmbiguous_ || wantVoid_ == "ALL") {
      throw; // Shouldn't have gotten here.
    } else { // Expected exception.
      BOOST_TEST_REQUIRE(e.categoryCode() == errors::ProductNotFound);
      BOOST_TEST(std::regex_search(e.what(), re));
    }
  }

  if (wantMany_) {
    auto const hABVM = e.getValidHandle<AssnsABV_t>({inputLabel_, "many"s});
    BOOST_TEST(hABVM->size() == vSizeM);
    auto const hBAVM = e.getValidHandle<AssnsBAV_t>({inputLabel_, "many"s});
    BOOST_TEST(hBAVM->size() == vSizeM);
    if (!process_.empty()) {
      BOOST_TEST(hABVM.provenance()->processName() == process_);
      BOOST_TEST(hBAVM.provenance()->processName() == process_);
    }
  }
  if (wantMV_) {
    auto const hmvABV = e.getValidHandle<AssnsABV_t>({inputLabel_, "mapvec"s});
    BOOST_TEST(hmvABV->size() == mvVSize);
    auto const hmvBAV = e.getValidHandle<AssnsBAV_t>({inputLabel_, "mapvec"s});
    BOOST_TEST(hmvBAV->size() == mvVSize);
    if (!process_.empty()) {
      BOOST_TEST(hmvABV.provenance()->processName() == process_);
      BOOST_TEST(hmvBAV.provenance()->processName() == process_);
    }
    if (wantMany_) {
      auto const hmvABVM =
        e.getValidHandle<AssnsABV_t>({inputLabel_, "manymapvec"s});
      BOOST_TEST(hmvABVM->size() == mvVSizeM);
      auto const hmvBAVM =
        e.getValidHandle<AssnsBAV_t>({inputLabel_, "manymapvec"s});
      BOOST_TEST(hmvBAVM->size() == mvVSizeM);
      if (!process_.empty()) {
        BOOST_TEST(hmvABVM.provenance()->processName() == process_);
        BOOST_TEST(hmvBAVM.provenance()->processName() == process_);
      }
    }
  }

  // Check all <A, B, V> and <B, A, V>.
  BOOST_TEST(e.getProduct<AssnsABX_t>(inputLabel_).size() == 3ull);
  BOOST_TEST(e.getProduct<AssnsBAX_t>(inputLabel_).size() == 3ull);
  if (wantMany_) {
    BOOST_TEST(e.getProduct<AssnsABX_t>({inputLabel_, "many"s}).size() == 4ull);
    BOOST_TEST(e.getProduct<AssnsBAX_t>({inputLabel_, "many"s}).size() == 4ull);
  }
  if (wantMV_) {
    BOOST_TEST(e.getProduct<AssnsABX_t>({inputLabel_, "mapvec"s}).size() ==
               3ull);
    BOOST_TEST(e.getProduct<AssnsBAX_t>({inputLabel_, "mapvec"s}).size() ==
               3ull);
    if (wantMany_) {
      BOOST_TEST(
        e.getProduct<AssnsABX_t>({inputLabel_, "manymapvec"s}).size() == 4ull);
      BOOST_TEST(
        e.getProduct<AssnsBAX_t>({inputLabel_, "manymapvec"s}).size() == 4ull);
    }
  }

  // Check expected behavior of getManyByType().
  checkExpectedSize_(e.getMany<AssnsABV_t>());
  checkExpectedSize_(e.getMany<AssnsBAV_t>());
  checkExpectedSize_(e.getMany<AssnsABX_t>());
  checkExpectedSize_(e.getMany<AssnsBAX_t>());
}

DEFINE_ART_MODULE(art::test::AssnsReaderTest)
