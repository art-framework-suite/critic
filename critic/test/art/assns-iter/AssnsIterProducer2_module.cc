////////////////////////////////////////////////////////////////////////
// Class:       AssnsIterProducer2
// Plugin Type: producer (art v2_05_00)
// File:        AssnsIterProducer2_module.cc
//
// Generated at Tue Dec 13 14:04:05 2016 by Saba Sehrish using cetskelgen
// from cetlib version v1_21_00.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Persistency/Common/PtrMaker.h"
#include "canvas/Persistency/Common/Assns.h"
#include "fhiclcpp/ParameterSet.h"

namespace arttest {
  class AssnsIterProducer2;
}

using arttest::AssnsIterProducer2;

class arttest::AssnsIterProducer2 : public art::EDProducer {
public:
  explicit AssnsIterProducer2(fhicl::ParameterSet const& p);

private:
  void produce(art::Event& e) override;

  std::string const fInputLabel;
};

AssnsIterProducer2::AssnsIterProducer2(fhicl::ParameterSet const& p)
  : EDProducer{p}, fInputLabel(p.get<std::string>("input_label"))
{
  produces<art::Assns<int, float, std::string>>();
}

void
AssnsIterProducer2::produce(art::Event& e)
{
  auto ih = e.getHandle<std::vector<int>>(fInputLabel);
  auto sh = e.getHandle<std::vector<std::string>>(fInputLabel);
  auto fh = e.getHandle<std::vector<float>>(fInputLabel);

  art::PtrMaker<int> make_intptr(e, ih.id());
  art::PtrMaker<float> make_floatptr(e, fh.id());

  auto assns = std::make_unique<art::Assns<int, float, std::string>>();
  for (size_t i = 0; i < 3; ++i) {
    auto p1 = make_intptr(i);
    for (size_t j = 0; j < 2; ++j) {
      auto p2 = make_floatptr(i * 2 + j);
      assns->addSingle(p1, p2, sh->at(i * 2 + j));
    }
  }
  e.put(std::move(assns));
}

DEFINE_ART_MODULE(AssnsIterProducer2)
