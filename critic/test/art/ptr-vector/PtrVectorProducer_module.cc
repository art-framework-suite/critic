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

#include <iostream>
#include <memory>
#include <vector>

namespace arttest {
  class PtrVectorProducer;
}

using arttest::PtrVectorProducer;

// ----------------------------------------------------------------------

class arttest::PtrVectorProducer : public art::EDProducer {
public:
  using intvector_t = std::vector<int>;
  using product_t = art::PtrVector<int>;

  explicit PtrVectorProducer(fhicl::ParameterSet const& p)
    : EDProducer{p}, input_label_(p.get<std::string>("input_label"))
  {
    produces<product_t>();
  }

private:
  void produce(art::Event& e) override;

  std::string const input_label_;

}; // PtrVectorProducer

// ----------------------------------------------------------------------

void
PtrVectorProducer::produce(art::Event& e)
{
  std::cerr << "PtrVectorProducer::produce is running!\n";

  auto h = e.getHandle<intvector_t>(input_label_);

  auto prod = std::make_unique<product_t>();
  for (int k = 0; k != 8; ++k) {
    art::Ptr<int> p(h, 7 - k);
    prod->push_back(p);
  }
  prod->sort();

  e.put(move(prod));
}

// ----------------------------------------------------------------------

DEFINE_ART_MODULE(PtrVectorProducer)

// ======================================================================
