#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"

namespace art::test {
  class FailingProducer;
}

//--------------------------------------------------------------------
// Every call to FailingProducer::produce throws an art::Exception
//
class art::test::FailingProducer : public EDProducer {
public:
  struct Config {};
  using Parameters = Table<Config>;
  explicit FailingProducer(Parameters const& ps) : EDProducer{ps}
  {
    produces<int>();
  }

private:
  void produce(Event&) override;
};

void
art::test::FailingProducer::produce(Event&)
{
  // We throw an exception with a configurable action.
  throw Exception(errors::ProductNotFound)
    << "Intentional 'ProductNotFound' exception for testing purposes\n";
}

DEFINE_ART_MODULE(art::test::FailingProducer)
