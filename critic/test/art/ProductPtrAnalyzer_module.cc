////////////////////////////////////////////////////////////////////////
// Class:       ProductPtrAnalyzer
// Module Type: analyzer
// File:        ProductPtrAnalyzer_module.cc
////////////////////////////////////////////////////////////////////////

#include "boost/test/unit_test.hpp"

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/test/TestObjects/ToyProducts.h"
#include "canvas/Persistency/Common/ProductPtr.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/types/Atom.h"

#include <string>

using namespace fhicl;

namespace art::test {

  class ProductPtrAnalyzer : public EDAnalyzer {
  public:
    struct Config {
      Atom<std::string> read_from{Name{"read_from"}};
      Atom<int> expected_int{Name{"expected_int"}};
      Atom<std::string> expected_string{Name{"expected_string"}};
    };
    using Parameters = Table<Config>;
    explicit ProductPtrAnalyzer(Parameters const&);

  private:
    void analyze(Event const& e) override;
    InputTag tag_;
    int expectedInt_;
    std::string expectedString_;
  };

  ProductPtrAnalyzer::ProductPtrAnalyzer(Parameters const& ps)
    : EDAnalyzer{ps}
    , tag_{ps().read_from()}
    , expectedInt_{ps().expected_int()}
    , expectedString_{ps().expected_string()}
  {}

  void
  ProductPtrAnalyzer::analyze(art::Event const& e)
  {
    {
      auto const& ptr = e.getProduct<ProductPtr<arttest::IntProduct>>(tag_);
      BOOST_TEST(ptr->value == expectedInt_);
    }

    {
      auto const& ptr = e.getProduct<ProductPtr<std::string>>(tag_);
      BOOST_REQUIRE(ptr);
      BOOST_TEST(*ptr == expectedString_);
    }
  }
}

DEFINE_ART_MODULE(art::test::ProductPtrAnalyzer)
