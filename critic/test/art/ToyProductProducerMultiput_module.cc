//--------------------------------------------------------------------
//
// Produces a ToyProductProducer instance.
//
//--------------------------------------------------------------------

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "canvas/Persistency/Provenance/BranchType.h"
#include "critic/test/art/RunTimeProduces.h"

#include "fhiclcpp/types/Atom.h"

#include "art/test/TestObjects/ToyProducts.h"

using namespace fhicl;

namespace art::test {
  class ToyProductProducerMultiput;
}

using arttest::IntProduct;

class art::test::ToyProductProducerMultiput : public EDProducer {
  BranchType const branchType_;

public:
  struct Config {
    Atom<unsigned long> branchType{Name("branchType")};
  };
  using Parameters = Table<Config>;
  explicit ToyProductProducerMultiput(Parameters const& p)
    : EDProducer{p}, branchType_{BranchType(p().branchType())}
  {
    run_time_produces<IntProduct>(producesCollector(), branchType_);
  }

private:
  void
  produce(Event& e) override
  {
    if (branchType_ != InEvent)
      return;
    e.put(std::make_unique<IntProduct>(1));
    e.put(std::make_unique<IntProduct>(2));
  }

  void
  endSubRun(SubRun& sr) override
  {
    if (branchType_ != InSubRun)
      return;
    sr.put(std::make_unique<IntProduct>(3), subRunFragment());
    sr.put(std::make_unique<IntProduct>(4), subRunFragment());
  }

  void
  endRun(Run& r) override
  {
    if (branchType_ != InRun)
      return;
    r.put(std::make_unique<IntProduct>(5), runFragment());
    r.put(std::make_unique<IntProduct>(6), runFragment());
  }
};

DEFINE_ART_MODULE(art::test::ToyProductProducerMultiput)
