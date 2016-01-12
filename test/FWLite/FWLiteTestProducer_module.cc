#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "test/TestObjects/ToyProducts.h"
#include "fhiclcpp/ParameterSet.h"

#include <memory>
#include <string>
#include <vector>

namespace arttest {
  class FWLiteTestProducer;
}

class arttest::FWLiteTestProducer : public art::EDProducer {
public:
  explicit FWLiteTestProducer(fhicl::ParameterSet const &p);
  virtual ~FWLiteTestProducer();

  virtual void produce(art::Event &e);

private:

  int value1_;
  int value2_;
  int value3_;

  std::string string1_;
  std::string string2_;
  std::string string3_;
};

arttest::FWLiteTestProducer::FWLiteTestProducer(fhicl::ParameterSet const& pset) :
  value1_( pset.get<int>("value1") ),
  value2_( pset.get<int>("value2") ),
  value3_( pset.get<int>("value3") ),
  string1_( pset.get<std::string>("string1") ),
  string2_( pset.get<std::string>("string2") ),
  string3_( pset.get<std::string>("string3") ) {

  produces<IntProduct>();
  produces<IntProduct>("i2");
  produces<IntProduct>("i3");

  produces<StringProduct>();
  produces<StringProduct>("i2");
  produces<StringProduct>("i3");

  produces<IntProduct>("eventID");

  produces<std::vector<int> >();
  produces<std::vector<arttest::SimpleDerived> >("SimpleDerived");
  produces<std::vector<int> >("willBeDropped");
}

arttest::FWLiteTestProducer::~FWLiteTestProducer() {
}

void arttest::FWLiteTestProducer::produce(art::Event & event) {
  if(value1_ > 0) {
    event.put(std::unique_ptr<IntProduct>(new IntProduct(value1_)));
  }
  if(value2_ > 0) {
    event.put(std::unique_ptr<IntProduct>(new IntProduct(value2_)), "i2");
  }
  if(value3_ > 0) {
    event.put(std::unique_ptr<IntProduct>(new IntProduct(value3_)), "i3");
  }

  if(string1_ != "noPut") {
    event.put(std::unique_ptr<StringProduct>(new StringProduct(string1_)));
  }
  if(string2_ != "noPut") {
    event.put(std::unique_ptr<StringProduct>(new StringProduct(string2_)), "i2");
  }
  if(string3_ != "noPut") {
    event.put(std::unique_ptr<StringProduct>(new StringProduct(string3_)), "i3");
  }
  event.put(std::unique_ptr<IntProduct>(new IntProduct(event.event())), "eventID");

  std::unique_ptr<std::vector<int> > vecInt(new std::vector<int>);
  vecInt->push_back(value1_ + event.event());
  vecInt->push_back(value2_);
  vecInt->push_back(value3_);
  event.put(std::move(vecInt));

  std::unique_ptr<std::vector<arttest::SimpleDerived> > vecSimpleDerived(new std::vector<arttest::SimpleDerived>);
  arttest::SimpleDerived sd1;
  sd1.key = value1_;
  sd1.value = 0.0;
  vecSimpleDerived->push_back(sd1);
  arttest::SimpleDerived sd2;
  sd2.key = value2_;
  sd2.value = 0.0;
  vecSimpleDerived->push_back(sd2);
  arttest::SimpleDerived sd3;
  sd3.key = value3_;
  sd3.value = 0.0;
  vecSimpleDerived->push_back(sd3);
  event.put(std::move(vecSimpleDerived), "SimpleDerived");

  std::unique_ptr<std::vector<int> > vecIntD(new std::vector<int>);
  vecIntD->push_back(value1_ + event.event());
  vecIntD->push_back(value2_);
  vecIntD->push_back(value3_);
  event.put(std::move(vecIntD), "willBeDropped");
}

DEFINE_ART_MODULE(arttest::FWLiteTestProducer)
