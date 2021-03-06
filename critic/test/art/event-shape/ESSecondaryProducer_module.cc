////////////////////////////////////////////////////////////////////////
// Class:       ESSecondaryProducer
// Module Type: producer
// File:        ESSecondaryProducer_module.cc
//
// Generated at Mon Feb  3 11:00:24 2014 by Christopher Green using artmod
// from cetpkgsupport v1_05_03.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "critic/test/art/event-shape/ESPtrSimple.h"
#include "fhiclcpp/ParameterSet.h"

#include <memory>

namespace arttest {
  class ESSecondaryProducer;
}

class arttest::ESSecondaryProducer : public art::EDProducer {
public:
  explicit ESSecondaryProducer(fhicl::ParameterSet const& p);

private:
  void produce(art::Event& e) override;

  std::string const label_;
  size_t const index_;
};

arttest::ESSecondaryProducer::ESSecondaryProducer(fhicl::ParameterSet const& p)
  : EDProducer{p}
  , label_(p.get<std::string>("moduleLabel"))
  , index_(p.get<size_t>("index"))
{
  produces<arttest::ESPtrSimple>();
  produces<arttest::IntProduct>();
}

void
arttest::ESSecondaryProducer::produce(art::Event& e)
{
  auto h = e.getValidHandle<arttest::VSimpleProduct>(label_);
  if (!(h->size() > index_)) {
    throw art::Exception(art::errors::Configuration)
      << "Specified index " << index_ << "is invalid for loaded product.";
  }
  e.put(std::make_unique<arttest::ESPtrSimple>(
    arttest::ESPtrSimple{art::Ptr{h, index_}}));
  e.put(std::make_unique<arttest::IntProduct>(static_cast<int>(h->size())));
}

DEFINE_ART_MODULE(arttest::ESSecondaryProducer)
