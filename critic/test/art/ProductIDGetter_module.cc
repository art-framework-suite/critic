////////////////////////////////////////////////////////////////////////
// Class:       ProductIDGetter
// Module Type: producer
// File:        ProductIDGetter_module.cc
//
// Generated at Wed Jun 15 17:19:52 2011 by Chris Green using artmod
// from art v0_07_09.
////////////////////////////////////////////////////////////////////////

#include "boost/test/unit_test.hpp"

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/SubRun.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "canvas/Persistency/Provenance/ProductID.h"

#include <vector>

namespace art::test {
  class ProductIDGetter;
}

class art::test::ProductIDGetter : public EDProducer {
public:
  struct Config {};
  using Parameters = Table<Config>;
  explicit ProductIDGetter(Parameters const&);

private:
  void beginSubRun(SubRun&) override;
  void produce(Event&) override;
};

art::test::ProductIDGetter::ProductIDGetter(Parameters const& ps)
  : EDProducer{ps}
{
  produces<std::vector<int>, InSubRun>();
  produces<Ptr<int>, InSubRun>();
  produces<std::vector<int>>();
  produces<Ptr<int>>();
}

void
art::test::ProductIDGetter::beginSubRun(SubRun& sr)
{
  auto vip = std::make_unique<std::vector<int>>();
  vip->push_back(1);
  vip->push_back(3);
  vip->push_back(5);
  vip->push_back(7);

  // Old-style way of making Ptr to product inserted in the same module.
  ProductID const pv{sr.getProductID<std::vector<int>>()};
  auto ptr = std::make_unique<Ptr<int>>(pv, 2, sr.productGetter(pv));
  BOOST_TEST_REQUIRE(ptr->id().isValid());

  // New-style way of making Ptr to product inserted in the same module.
  auto h = sr.put(move(vip), fullSubRun());
  Ptr<int> const ptr_check{h, 2};
  BOOST_TEST_REQUIRE(ptr->id() == ptr_check.id());

  // Ptr and Handle available for use even now
  BOOST_TEST_REQUIRE(ptr_check.isAvailable());
  BOOST_TEST_REQUIRE((*h)[2] == 5);
  BOOST_TEST_REQUIRE(*ptr_check == 5);

  sr.put(move(ptr), fullSubRun());
}

void
art::test::ProductIDGetter::produce(Event& e)
{
  // Test that getting a ProductID for an unregistered product yields
  // an exception.
  BOOST_REQUIRE_EXCEPTION(
    e.getProductID<int>(), Exception, [](Exception const& e) {
      return e.categoryCode() == errors::ProductRegistrationFailure;
    });

  auto vip = std::make_unique<std::vector<int>>();
  vip->push_back(0);
  vip->push_back(2);
  vip->push_back(4);
  vip->push_back(6);

  // Old-style way of making Ptr to product inserted in the same module.
  ProductID const pv{e.getProductID<std::vector<int>>()};
  auto ptr = std::make_unique<Ptr<int>>(pv, 2, e.productGetter(pv));
  BOOST_TEST_REQUIRE(ptr->id().isValid());

  // New-style way of making Ptr to product inserted in the same module.
  auto h = e.put(move(vip));
  Ptr<int> const ptr_check{h, 2};
  BOOST_TEST_REQUIRE(ptr->id() == ptr_check.id());

  // Ptr and Handle available for use even now
  BOOST_TEST_REQUIRE(ptr_check.isAvailable());
  BOOST_TEST_REQUIRE((*h)[2] == 4);
  BOOST_TEST_REQUIRE(*ptr_check == 4);

  e.put(move(ptr));
}

DEFINE_ART_MODULE(art::test::ProductIDGetter)
