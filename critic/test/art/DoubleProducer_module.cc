//--------------------------------------------------------------------
//
// Produces an DoubleProduct instance.
//
//--------------------------------------------------------------------

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Principal/Event.h"
#include "art/test/TestObjects/ToyProducts.h"
#include "fhiclcpp/ParameterSet.h"

#include <memory>

namespace arttest {
  class DoubleProducer;
}

class arttest::DoubleProducer : public art::EDProducer {
public:
  explicit DoubleProducer(fhicl::ParameterSet const& p)
    : EDProducer{p}, value_(p.get<double>("dvalue"))
  {
    produces<DoubleProduct>();
  }

private:
  void produce(art::Event& e) override;

  double const value_;
}; // DoubleProducer

void
arttest::DoubleProducer::produce(art::Event& e)
{
  e.put(std::make_unique<DoubleProduct>(value_));
}

DEFINE_ART_MODULE(arttest::DoubleProducer)
