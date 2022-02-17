#include "art/test/TestObjects/ToyProducts.h"
#include "critic/test/art/GenericOneSimpleProductAnalyzer.h"

namespace {
  using CompressedIntTestAnalyzer =
    art::test::GenericOneSimpleProductAnalyzer<int,
                                               arttest::CompressedIntProduct>;
}

DEFINE_ART_MODULE(CompressedIntTestAnalyzer)
