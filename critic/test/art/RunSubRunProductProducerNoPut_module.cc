//--------------------------------------------------------------------
//
// Produces an RunSubRunProducerNoPut instance.
//
//--------------------------------------------------------------------

#include "art/Framework/Core/EDProducer.h"

namespace art {
  class Event;
}
namespace fhicl {
  class ParameterSet;
}

namespace arttest {

  class RunSubRunProducerNoPut : public art::EDProducer {
  public:
    explicit RunSubRunProducerNoPut(fhicl::ParameterSet const& ps)
      : EDProducer{ps}
    {
      produces<int, art::InRun>("bgnRun");
      produces<int, art::InSubRun>("bgnSubRun");
    }

  private:
    // We are not putting the products on the Run or SubRun -- i.e. no
    // overrides for beginRun and beginSubRun.
    void
    produce(art::Event&) override
    {}
  };

} // namespace arttest

DEFINE_ART_MODULE(arttest::RunSubRunProducerNoPut)
