#include "art/Framework/Core/ModuleMacros.h"
#include "art/test/TestObjects/ToyProducts.h"
#include "critic/test/art/GenericOneSimpleProductAnalyzer.h"

namespace {
  using IntTestAnalyzer =
    art::test::GenericOneSimpleProductAnalyzer<int, arttest::IntProduct>;
}

DEFINE_ART_MODULE(IntTestAnalyzer)
