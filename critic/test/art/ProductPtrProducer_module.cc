////////////////////////////////////////////////////////////////////////
// Class:       ProductPtrProducer
// Module Type: producer
// File:        ProductPtrProducer_module.cc
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/test/TestObjects/ToyProducts.h"
#include "canvas/Persistency/Common/ProductPtr.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/types/Atom.h"

#include <string>

namespace art::test {

  class ProductPtrProducer : public EDProducer {
  public:
    struct Config {
      fhicl::Atom<std::string> read_from{fhicl::Name{"read_from"}};
      fhicl::Atom<std::string> string_to_make{fhicl::Name{"string_to_make"}};
    };
    using Parameters = Table<Config>;
    explicit ProductPtrProducer(Parameters const&);

  private:
    void produce(art::Event& e) override;
    InputTag tag_;
    std::string stringToMake_;
  };

  ProductPtrProducer::ProductPtrProducer(Parameters const& ps)
    : EDProducer{ps}
    , tag_{ps().read_from()}
    , stringToMake_{ps().string_to_make()}
  {
    produces<ProductPtr<arttest::IntProduct>>();
    produces<std::string>();
    produces<ProductPtr<std::string>>();
  }

  void
  ProductPtrProducer::produce(art::Event& e)
  {
    // Make ProductPtr from earlier-placed product
    auto const intProductH = e.getValidHandle<arttest::IntProduct>(tag_);
    auto ptr = std::make_unique<ProductPtr<arttest::IntProduct>>(intProductH);
    e.put(move(ptr));

    // Make ProductPtr from product created in this module
    auto const h = e.put(std::make_unique<std::string>(stringToMake_));
    e.put(std::make_unique<ProductPtr<std::string>>(h));
  }

}

DEFINE_ART_MODULE(art::test::ProductPtrProducer)
