#include "art/Framework/Core/ModuleMacros.h"
#include "art/test/TestObjects/ToyProducts.h"
#include "critic/test/art/GenericOneSimpleProductAnalyzer.h"

namespace {
  using DoubleTestAnalyzer =
    art::test::GenericOneSimpleProductAnalyzer<double, arttest::DoubleProduct>;
}

DEFINE_ART_MODULE(DoubleTestAnalyzer)
