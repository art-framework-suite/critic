#include "art/Framework/Core/ModuleMacros.h"
#include "critic/test/art/GenericOneSimpleProductAnalyzer.h"

#include <string>

namespace {
  using BareStringAnalyzer =
    art::test::GenericOneSimpleProductAnalyzer<std::string, std::string>;
}

DEFINE_ART_MODULE(BareStringAnalyzer)
