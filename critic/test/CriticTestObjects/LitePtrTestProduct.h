#ifndef test_CriticTestObjects_LitePtrTestProduct_h
#define test_CriticTestObjects_LitePtrTestProduct_h

#include "canvas/Persistency/Common/Ptr.h"
#include "canvas/Persistency/Common/PtrVector.h"
#include "critic/test/CriticTestObjects/ToyProducts.h"

namespace critictest {

  class LitePtrTestProduct {
  public:

    LitePtrTestProduct();

    art::Ptr<int> ptrInt1;
    art::Ptr<int> ptrInt2;
    art::Ptr<int> ptrInt3;

    art::Ptr<critictest::Simple> ptrSimple1;
    art::Ptr<critictest::Simple> ptrSimple2;
    art::Ptr<critictest::Simple> ptrSimple3;

    art::Ptr<critictest::SimpleDerived> ptrSimpleDerived1;
    art::Ptr<critictest::SimpleDerived> ptrSimpleDerived2;
    art::Ptr<critictest::SimpleDerived> ptrSimpleDerived3;

    art::PtrVector<int> ptrVectorInt;
    art::PtrVector<critictest::Simple> ptrVectorSimple;
    art::PtrVector<critictest::SimpleDerived> ptrVectorSimpleDerived;

    art::Ptr<int> ptrIntoContainerToBeDropped;
    art::Ptr<int> nullPtr;
    art::Ptr<int> nullDroppedPtr;
    art::Ptr<int> invalidPtr;
  };
}

#endif /* test_CriticTestObjects_LitePtrTestProduct_h */

// Local Variables:
// mode: c++
// End:
