////////////////////////////////////////////////////////////////////////
// Class:       DropTestParentageFaker
// Module Type: producer
// File:        DropTestParentageFaker_module.cc
//
// Synopsis:    Make sure that one of the products produced by the
//              PtrmvProducer module is registered as a parent to
//              improve test coverage.
//
// Generated at Thu Jan  5 17:44:19 2012 by Chris Green using artmod
// from art v1_00_06.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Principal/Event.h"
#include "canvas/Persistency/Common/Ptr.h"

#include <memory>

namespace arttest {
  class DropTestParentageFaker;
}

class arttest::DropTestParentageFaker : public art::EDProducer {
public:
  explicit DropTestParentageFaker(fhicl::ParameterSet const& p);

private:
  void produce(art::Event& e) override;

  std::string const inputLabel_;
};

arttest::DropTestParentageFaker::DropTestParentageFaker(
  fhicl::ParameterSet const& p)
  : EDProducer{p}, inputLabel_(p.get<std::string>("input_label"))
{
  produces<std::string>();
}

void
arttest::DropTestParentageFaker::produce(art::Event& e)
{
  // Force the art::Ptr to be a parent of our child.
  e.getHandle<art::Ptr<std::string>>(inputLabel_);
  e.put(std::make_unique<std::string>("Child"));
}

DEFINE_ART_MODULE(arttest::DropTestParentageFaker)
