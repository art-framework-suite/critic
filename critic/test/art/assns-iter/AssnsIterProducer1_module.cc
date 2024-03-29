////////////////////////////////////////////////////////////////////////
// Class:       AssnsIterProducer1
// Plugin Type: producer (art v2_05_00)
// File:        AssnsIterProducer1_module.cc
//
// Generated at Thu Dec  8 12:20:56 2016 by Saba Sehrish using cetskelgen
// from cetlib version v1_21_00.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Principal/Event.h"
#include "fhiclcpp/fwd.h"

#include <vector>

namespace arttest {
  class AssnsIterProducer1;
}

using arttest::AssnsIterProducer1;

class arttest::AssnsIterProducer1 : public art::EDProducer {
public:
  using intvec_t = std::vector<int>;
  using strvec_t = std::vector<std::string>;
  using floatvec_t = std::vector<float>;

  explicit AssnsIterProducer1(fhicl::ParameterSet const& p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  AssnsIterProducer1(AssnsIterProducer1 const&) = delete;
  AssnsIterProducer1(AssnsIterProducer1&&) = delete;
  AssnsIterProducer1& operator=(AssnsIterProducer1 const&) = delete;
  AssnsIterProducer1& operator=(AssnsIterProducer1&&) = delete;

private:
  void produce(art::Event& e) override;
};

AssnsIterProducer1::AssnsIterProducer1(fhicl::ParameterSet const& p)
  : EDProducer{p}
{
  produces<intvec_t>();
  produces<strvec_t>();
  produces<floatvec_t>();
}

void
AssnsIterProducer1::produce(art::Event& e)
{
  auto vs = std::make_unique<strvec_t>(
    strvec_t{"one", "one-a", "two", "two-a", "three", "three-a"});
  auto vi = std::make_unique<intvec_t>(intvec_t{1, 2, 3});
  auto vf =
    std::make_unique<floatvec_t>(floatvec_t{1.0, 1.1, 2.0, 2.1, 3.0, 3.1});

  e.put(std::move(vs));
  e.put(std::move(vi));
  e.put(std::move(vf));
}

DEFINE_ART_MODULE(AssnsIterProducer1)
