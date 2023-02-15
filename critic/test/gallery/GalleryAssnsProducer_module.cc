////////////////////////////////////////////////////////////////////////
// Class:       GalleryAssnsProducer
// Module Type: producer
// File:        GalleryAssnsProducer_module.cc
//
// This is started as a copy of the AssnsProducer class in critic/test/art.
// Initially the only differences were these name changes:
//   AssnsProducer -> GalleryAssnsProducer
//   arttest -> critictest
//   AssnTestData -> LiteAssnTestData
// And these changes to avoid conflicting with dictionaries in art/test
//   size_t -> int
//   string -> arttest::StringProduct
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Principal/Event.h"
#include "art/test/TestObjects/ToyProducts.h"
#include "canvas/Persistency/Common/Assns.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "critic/test/CriticTestObjects/LiteAssnTestData.h"

#include "cetlib/map_vector.h"

#include <memory>
#include <vector>

using arttest::StringProduct;
using critictest::LiteAssnTestData;
using std::make_unique;
using std::vector;

using art::ProductID;
using art::Ptr;

using intvec = vector<int>;
using stringvec = vector<StringProduct>;
using mapvec = cet::map_vector<StringProduct>;

namespace critictest {
  class GalleryAssnsProducer;
}

class critictest::GalleryAssnsProducer : public art::EDProducer {
public:
  explicit GalleryAssnsProducer(fhicl::ParameterSet const& p);

private:
  void produce(art::Event& e) override;
};

namespace {
  using AssnsAB_t = art::Assns<int, StringProduct, LiteAssnTestData>;
  using AssnsBA_t = art::Assns<StringProduct, int, LiteAssnTestData>;
  using AssnsVoid_t = art::Assns<int, StringProduct>;
} // namespace

critictest::GalleryAssnsProducer::GalleryAssnsProducer(
  fhicl::ParameterSet const& ps)
  : EDProducer{ps}
{
  produces<intvec>();
  produces<stringvec>();
  produces<mapvec>("mv");
  produces<AssnsAB_t>();
  produces<AssnsAB_t>("mapvec");
  produces<AssnsVoid_t>();
  produces<AssnsVoid_t>("mapvec");
  produces<AssnsAB_t>("many");
  produces<AssnsAB_t>("manymapvec");
  produces<AssnsVoid_t>("many");
  produces<AssnsVoid_t>("manymapvec");
}

void
critictest::GalleryAssnsProducer::produce(art::Event& e)
{
  // Create the data products among which we will make associations.
  auto const viH = e.put(make_unique<intvec>(intvec{2, 0, 1}));
  auto const vsH = e.put(make_unique<stringvec>(stringvec{
    StringProduct("one"), StringProduct("two"), StringProduct("zero")}));

  // Making a map_vector is hard.
  auto mvs = make_unique<mapvec>();
  using key_t = mapvec::key_type;
  mvs->reserve(3);
  (*mvs)[key_t(0)] = StringProduct{"zero"};
  (*mvs)[key_t(11)] = StringProduct{"one"};
  (*mvs)[key_t(22)] = StringProduct{"two"};
  auto const mvsH = e.put(std::move(mvs), "mv");

  // Create the association objects.
  // Assns into vectors.
  auto a = std::make_unique<AssnsAB_t>();
  auto av = std::make_unique<AssnsVoid_t>();

  // Assns into map_vector.
  auto b = std::make_unique<AssnsAB_t>();
  auto bv = std::make_unique<AssnsVoid_t>();

  // addS will add to both x and xv a reference between slot1 of
  // product1 and slot2 of product2. The reference in x will have
  // associated data td.
  auto addS =
    [](auto& x, auto& xv, Ptr<int> p1, Ptr<StringProduct> p2, auto const td) {
      x->addSingle(p1, p2, td);
      xv->addSingle(p1, p2);
    };

  // We add associations in an order such that the associated data are
  // in alphabetical order.
  using PtrStr = Ptr<StringProduct>;
  addS(a, av, {viH, 1}, PtrStr{vsH, 2}, LiteAssnTestData{1, 2, "A"});
  addS(b, bv, {viH, 1}, PtrStr{mvsH, 0}, LiteAssnTestData{1, 0, "A"});

  addS(a, av, {viH, 2}, PtrStr{vsH, 0}, LiteAssnTestData{2, 0, "B"});
  addS(b, bv, {viH, 2}, PtrStr{mvsH, 11}, LiteAssnTestData{2, 11, "B"});

  addS(a, av, {viH, 0}, PtrStr{vsH, 1}, LiteAssnTestData{0, 1, "C"});
  addS(b, bv, {viH, 0}, PtrStr{mvsH, 22}, LiteAssnTestData{0, 22, "C"});

  auto aH = e.put(std::move(a));
  auto avH = e.put(std::move(av));
  auto bH = e.put(std::move(b), "mapvec");
  auto bvH = e.put(std::move(bv), "mapvec");

  auto am = make_unique<AssnsAB_t>(*aH);
  auto avm = make_unique<AssnsVoid_t>(*avH);
  auto bm = make_unique<AssnsAB_t>(*bH);
  auto bvm = make_unique<AssnsVoid_t>(*bvH);

  addS(am, avm, {viH, 1}, PtrStr{vsH, 2}, LiteAssnTestData{1, 2, "AA"});
  addS(bm, bvm, {viH, 1}, PtrStr{mvsH, 0}, LiteAssnTestData{1, 0, "AA"});

  e.put(std::move(am), "many");
  e.put(std::move(avm), "many");
  e.put(std::move(bm), "manymapvec");
  e.put(std::move(bvm), "manymapvec");
}

DEFINE_ART_MODULE(critictest::GalleryAssnsProducer)
