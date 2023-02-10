////////////////////////////////////////////////////////////////////////
// Class:       MixProducer
// Module Type: producer
// File:        MixProducer_module.cc
//
// Generated at Wed May 11 10:14:20 2011 by Chris Green using artmod
// from art v0_06_02.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/SharedProducer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/test/TestObjects/ProductWithPtrs.h"
#include "art/test/TestObjects/ToyProducts.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "canvas/Persistency/Common/PtrVector.h"
#include "cetlib/map_vector.h"

#include <atomic>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using arttest::IntProduct;
using arttest::ProductWithPtrs;

namespace art::test {
  class MixProducer : public SharedProducer {
  public:
    struct Config {};
    using Parameters = Table<Config>;
    explicit MixProducer(Parameters const& p, ProcessingFrame const&);

  private:
    void produce(Event& e, ProcessingFrame const&) override;
    void endSubRun(SubRun& sr, ProcessingFrame const&) override;
    void endRun(Run& r, ProcessingFrame const&) override;

    using mv_t = cet::map_vector<unsigned int>;
    using mvv_t = mv_t::value_type;
    using mvm_t = mv_t::mapped_type;

    // Declare member data here.
    std::atomic<size_t> eventCounter_{};
    std::atomic<size_t> subrunCounter_{};
    std::atomic<size_t> runCounter_{};
  };
}

art::test::MixProducer::MixProducer(Parameters const& p, ProcessingFrame const&)
  : SharedProducer{p}
{
  async<InEvent>();

  produces<double>("doubleLabel");
  produces<IntProduct>("IntProductLabel");
  produces<IntProduct>("SpottyProductLabel");
  produces<std::string>("stringLabel");
  produces<std::vector<double>>("doubleCollectionLabel");
  produces<std::vector<Ptr<double>>>("doubleVectorPtrLabel");
  produces<PtrVector<double>>("doublePtrVectorLabel");
  produces<ProductWithPtrs>("ProductWithPtrsLabel");
  produces<mv_t>("mapVectorLabel");
  produces<std::vector<Ptr<mvv_t>>>("intVectorPtrLabel");
  produces<double, InSubRun>("DoubleSRLabel");
  produces<double, InRun>("DoubleRLabel");
}

void
art::test::MixProducer::produce(Event& e, ProcessingFrame const&)
{
  ++eventCounter_;

  // double
  e.put(std::make_unique<double>(eventCounter_), "doubleLabel");

  // IntProduct
  e.put(std::make_unique<IntProduct>(eventCounter_ + 1000000),
        "IntProductLabel");

  // SpottyProduct
  if (e.event() % 100) {
    e.put(std::make_unique<IntProduct>(eventCounter_), "SpottyProductLabel");
  }

  // std::string
  std::ostringstream s;
  s << "string value: " << std::setfill('0') << std::setw(7) << eventCounter_
    << "\n";
  e.put(std::make_unique<std::string>(s.str()), "stringLabel");

  // 1. std::vector<double>
  // 2. std::vector<Ptr<double>>
  // 3. PtrVector<double>
  // 4. ProductWithPtrs

  auto coll = std::make_unique<std::vector<double>>();
  coll->reserve(10);
  for (size_t i = 1; i < 11; ++i) {
    coll->push_back(i + 10 * (eventCounter_ - 1));
  }
  auto collH = e.put(std::move(coll), "doubleCollectionLabel"); // 1.

  auto vpd = std::make_unique<std::vector<Ptr<double>>>();
  vpd->reserve(3);
  vpd->emplace_back(collH, 0);
  vpd->emplace_back(collH, 4);
  vpd->emplace_back(collH, 8);

  auto pvd = std::make_unique<PtrVector<double>>();
  pvd->reserve(3);
  pvd->emplace_back(collH, 1);
  pvd->emplace_back(collH, 5);
  pvd->emplace_back(collH, 9);

  auto pwp = std::make_unique<ProductWithPtrs>(*pvd, *vpd, ProductPtr{collH});
  e.put(std::move(vpd), "doubleVectorPtrLabel"); // 2.
  e.put(std::move(pvd), "doublePtrVectorLabel"); // 3.
  e.put(std::move(pwp), "ProductWithPtrsLabel"); // 4.

  // map_vector, .
  auto mv = std::make_unique<mv_t>();
  static size_t constexpr mv_size{5};
  mv->reserve(mv_size);
  for (size_t i = 0; i < mv_size; ++i) {
    (*mv)[cet::map_vector_key(
      static_cast<mvm_t>(1 + i * 2 + 10 * (eventCounter_ - 1)))] =
      (eventCounter_ - 1) * mv_size + i + 1;
  }

  // Ptr into map_vector.
  auto mvvp = std::make_unique<std::vector<Ptr<mvv_t>>>();
  mvvp->reserve(mv_size);
  ProductID const mvID{e.getProductID<mv_t>("mapVectorLabel")};
  mvvp->emplace_back(mvID, 10 * (eventCounter_ - 1) + 7, e.productGetter(mvID));
  mvvp->emplace_back(mvID, 10 * (eventCounter_ - 1) + 1, e.productGetter(mvID));
  mvvp->emplace_back(mvID, 10 * (eventCounter_ - 1) + 3, e.productGetter(mvID));
  mvvp->emplace_back(mvID, 10 * (eventCounter_ - 1) + 9, e.productGetter(mvID));
  mvvp->emplace_back(mvID, 10 * (eventCounter_ - 1) + 5, e.productGetter(mvID));

  e.put(std::move(mvvp), "intVectorPtrLabel");
  e.put(std::move(mv), "mapVectorLabel"); // Note we're putting these into the
                                          // event in the "wrong" order.
}

void
art::test::MixProducer::endSubRun(SubRun& sr, ProcessingFrame const&)
{
  ++subrunCounter_;
  sr.put(std::make_unique<double>(subrunCounter_),
         "DoubleSRLabel",
         subRunFragment());
}

void
art::test::MixProducer::endRun(Run& r, ProcessingFrame const&)
{
  ++runCounter_;
  r.put(std::make_unique<double>(runCounter_), "DoubleRLabel", runFragment());
}

DEFINE_ART_MODULE(art::test::MixProducer)
