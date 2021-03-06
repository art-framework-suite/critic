#include "art/Framework/Core/EDFilter.h"

#include "fhiclcpp/types/Atom.h"

#include <cassert>

namespace {
  using namespace fhicl;
  struct Config {};
} // namespace

namespace arttest {

  class PassEverything : public art::EDFilter {
  public:
    using Parameters = EDFilter::Table<Config>;
    explicit PassEverything(EDFilter::Table<Config> const&) {}

    bool
    filter(art::Event&) override
    {
      return EDFilter::Pass;
    }
  };

} // namespace arttest

DEFINE_ART_MODULE(arttest::PassEverything)
