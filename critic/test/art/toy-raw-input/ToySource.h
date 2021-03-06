#ifndef critic_test_art_toy_raw_input_ToySource_h
#define critic_test_art_toy_raw_input_ToySource_h

// Our simulated input file format is:
// A parameter in a parameter set, which contains a vector of vector of int.
// Each inner vector is a triplet of run/subrun/event number.
//   -1 means no new item of that type
//

#include "art/Framework/Core/fwd.h"
#include "art/Framework/IO/Sources/SourceHelper.h"
#include "art/Framework/Principal/fwd.h"
#include "fhiclcpp/ParameterSet.h"

#include <string>
#include <vector>

namespace arttest {
  class ToySource;
}

class arttest::ToySource {
public:
  ToySource(fhicl::ParameterSet const& ps,
            art::ProductRegistryHelper& help,
            art::SourceHelper const& sHelper);

  virtual ~ToySource() = default;

  void closeCurrentFile();

  virtual void readFile(std::string const& name, art::FileBlock*& fb) = 0;

  bool readNext(art::RunPrincipal const* const inR,
                art::SubRunPrincipal const* const inSR,
                art::RunPrincipal*& outR,
                art::SubRunPrincipal*& outSR,
                art::EventPrincipal*& outE);

protected:
  // Helper function to throw an exception with the appropriate text.
  [[noreturn]] static void throw_exception_from(const char* funcname);

  using vv_t = std::vector<std::tuple<int, int, int>>;
  using iter = vv_t::const_iterator;

  iter current_{};
  iter end_{};
  fhicl::ParameterSet data_;
  vv_t fileData_{};

  art::SourceHelper const& sHelper_;
  std::string currentFilename_{};
  bool const throw_on_construction_;
  bool const throw_on_closeCurrentFile_;
  bool const throw_on_readNext_;
  bool const throw_on_readFile_;

  art::TypeLabel vtl_;
};
#endif /* critic_test_art_toy_raw_input_ToySource_h */

// Local Variables:
// mode: c++
// End:
