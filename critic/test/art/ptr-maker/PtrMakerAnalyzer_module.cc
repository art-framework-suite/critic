////////////////////////////////////////////////////////////////////////
// Class:       PtrMakerAnalyzer
// Plugin Type: analyzer (art v2_05_00)
// File:        PtrMakerAnalyzer_module.cc
//
// Generated at Wed Nov 23 21:29:25 2016 by Saba Sehrish using cetskelgen
// from cetlib version v1_21_00.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"

class PtrMakerAnalyzer : public art::EDAnalyzer {
public:
  using intptrvector_t = art::PtrVector<int>;

  struct Config {
    fhicl::Atom<std::string> input_label{fhicl::Name{"input_label"}};
    fhicl::Atom<int> nvalues{fhicl::Name{"nvalues"}};
  };
  using Parameters = art::EDAnalyzer::Table<Config>;
  explicit PtrMakerAnalyzer(Parameters const& p);

  // Plugins should not be copied or assigned.
  PtrMakerAnalyzer(PtrMakerAnalyzer const&) = delete;
  PtrMakerAnalyzer(PtrMakerAnalyzer&&) = delete;
  PtrMakerAnalyzer& operator=(PtrMakerAnalyzer const&) = delete;
  PtrMakerAnalyzer& operator=(PtrMakerAnalyzer&&) = delete;

  // Required functions.
  void analyze(art::Event const& e) override;

private:
  std::string const fInputLabel;
  int const nvalues;
};

PtrMakerAnalyzer::PtrMakerAnalyzer(Parameters const& p)
  : EDAnalyzer{p}, fInputLabel{p().input_label()}, nvalues{p().nvalues()}
{}

void
PtrMakerAnalyzer::analyze(art::Event const& e)
{
  std::cerr << "PtrMakerAnalyzer is running\n";
  auto const& ipv = e.getProduct<intptrvector_t>(fInputLabel);
  size_t sz = ipv.size();
  if (sz != (size_t)nvalues) {
    throw cet::exception("SizeMismatch")
      << "Expected a PtrVector of size " << nvalues
      << " but the obtained size is " << sz << '\n';
  }

  int eid = e.id().event();

  // now check the values
  intptrvector_t local{ipv};
  for (int i = 0; i < nvalues; ++i) {
    assert(*local[i] == eid + i);
  }
}

DEFINE_ART_MODULE(PtrMakerAnalyzer)
