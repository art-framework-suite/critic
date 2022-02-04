////////////////////////////////////////////////////////////////////////
// Class:       DeferredIsReadyWithAssnsProducer
// Module Type: producer
// File:        DeferredIsReadyWithAssnsProducer_module.cc
//
// Generated at Wed Nov  6 17:21:54 2013 by Christopher Green using artmod
// from cetpkgsupport v1_04_02.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/test/TestObjects/AssnTestData.h"
#include "canvas/Persistency/Common/Assns.h"
#include "fhiclcpp/fwd.h"

#include <memory>

namespace arttest {
  class DeferredIsReadyWithAssnsProducer;
}

class arttest::DeferredIsReadyWithAssnsProducer : public art::EDProducer {
public:
  explicit DeferredIsReadyWithAssnsProducer(fhicl::ParameterSet const& p);

private:
  void produce(art::Event& e) override;
};

arttest::DeferredIsReadyWithAssnsProducer::DeferredIsReadyWithAssnsProducer(
  fhicl::ParameterSet const& ps)
  : EDProducer{ps}
{
  produces<std::vector<std::string>>();
  produces<std::vector<size_t>>();
  produces<art::Assns<std::string, size_t, arttest::AssnTestData>>();
}

void
arttest::DeferredIsReadyWithAssnsProducer::produce(art::Event& e)
{
  std::vector<std::string> letters{"f", "e", "d", "c", "b", "a"};
  std::vector<size_t> nums{0, 1, 2, 3, 4, 5};
  auto vs = std::make_unique<std::vector<std::string>>(move(letters));
  auto vi = std::make_unique<std::vector<size_t>>(move(nums));

  auto sz = vs->size();

  auto vsh = e.put(move(vs));
  auto vih = e.put(move(vi));

  auto asid =
    std::make_unique<art::Assns<std::string, size_t, arttest::AssnTestData>>();
  for (size_t i = 0; i != sz; ++i) {
    asid->addSingle({vsh, sz - i - 1}, {vih, i}, {i, i, "Ethel"});
  }
  e.put(move(asid));
}

DEFINE_ART_MODULE(arttest::DeferredIsReadyWithAssnsProducer)
