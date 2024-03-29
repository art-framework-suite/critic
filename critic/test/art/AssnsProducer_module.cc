////////////////////////////////////////////////////////////////////////
// Class:       AssnsProducer
// Module Type: producer
// File:        AssnsProducer_module.cc
//
// Generated at Thu Jul  7 13:34:45 2011 by Chris Green using artmod
// from art v0_07_12.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Principal/Event.h"
#include "art/test/TestObjects/AssnTestData.h"
#include "canvas/Persistency/Common/Assns.h"
#include "canvas/Persistency/Common/Ptr.h"

#include "cetlib/map_vector.h"

#include <memory>
#include <string>
#include <vector>

using arttest::AssnTestData;
using std::make_unique;
using std::string;
using std::vector;

using uintvec = vector<size_t>;
using stringvec = vector<string>;
using mapvec = cet::map_vector<string>;

namespace art::test {
  class AssnsProducer : public EDProducer {
  public:
    explicit AssnsProducer(fhicl::ParameterSet const& p);

  private:
    void produce(Event& e) override;

    std::string const wantVoid_; // "ALL," "NONE," or "SOME."
    bool const wantMV_;          // Produce mapvector and derived Assns.
    bool const wantMany_;        // Produce many-to-many associations.
    bool const dinkVoid_;        // Distinguish ABV from ABD with extra entry.
    bool const wantAmbiguous_;   // Add an extra ABD to cause ambiguity.
  };
} // art::test

namespace {
  using AssnsABX_t = art::Assns<size_t, string, arttest::AssnTestData>;
  using AssnsABY_t = art::Assns<size_t, string, string>;
  using AssnsVoid_t = art::Assns<size_t, string>;
} // namespace

art::test::AssnsProducer::AssnsProducer(fhicl::ParameterSet const& ps)
  : EDProducer{ps}
  , wantVoid_{ps.get<std::string>("wantVoid", "ALL")}
  , wantMV_{ps.get<bool>("wantMV", true)}
  , wantMany_{ps.get<bool>("wantMany", true)}
  , dinkVoid_{ps.get<bool>("dinkVoid", false)}
  , wantAmbiguous_{ps.get<bool>("wantAmbiguous", false)}
{
  produces<uintvec>();
  produces<stringvec>();
  if (wantMV_) {
    produces<mapvec>("mv");
    produces<AssnsABX_t>("mapvec");
    if (wantMany_) {
      produces<AssnsABX_t>("manymapvec");
    }
    if (wantVoid_ != "NONE") {
      produces<AssnsVoid_t>("mapvec");
      if (wantMany_) {
        produces<AssnsVoid_t>("manymapvec");
      }
    }
  }
  produces<AssnsABX_t>();
  if (wantMany_) {
    produces<AssnsABX_t>("many");
  }
  if (wantVoid_ == "ALL") {
    produces<AssnsVoid_t>();
    if (wantMany_) {
      produces<AssnsVoid_t>("many");
    }
  }
  if (wantAmbiguous_) {
    produces<AssnsABY_t>();
  }
}

void
art::test::AssnsProducer::produce(Event& e)
{
  // Create the data products among which we will make associations.
  auto vui = make_unique<uintvec>(uintvec{2, 0, 1});
  auto vs = make_unique<stringvec>(stringvec{"one", "two", "zero"});

  // Making a map_vector is hard.
  auto mvs = make_unique<mapvec>();
  using key_t = mapvec::key_type;
  mvs->reserve(4);
  (*mvs)[key_t(0)] = "zero";
  (*mvs)[key_t(11)] = "one";
  (*mvs)[key_t(22)] = "two";

  // Extra members.
  if (dinkVoid_) {
    vui->emplace_back(4);
    vs->emplace_back("four");
    (*mvs)[key_t(33)] = "four";
  }

  // Insert containers and keep product IDs of the data products.
  auto vuiH = e.put(std::move(vui));
  auto vsH = e.put(std::move(vs));

  // Create the association objects.
  // Assns into vectors.
  auto a = std::make_unique<AssnsABX_t>();
  auto ay = std::make_unique<AssnsABY_t>();
  auto av = std::make_unique<AssnsVoid_t>();

  // addS will add to both x and xv a reference between slot1 of
  // product1 and slot2 of product2. The reference in x will have
  // associated data td.
  auto addS = [](auto& x, auto& xv, Ptr<size_t> p1, Ptr<string> p2, auto td) {
    x->addSingle(p1, p2, td);
    xv->addSingle(p1, p2);
  };

  // We add associations in an order such that the associated data are
  // in alphabetical order.
  addS(a, av, {vuiH, 1}, {vsH, 2}, AssnTestData(1, 2, "A"));
  ay->addSingle({vuiH, 1}, {vsH, 2}, "A");

  addS(a, av, {vuiH, 2}, {vsH, 0}, AssnTestData(2, 0, "B"));
  ay->addSingle({vuiH, 2}, {vsH, 0}, "B");

  addS(a, av, {vuiH, 0}, {vsH, 1}, AssnTestData(0, 1, "C"));
  ay->addSingle({vuiH, 0}, {vsH, 1}, "C");

  if (dinkVoid_) {
    av->addSingle({vuiH, 3}, {vsH, 3});
  }

  auto aH = e.put(std::move(a));

  auto am = make_unique<AssnsABX_t>(*aH);
  auto avm = make_unique<AssnsVoid_t>(*av);

  addS(am, avm, {vuiH, 1}, {vsH, 2}, AssnTestData(1, 2, "AA"));
  if (dinkVoid_) {
    avm->addSingle({vuiH, 3}, {vsH, 3});
  }

  if (wantMany_) {
    e.put(std::move(am), "many");
  }

  if (wantVoid_ == "ALL") {
    e.put(std::move(av));
    if (wantMany_) {
      e.put(std::move(avm), "many");
    }
  }
  if (wantAmbiguous_) {
    e.put(std::move(ay));
  }

  if (!wantMV_) {
    return;
  }

  // Assns into map_vector.
  auto b = std::make_unique<AssnsABX_t>();
  auto bv = std::make_unique<AssnsVoid_t>();

  auto mvsH = e.put(std::move(mvs), "mv");
  addS(b, bv, {vuiH, 1}, Ptr<string>{mvsH, 0}, AssnTestData(1, 0, "A"));
  addS(b, bv, {vuiH, 2}, Ptr<string>{mvsH, 11}, AssnTestData(2, 11, "B"));
  addS(b, bv, {vuiH, 0}, Ptr<string>{mvsH, 22}, AssnTestData(0, 22, "C"));
  if (dinkVoid_) {
    bv->addSingle({vuiH, 3}, Ptr<string>{mvsH, 33});
  }

  auto bH = e.put(std::move(b), "mapvec");

  auto bm = make_unique<AssnsABX_t>(*bH);
  auto bvm = make_unique<AssnsVoid_t>(*bv);
  addS(bm, bvm, {vuiH, 1}, Ptr<string>{mvsH, 0}, AssnTestData(1, 0, "AA"));
  if (dinkVoid_) {
    bvm->addSingle({vuiH, 3}, Ptr<string>{mvsH, 33});
  }

  if (wantMany_) {
    e.put(std::move(bm), "manymapvec");
  }
  if (wantVoid_ != "NONE") {
    e.put(std::move(bv), "mapvec");
    if (wantMany_) {
      e.put(std::move(bvm), "manymapvec");
    }
  }
}

DEFINE_ART_MODULE(art::test::AssnsProducer)
