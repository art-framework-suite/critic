////////////////////////////////////////////////////////////////////////
// Class:       SimpleServiceTestAnalyzer
// Plugin Type: analyzer (art v1_19_00_rc3)
// File:        SimpleServiceTestAnalyzer_module.cc
//
// Generated at Mon May  9 16:40:47 2016 by Christopher Green using cetskelgen
// from cetlib version v1_17_04.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "critic/test/art/SimpleServiceTest.h"
#include "fhiclcpp/fwd.h"

#include <cassert>

namespace arttest {
  class SimpleServiceTestAnalyzer;
}

class arttest::SimpleServiceTestAnalyzer : public art::EDAnalyzer {
public:
  explicit SimpleServiceTestAnalyzer(fhicl::ParameterSet const& p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  SimpleServiceTestAnalyzer(SimpleServiceTestAnalyzer const&) = delete;
  SimpleServiceTestAnalyzer(SimpleServiceTestAnalyzer&&) = delete;
  SimpleServiceTestAnalyzer& operator=(SimpleServiceTestAnalyzer const&) =
    delete;
  SimpleServiceTestAnalyzer& operator=(SimpleServiceTestAnalyzer&&) = delete;

  // Required functions.
  void analyze(art::Event const& e) override;

private:
  // Declare member data here.
};

arttest::SimpleServiceTestAnalyzer::SimpleServiceTestAnalyzer(
  fhicl::ParameterSet const& p)
  : EDAnalyzer(p)
{
  assert(art::ServiceHandle<arttest::SimpleServiceTest>()->verifyStatus());
  assert((*art::ServiceHandle<arttest::SimpleServiceTest>()).verifyStatus());
  assert(
    art::ServiceHandle<arttest::SimpleServiceTest>().get()->verifyStatus());
}

void
arttest::SimpleServiceTestAnalyzer::analyze(art::Event const&)
{}

DEFINE_ART_MODULE(arttest::SimpleServiceTestAnalyzer)
