#ifndef test_CriticTestObjects_classes_h
#define test_CriticTestObjects_classes_h

#include "canvas/Persistency/Common/Assns.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "canvas/Persistency/Common/PtrVector.h"
#include "canvas/Persistency/Common/Wrapper.h"
#include "test/CriticTestObjects/LiteAssnTestData.h"
#include "test/CriticTestObjects/LitePtrTestProduct.h"
#include "test/CriticTestObjects/ToyProducts.h"

#include <string>
#include <vector>

namespace critictest {
  struct dictionary {
    art::Wrapper<critictest::IntProduct> dummyIntProduct;
    art::Wrapper<critictest::StringProduct> dummyStringProduct;
    std::vector<critictest::StringProduct> dummyVStringProduct;
    art::Wrapper<std::vector<critictest::StringProduct> > dummyWVStringProduct;

    cet::map_vector<critictest::StringProduct>::value_type dummy1;
    cet::map_vector<critictest::StringProduct>::impl_type dummy2;
    cet::map_vector<critictest::StringProduct> dummy3;
    art::Wrapper<cet::map_vector<critictest::StringProduct> > dummy4;

    std::vector<critictest::SimpleDerived> dummyVSimpleDerived;
    art::Wrapper<std::vector<critictest::SimpleDerived> > dummyWVSimpleDerived;
    art::Wrapper<critictest::LitePtrTestProduct> dummyWLitePtrTestProduct;
    art::Ptr<critictest::Simple> dummyPtrSimple;
    art::Ptr<critictest::SimpleDerived> dummyPtrSimpleDerived;
    art::Ptr<critictest::Simple> dummyPtrVectorSimple;
    art::Ptr<critictest::SimpleDerived> dummyPtrVectorSimpleDerived;

    art::Assns<int, critictest::StringProduct, critictest::LiteAssnTestData> dummy5;
    art::Wrapper<art::Assns<int, critictest::StringProduct, critictest::LiteAssnTestData> > dummy6;

    art::Assns<critictest::StringProduct, int, critictest::LiteAssnTestData> dummy7;
    art::Wrapper<art::Assns<critictest::StringProduct, int, critictest::LiteAssnTestData> > dummy8;

    art::Assns<int, critictest::StringProduct, void> dummy9;
    art::Wrapper<art::Assns<int, critictest::StringProduct, void> > dummy10;

    art::Assns<critictest::StringProduct, int, void> dummy11;
    art::Wrapper<art::Assns<critictest::StringProduct, int, void> > dummy12;
  };
}
#endif
