#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/test/TestObjects/ToyProducts.h"
#include "canvas/Persistency/Common/Assns.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "canvas/Utilities/InputTag.h"
#include "critic/test/CriticTestObjects/LiteAssnTestData.h"
#include "critic/test/CriticTestObjects/LitePtrTestProduct.h"

#include <memory>
#include <vector>

namespace fhicl {
  class ParameterSet;
}

using arttest::Simple;
using arttest::SimpleDerived;
using arttest::StringProduct;

namespace critictest {

  class GalleryPtrTestProducer : public art::EDProducer {
  public:
    using AssnsAB_t = art::Assns<StringProduct, int, LiteAssnTestData>;
    using AssnsBA_t = art::Assns<int, StringProduct, LiteAssnTestData>;

    explicit GalleryPtrTestProducer(fhicl::ParameterSet const&);

  private:
    void produce(art::Event&) override;

    bool const produceAssnStringInt_;
    bool const produceAssnIntString_;
  };

  GalleryPtrTestProducer::GalleryPtrTestProducer(
    fhicl::ParameterSet const& pset)
    : produceAssnStringInt_(pset.get<bool>("produceAssnStringInt", false))
    , produceAssnIntString_(pset.get<bool>("produceAssnIntString", false))
  {
    produces<LitePtrTestProduct>();

    if (produceAssnStringInt_) {
      produces<AssnsAB_t>();
    }

    if (produceAssnIntString_) {
      produces<AssnsBA_t>();
    }
  }

  void
  GalleryPtrTestProducer::produce(art::Event& event)
  {
    auto testProduct = std::make_unique<LitePtrTestProduct>();

    art::InputTag const tagint1{"m1", "", "PROD1"};
    art::InputTag const tagint2{"m1", "", "PROD2"};
    art::InputTag const tagint3{"m1", "", "PROD3"};
    auto const& hint1 = event.getValidHandle<std::vector<int>>(tagint1);
    auto const& hint2 = event.getValidHandle<std::vector<int>>(tagint2);
    auto const& hint3 = event.getValidHandle<std::vector<int>>(tagint3);
    testProduct->ptrInt1 = art::Ptr<int>{hint1, 0};
    testProduct->ptrInt2 = art::Ptr<int>{hint2, 1};
    testProduct->ptrInt3 = art::Ptr<int>{hint3, 2};

    art::InputTag const tagSimpleDerived1{"m1", "SimpleDerived", "PROD1"};
    art::InputTag const tagSimpleDerived2{"m1", "SimpleDerived", "PROD2"};
    art::InputTag const tagSimpleDerived3{"m1", "SimpleDerived", "PROD3"};
    auto const& hSimpleDerived1 =
      event.getValidHandle<std::vector<SimpleDerived>>(tagSimpleDerived1);
    auto const& hSimpleDerived2 =
      event.getValidHandle<std::vector<SimpleDerived>>(tagSimpleDerived2);
    auto const& hSimpleDerived3 =
      event.getValidHandle<std::vector<SimpleDerived>>(tagSimpleDerived3);
    testProduct->ptrSimple1 = art::Ptr<Simple>{hSimpleDerived1, 0};
    testProduct->ptrSimple2 = art::Ptr<Simple>{hSimpleDerived2, 1};
    testProduct->ptrSimple3 = art::Ptr<Simple>{hSimpleDerived3, 2};

    testProduct->ptrSimpleDerived1 =
      art::Ptr<SimpleDerived>{hSimpleDerived1, 0};
    testProduct->ptrSimpleDerived2 =
      art::Ptr<SimpleDerived>{hSimpleDerived2, 1};
    testProduct->ptrSimpleDerived3 =
      art::Ptr<SimpleDerived>{hSimpleDerived3, 2};

    testProduct->ptrVectorInt.push_back(art::Ptr<int>{hint1, 0});
    testProduct->ptrVectorInt.push_back(art::Ptr<int>{hint1, 2});

    testProduct->ptrVectorSimple.push_back(
      art::Ptr<Simple>{hSimpleDerived1, 0});
    testProduct->ptrVectorSimple.push_back(
      art::Ptr<Simple>{hSimpleDerived1, 2});

    testProduct->ptrVectorSimpleDerived.push_back(
      art::Ptr<SimpleDerived>{hSimpleDerived1, 0});
    testProduct->ptrVectorSimpleDerived.push_back(
      art::Ptr<SimpleDerived>{hSimpleDerived1, 2});

    art::InputTag const tagDrop{"m1", "willBeDropped", "PROD1"};
    auto const& hDrop = event.getValidHandle<std::vector<int>>(tagDrop);
    testProduct->ptrIntoContainerToBeDropped = art::Ptr<int>(hDrop, 1);
    testProduct->nullPtr = art::Ptr<int>(hint1.id());
    testProduct->nullDroppedPtr = art::Ptr<int>(hDrop.id());
    testProduct->invalidPtr = art::Ptr<int>();

    event.put(std::move(testProduct));

    auto const& hVStringProduct =
      event.getValidHandle<std::vector<StringProduct>>(tagint1);
    if (produceAssnStringInt_) {
      auto assnsAB = std::make_unique<AssnsAB_t>();
      assnsAB->addSingle(art::Ptr<StringProduct>{hVStringProduct, 0},
                         art::Ptr<int>{hint1, 1},
                         LiteAssnTestData{0, 1, "A"});
      assnsAB->addSingle(art::Ptr<StringProduct>{hVStringProduct, 1},
                         art::Ptr<int>{hint1, 2},
                         LiteAssnTestData{1, 2, "B"});
      event.put(move(assnsAB));
    }

    if (produceAssnIntString_) {
      auto assnsBA = std::make_unique<AssnsBA_t>();
      assnsBA->addSingle(art::Ptr<int>{hint1, 2},
                         art::Ptr<StringProduct>{hVStringProduct, 2},
                         LiteAssnTestData{1, 2, "C"});
      assnsBA->addSingle(art::Ptr<int>{hint1, 1},
                         art::Ptr<StringProduct>{hVStringProduct, 0},
                         LiteAssnTestData{1, 2, "D"});
      event.put(move(assnsBA));
    }
  }
} // namespace critictest

DEFINE_ART_MODULE(critictest::GalleryPtrTestProducer)
