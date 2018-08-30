#include "art/Framework/Core/ModuleMacros.h"
#include "critic/test/art/GenericOneSimpleProductAnalyzer.h"
#include "art/test/TestObjects/ToyProducts.h"

namespace arttest {
  typedef GenericOneSimpleProductAnalyzer<double, DoubleProduct>
    DoubleTestAnalyzer;
}

DEFINE_ART_MODULE(arttest::DoubleTestAnalyzer)
