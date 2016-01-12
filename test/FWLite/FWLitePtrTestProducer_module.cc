#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "canvas/Utilities/InputTag.h"
#include "test/TestObjects/LitePtrTestProduct.h"
#include "test/TestObjects/ToyProducts.h"

#include <memory>
#include <vector>

namespace fhicl {
  class ParameterSet;
}

namespace arttest {
  class FWLitePtrTestProducer;
}

class arttest::FWLitePtrTestProducer : public art::EDProducer {
public:
  explicit FWLitePtrTestProducer(fhicl::ParameterSet const &);
  virtual ~FWLitePtrTestProducer();

  virtual void produce(art::Event &);

private:

};

arttest::FWLitePtrTestProducer::FWLitePtrTestProducer(fhicl::ParameterSet const&) {
  produces<LitePtrTestProduct>();
}

arttest::FWLitePtrTestProducer::~FWLitePtrTestProducer() {
}

void arttest::FWLitePtrTestProducer::produce(art::Event & event) {

  std::unique_ptr<LitePtrTestProduct > testProduct(new LitePtrTestProduct);

  art::InputTag tagint1("m1", "", "PROD1");
  art::Handle<std::vector<int> > hint1;
  event.getByLabel(tagint1, hint1);
  testProduct->ptrInt1 = art::Ptr<int>(hint1, 0);

  art::InputTag tagint2("m1", "", "PROD2");
  art::Handle<std::vector<int> > hint2;
  event.getByLabel(tagint2, hint2);
  testProduct->ptrInt2 = art::Ptr<int>(hint2, 1);

  art::InputTag tagint3("m1", "", "PROD3");
  art::Handle<std::vector<int> > hint3;
  event.getByLabel(tagint3, hint3);
  testProduct->ptrInt3 = art::Ptr<int>(hint3, 2);

  art::InputTag tagSimpleDerived1("m1", "SimpleDerived", "PROD1");
  art::InputTag tagSimpleDerived2("m1", "SimpleDerived", "PROD2");
  art::InputTag tagSimpleDerived3("m1", "SimpleDerived", "PROD3");
  art::Handle<std::vector<SimpleDerived> > hSimpleDerived1;
  art::Handle<std::vector<SimpleDerived> > hSimpleDerived2;
  art::Handle<std::vector<SimpleDerived> > hSimpleDerived3;
  event.getByLabel(tagSimpleDerived1, hSimpleDerived1);
  event.getByLabel(tagSimpleDerived2, hSimpleDerived2);
  event.getByLabel(tagSimpleDerived3, hSimpleDerived3);

  testProduct->ptrSimple1 = art::Ptr<Simple>(hSimpleDerived1, 0);
  testProduct->ptrSimple2 = art::Ptr<Simple>(hSimpleDerived2, 1);
  testProduct->ptrSimple3 = art::Ptr<Simple>(hSimpleDerived3, 2);

  testProduct->ptrSimpleDerived1 = art::Ptr<SimpleDerived>(hSimpleDerived1, 0);
  testProduct->ptrSimpleDerived2 = art::Ptr<SimpleDerived>(hSimpleDerived2, 1);
  testProduct->ptrSimpleDerived3 = art::Ptr<SimpleDerived>(hSimpleDerived3, 2);

  testProduct->ptrVectorInt.push_back(art::Ptr<int>(hint1, 0));
  testProduct->ptrVectorInt.push_back(art::Ptr<int>(hint1, 2));

  testProduct->ptrVectorSimple.push_back(art::Ptr<Simple>(hSimpleDerived1, 0));
  testProduct->ptrVectorSimple.push_back(art::Ptr<Simple>(hSimpleDerived1, 2));

  testProduct->ptrVectorSimpleDerived.push_back(art::Ptr<SimpleDerived>(hSimpleDerived1, 0));
  testProduct->ptrVectorSimpleDerived.push_back(art::Ptr<SimpleDerived>(hSimpleDerived1, 2));

  art::InputTag tagDrop("m1", "willBeDropped", "PROD1");
  art::Handle<std::vector<int> > hDrop;
  event.getByLabel(tagDrop, hDrop);
  testProduct->ptrIntoContainerToBeDropped = art::Ptr<int>(hDrop, 1);
  testProduct->nullPtr = art::Ptr<int>(hint1.id());
  testProduct->nullDroppedPtr = art::Ptr<int>(hDrop.id());
  testProduct->invalidPtr = art::Ptr<int>();

  event.put(std::move(testProduct));
}

DEFINE_ART_MODULE(arttest::FWLitePtrTestProducer)
