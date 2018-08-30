#include "art/Framework/Core/ModuleMacros.h"
#include "critic/test/art/GenericOneSimpleProductAnalyzer.h"
#include "art/test/TestObjects/ToyProducts.h"

namespace arttest {
  using IntTestAnalyzer = GenericOneSimpleProductAnalyzer<int, IntProduct>;
}

DEFINE_ART_MODULE(arttest::IntTestAnalyzer)
