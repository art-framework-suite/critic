// ------------------------------------------------------------
//
// ToyRawInput is a RawInputSource that pretends to reconstitute several
// products. It exercises the Source template.
//
// ------------------------------------------------------------

#include "art/Framework/Core/FileBlock.h"
#include "art/Framework/Core/InputSourceMacros.h"
#include "art/Framework/IO/Sources/Source.h"
#include "cetlib/filepath_maker.h"
#include "critic/test/art/toy-raw-input/ToySource.h"
#include "fhiclcpp/ParameterSet.h"

namespace arttest {
  // ToyFile is the sort of class that experimenters who make use of
  // Source must write.
  class ToyFileReader;

  // ToyRawInput is an instantiation of the Source template.
  using ToyRawFileInput = art::Source<ToyFileReader>;
} // namespace arttest

class arttest::ToyFileReader final : public ToySource {
public:
  ToyFileReader(fhicl::ParameterSet const& ps,
                art::ProductRegistryHelper& help,
                art::SourceHelper const& sHelper);

  void readFile(std::string const& name, art::FileBlock*& fb) override;
};

arttest::ToyFileReader::ToyFileReader(fhicl::ParameterSet const& ps,
                                      art::ProductRegistryHelper& help,
                                      art::SourceHelper const& sHelper)
  : ToySource{ps, help, sHelper}
{}

void
arttest::ToyFileReader::readFile(std::string const& name, art::FileBlock*& fb)
{
  if (throw_on_readFile_)
    throw_exception_from("readFile");
  cet::filepath_lookup_after1 lookupPolicy{".:"};
  auto const file_pset = fhicl::ParameterSet::make(name, lookupPolicy);

  if (!file_pset.get_if_present("data", fileData_)) {
    throw art::Exception{art::errors::Configuration}
      << "ToyFileReader expects to find a parameter \"data\" representing a "
         "file's\n"
      << "contents in file " << name << "\n";
  }
  currentFilename_ = name;
  current_ = fileData_.begin();
  end_ = fileData_.end();
  fb = new art::FileBlock{art::FileFormatVersion{1, "ToyFileReader 2011a"},
                          currentFilename_};
}

DEFINE_ART_INPUT_SOURCE(arttest::ToyRawFileInput)
