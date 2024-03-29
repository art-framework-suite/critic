// Produces a MockClusterList starting from a vector<SimpleDerived>

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/test/TestObjects/MockCluster.h"
#include "art/test/TestObjects/ToyProducts.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "fhiclcpp/ParameterSet.h"

#include <memory>
#include <vector>

namespace arttest {
  class MockClusterListProducer;
}

using arttest::MockClusterListProducer;

// ----------------------------------------------------------------------

class arttest::MockClusterListProducer : public art::EDProducer {
public:
  using input_t = std::vector<arttest::SimpleDerived>;
  using product_t = arttest::MockClusterList;

  explicit MockClusterListProducer(fhicl::ParameterSet const& p)
    : EDProducer{p}
    , input_label_(p.get<std::string>("input_label"))
    , nvalues_(p.get<int>("nvalues"))
  {
    produces<product_t>();
  }

private:
  void produce(art::Event& e) override;

  std::string const input_label_;
  unsigned const nvalues_;
}; // MockClusterListProducer

// ----------------------------------------------------------------------

void
MockClusterListProducer::produce(art::Event& e)
{
  auto h = e.getHandle<input_t>({input_label_, "derived"});

  auto prod = std::make_unique<product_t>();
  arttest::MockCluster c1;
  c1.skew = 1;
  for (unsigned k = 0; k < (nvalues_ / 2); ++k) {
    c1.cells.emplace_back(h, k);
  }
  c1.eNum = e.id().event();
  prod->push_back(c1);

  arttest::MockCluster c2;
  c2.skew = 2;
  for (unsigned k = nvalues_ / 2; k < nvalues_; ++k) {
    c2.cells.emplace_back(h, k);
  }
  c2.eNum = e.id().event() + 1;
  prod->push_back(c2);

  e.put(std::move(prod));
}

DEFINE_ART_MODULE(MockClusterListProducer)
