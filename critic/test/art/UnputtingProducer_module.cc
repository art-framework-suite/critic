#include "art/Framework/Core/EDProducer.h"

#include <string>

namespace arttest {
  class UnputtingProducer;
}

//--------------------------------------------------------------------
//
// UnputtingProducer registers products, but does not put them into
// the Event.
//
class arttest::UnputtingProducer : public art::EDProducer {
public:
  explicit UnputtingProducer(fhicl::ParameterSet const&);

private:
  void produce(art::Event& e) override;
};

arttest::UnputtingProducer::UnputtingProducer(fhicl::ParameterSet const& ps)
  : art::EDProducer{ps}
{
  produces<std::string>();
}

void
arttest::UnputtingProducer::produce(art::Event&)
{
  // We don't put anything.
}

DEFINE_ART_MODULE(arttest::UnputtingProducer)
