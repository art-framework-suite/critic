////////////////////////////////////////////////////////////////////////
// Class:       AssnsProducer2
// Plugin Type: producer (art v2_05_00)
// File:        AssnsProducer2_module.cc
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

namespace art::test {
  class AssnsProducer2 : public EDProducer {
  public:
    explicit AssnsProducer2(fhicl::ParameterSet const& p);

  private:
    void produce(Event& e) override;

    std::string const fInputLabel;
  };
}

art::test::AssnsProducer2::AssnsProducer2(fhicl::ParameterSet const& p)
  : EDProducer{p}, fInputLabel(p.get<std::string>("input_label"))
{
  produces<Assns<int, std::string>>();
}

void
art::test::AssnsProducer2::produce(Event& e)
{
  auto ih = e.getHandle<std::vector<int>>(fInputLabel);
  auto sh = e.getHandle<std::vector<std::string>>(fInputLabel);

  PtrMaker<int> make_intptr(e, ih.id());
  PtrMaker<std::string> make_strptr(e, sh.id());

  auto assns = std::make_unique<Assns<int, std::string>>();
  for (size_t i = 0; i < 3; ++i) {
    auto p1 = make_intptr(i);
    for (size_t j = 0; j < 2; ++j) {
      auto p2 = make_strptr(i * 2 + j);
      assns->addSingle(p1, p2);
    }
  }
  e.put(std::move(assns));
}

DEFINE_ART_MODULE(art::test::AssnsProducer2)
