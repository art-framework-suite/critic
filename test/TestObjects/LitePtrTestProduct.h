#ifndef test_TestObjects_LitePtrTestProduct_h
#define test_TestObjects_LitePtrTestProduct_h

#include "canvas/Persistency/Common/Ptr.h"
#include "canvas/Persistency/Common/PtrVector.h"
#include "test/TestObjects/ToyProducts.h"

namespace critictest {

  class LitePtrTestProduct {
  public:

    LitePtrTestProduct() { }

    art::Ptr<int> ptrInt1;
    art::Ptr<int> ptrInt2;
    art::Ptr<int> ptrInt3;

    art::Ptr<arttest::Simple> ptrSimple1;
    art::Ptr<arttest::Simple> ptrSimple2;
    art::Ptr<arttest::Simple> ptrSimple3;

    art::Ptr<arttest::SimpleDerived> ptrSimpleDerived1;
    art::Ptr<arttest::SimpleDerived> ptrSimpleDerived2;
    art::Ptr<arttest::SimpleDerived> ptrSimpleDerived3;

    art::PtrVector<int> ptrVectorInt;
    art::PtrVector<arttest::Simple> ptrVectorSimple;
    art::PtrVector<arttest::SimpleDerived> ptrVectorSimpleDerived;

    art::Ptr<int> ptrIntoContainerToBeDropped;
    art::Ptr<int> nullPtr;
    art::Ptr<int> nullDroppedPtr;
    art::Ptr<int> invalidPtr;
  };
}

#endif /* test_TestObjects_LitePtrTestProduct_h */

// Local Variables:
// mode: c++
// End:
