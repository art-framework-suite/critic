////////////////////////////////////////////////////////////////////////
// Class:       AssnsProducer1
// Plugin Type: producer (art v2_05_00)
// File:        AssnsProducer1_module.cc
//
// Generated at Thu Dec  8 12:20:56 2016 by Saba Sehrish using cetskelgen
// from cetlib version v1_21_00.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Principal/Event.h"
#include "fhiclcpp/fwd.h"

namespace arttest {
  class AssnsProducer1;
}

using arttest::AssnsProducer1;

class arttest::AssnsProducer1 : public art::EDProducer {
public:
  using intvec_t = std::vector<int>;
  using strvec_t = std::vector<std::string>;

  explicit AssnsProducer1(fhicl::ParameterSet const& p);

  // Plugins should not be copied or assigned.
  AssnsProducer1(AssnsProducer1 const&) = delete;
  AssnsProducer1(AssnsProducer1&&) = delete;
  AssnsProducer1& operator=(AssnsProducer1 const&) = delete;
  AssnsProducer1& operator=(AssnsProducer1&&) = delete;

private:
  void produce(art::Event& e) override;
};

AssnsProducer1::AssnsProducer1(fhicl::ParameterSet const& ps) : EDProducer{ps}
{
  produces<intvec_t>();
  produces<strvec_t>();
}

void
AssnsProducer1::produce(art::Event& e)
{
  auto vs = std::make_unique<strvec_t>(
    strvec_t{"one", "one-a", "two", "two-a", "three", "three-a"});
  auto vi = std::make_unique<intvec_t>(intvec_t{1, 2, 3});

  e.put(std::move(vs));
  e.put(std::move(vi));
}

DEFINE_ART_MODULE(AssnsProducer1)
