#ifndef critic_test_TestObjects_classes_h
#define critic_test_TestObjects_classes_h

#include "canvas/Persistency/Common/Ptr.h"
#include "canvas/Persistency/Common/PtrVector.h"
#include "canvas/Persistency/Common/Wrapper.h"
#include "test/TestObjects/LitePtrTestProduct.h"
#include "test/TestObjects/ToyProducts.h"

#include <vector>

namespace critictest {
  struct dictionary {
    art::Wrapper<critictest::IntProduct> dummyIntProduct;
    art::Wrapper<critictest::StringProduct> dummyStringProduct;
    std::vector<critictest::SimpleDerived> dummyVSimpleDerived;
    art::Wrapper<std::vector<critictest::SimpleDerived> > dummyWVSimpleDerived;
    art::Wrapper<critictest::LitePtrTestProduct> dummyWLitePtrTestProduct;
    art::Ptr<int> dummyPtrInt;
    art::Ptr<critictest::Simple> dummyPtrSimple;
    art::Ptr<critictest::SimpleDerived> dummyPtrSimpleDerived;
    art::PtrVector<int> dummyPtrVectorInt;
    art::Ptr<critictest::Simple> dummyPtrVectorSimple;
    art::Ptr<critictest::SimpleDerived> dummyPtrVectorSimpleDerived;
  };
}
