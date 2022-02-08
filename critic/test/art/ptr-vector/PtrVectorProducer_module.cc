// ======================================================================
//
// Produces an PtrVector instance.
//
// ======================================================================

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "canvas/Persistency/Common/PtrVector.h"
#include "fhiclcpp/ParameterSet.h"

#include <memory>
#include <vector>

namespace art::test {
  class PtrVectorProducer : public EDProducer {
  public:
    explicit PtrVectorProducer(fhicl::ParameterSet const& p)
      : EDProducer{p}, input_label_(p.get<std::string>("input_label"))
    {
      produces<PtrVector<int>>();
    }

  private:
    void produce(Event& e) override;

    std::string const input_label_;
  }; // PtrVectorProducer
}

void
art::test::PtrVectorProducer::produce(Event& e)
{
  auto h = e.getHandle<std::vector<int>>(input_label_);

  auto prod = std::make_unique<PtrVector<int>>();
  for (int k = 0; k != 8; ++k) {
    prod->emplace_back(h, 7 - k);
  }
  prod->sort();

  e.put(move(prod));
}

DEFINE_ART_MODULE(art::test::PtrVectorProducer)
