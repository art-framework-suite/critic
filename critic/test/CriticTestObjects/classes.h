#ifndef test_CriticTestObjects_classes_h
#define test_CriticTestObjects_classes_h

#include "canvas/Persistency/Common/Assns.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "canvas/Persistency/Common/PtrVector.h"
#include "canvas/Persistency/Common/Wrapper.h"
#include "critic/test/CriticTestObjects/LiteAssnTestData.h"
#include "critic/test/CriticTestObjects/LitePtrTestProduct.h"
#include "art/test/TestObjects/ToyProducts.h"

#include <string>
#include <vector>

namespace critictest {
  struct dictionary {
    art::Wrapper<arttest::IntProduct> dummyIntProduct;
    art::Wrapper<arttest::StringProduct> dummyStringProduct;
    std::vector<arttest::StringProduct> dummyVStringProduct;
    art::Wrapper<std::vector<arttest::StringProduct> > dummyWVStringProduct;

    cet::map_vector<arttest::StringProduct>::value_type dummy1;
    cet::map_vector<arttest::StringProduct>::impl_type dummy2;
    cet::map_vector<arttest::StringProduct> dummy3;
    art::Wrapper<cet::map_vector<arttest::StringProduct> > dummy4;

    std::vector<arttest::SimpleDerived> dummyVSimpleDerived;
    art::Wrapper<std::vector<arttest::SimpleDerived> > dummyWVSimpleDerived;
    art::Wrapper<critictest::LitePtrTestProduct> dummyWLitePtrTestProduct;
    art::Ptr<arttest::Simple> dummyPtrSimple;
    art::Ptr<arttest::SimpleDerived> dummyPtrSimpleDerived;
    art::Ptr<arttest::Simple> dummyPtrVectorSimple;
    art::Ptr<arttest::SimpleDerived> dummyPtrVectorSimpleDerived;

    art::Assns<int, arttest::StringProduct, critictest::LiteAssnTestData> dummy5;
    art::Wrapper<art::Assns<int, arttest::StringProduct, critictest::LiteAssnTestData> > dummy6;

    art::Assns<arttest::StringProduct, int, critictest::LiteAssnTestData> dummy7;
    art::Wrapper<art::Assns<arttest::StringProduct, int, critictest::LiteAssnTestData> > dummy8;

    art::Assns<int, arttest::StringProduct, void> dummy9;
    art::Wrapper<art::Assns<int, arttest::StringProduct, void> > dummy10;

    art::Assns<arttest::StringProduct, int, void> dummy11;
    art::Wrapper<art::Assns<arttest::StringProduct, int, void> > dummy12;
  };
}
#endif
