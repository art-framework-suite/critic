// ------------------------------------------------------------
//
// The purpose of 'DoNothingInput' is to verify that when the
// destructor is called, the art::ServiceHandle can be constructed
// successfully and not throw.  This tests the expected behavior of
// the art::EventProcessor constructor.
//
// ------------------------------------------------------------

#include "art/Framework/Core/InputSourceMacros.h"
#include "art/Framework/IO/Sources/Source.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "critic/test/art/Wanted.h"

namespace art {
  class FileBlock;
  class ProductRegistryHelper;
  class SourceHelper;
} // namespace art

namespace fhicl {
  class ParameterSet;
}

namespace art::test {

  class DoNothingInput {
  public:
    DoNothingInput(fhicl::ParameterSet const&,
                   ProductRegistryHelper&,
                   SourceHelper const&)
    {}

    ~DoNothingInput() { ServiceHandle<Wanted> shouldNotThrow [[maybe_unused]]; }

    void
    readFile(std::string const&, art::FileBlock*&)
    {}

    void
    closeCurrentFile()
    {}

    bool
    readNext(RunPrincipal const* const,
             SubRunPrincipal const* const,
             RunPrincipal*&,
             SubRunPrincipal*&,
             EventPrincipal*&)
    {
      return false;
    }
  };

} // namespace art::detail

DEFINE_ART_INPUT_SOURCE(art::Source<art::test::DoNothingInput>)
