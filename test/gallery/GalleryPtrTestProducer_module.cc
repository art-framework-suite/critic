#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "canvas/Persistency/Common/Assns.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "canvas/Utilities/InputTag.h"
#include "test/CriticTestObjects/LiteAssnTestData.h"
#include "test/CriticTestObjects/LitePtrTestProduct.h"
#include "test/CriticTestObjects/ToyProducts.h"

#include <memory>
#include <vector>

namespace fhicl {
  class ParameterSet;
}

namespace critictest {

  class GalleryPtrTestProducer : public art::EDProducer {
  public:

    typedef art::Assns<StringProduct, int, LiteAssnTestData> AssnsAB_t;
    typedef art::Assns<int, StringProduct, LiteAssnTestData> AssnsBA_t;

    explicit GalleryPtrTestProducer(fhicl::ParameterSet const &);
    virtual ~GalleryPtrTestProducer();

    virtual void produce(art::Event &);

  private:

    bool produceAssnStringInt_;
    bool produceAssnIntString_;
  };

  GalleryPtrTestProducer::GalleryPtrTestProducer(fhicl::ParameterSet const& pset) :
    produceAssnStringInt_(pset.get<bool>("produceAssnStringInt", false)),
    produceAssnIntString_(pset.get<bool>("produceAssnIntString", false))
  {
    produces<LitePtrTestProduct>();

    if (produceAssnStringInt_) {
      produces<AssnsAB_t>();
    }

    if (produceAssnIntString_) {
      produces<AssnsBA_t>();
    }
  }

  GalleryPtrTestProducer::~GalleryPtrTestProducer() {
  }

  void GalleryPtrTestProducer::produce(art::Event & event) {

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

    art::Handle<std::vector<StringProduct> > hVStringProduct;
    event.getByLabel(tagint1, hVStringProduct);

    if (produceAssnStringInt_) {
      std::unique_ptr<AssnsAB_t> assnsAB(new AssnsAB_t);
      assnsAB->addSingle(art::Ptr<StringProduct>(hVStringProduct,0), art::Ptr<int>(hint1, 1), LiteAssnTestData(0,1,"A"));
      assnsAB->addSingle(art::Ptr<StringProduct>(hVStringProduct,1), art::Ptr<int>(hint1, 2), LiteAssnTestData(1,2,"B"));
      event.put(std::move(assnsAB));
    }

    if (produceAssnIntString_) {
      std::unique_ptr<AssnsBA_t> assnsBA(new AssnsBA_t);
      assnsBA->addSingle(art::Ptr<int>(hint1, 2), art::Ptr<StringProduct>(hVStringProduct,2), LiteAssnTestData(1,2,"C"));
      assnsBA->addSingle(art::Ptr<int>(hint1, 1), art::Ptr<StringProduct>(hVStringProduct,0), LiteAssnTestData(1,2,"D"));
      event.put(std::move(assnsBA));
    }
  }
}

DEFINE_ART_MODULE(critictest::GalleryPtrTestProducer)
