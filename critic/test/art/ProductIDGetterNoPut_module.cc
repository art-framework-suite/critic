////////////////////////////////////////////////////////////////////////
// Class:       ProductIDGetterNoPut
// Module Type: producer
// File:        ProductIDGetterNoPut_module.cc
////////////////////////////////////////////////////////////////////////

#include "boost/test/unit_test.hpp"

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Principal/Event.h"
#include "canvas/Persistency/Provenance/ProductID.h"

namespace art::test {
  class ProductIDGetterNoPut;
}

class art::test::ProductIDGetterNoPut : public EDProducer {
public:
  explicit ProductIDGetterNoPut(fhicl::ParameterSet const&);

private:
  void produce(art::Event&) override;
};

art::test::ProductIDGetterNoPut::ProductIDGetterNoPut(
  fhicl::ParameterSet const& ps)
  : EDProducer{ps}
{
  produces<int>();
  produces<int>("i1");
}

void
art::test::ProductIDGetterNoPut::produce(Event& e)
{
  ProductID const p1{e.getProductID<int>()};
  BOOST_TEST_REQUIRE(p1.isValid());
  ProductID const p2{e.getProductID<int>("i1")};
  BOOST_TEST_REQUIRE(p2.isValid());
  BOOST_TEST(p1 != p2);
}

DEFINE_ART_MODULE(art::test::ProductIDGetterNoPut)
