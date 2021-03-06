#include "art/Framework/Core/OutputModule.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/EventPrincipal.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Persistency/Provenance/ModuleContext.h"
#include "art/Persistency/Provenance/ModuleDescription.h"
#include "art/Utilities/Globals.h"
#include "canvas/Persistency/Common/TriggerResults.h"
#include "fhiclcpp/types/ConfigurationTable.h"

#include <cassert>
#include <cstdlib>
#include <iostream>

using namespace art;

namespace {

  void
  printBits(unsigned char c)
  {
    // cout << "HEX: "<< "0123456789ABCDEF"[((c >> 4) & 0xF)] << std::endl;
    for (int i = 7; i >= 0; --i) {
      int bit = ((c >> i) & 1);
      std::cout << " " << bit;
    }
  }

  void
  packIntoString(std::vector<unsigned char> const& source,
                 std::vector<unsigned char>& package)
  {
    unsigned int const packInOneByte{4};
    // Two bits per HLT.
    std::size_t const sizeOfPackage{
      source.empty() ? 0u : 1u + ((source.size() - 1) / packInOneByte)};
    package.resize(sizeOfPackage);
    memset(&package[0], 0x00, sizeOfPackage);
    for (unsigned int i = 0; i != source.size(); ++i) {
      unsigned int whichByte = i / packInOneByte;
      unsigned int indexWithinByte = i % packInOneByte;
      package[whichByte] =
        package[whichByte] | (source[i] << (indexWithinByte * 2));
    }
    // for (unsigned int i=0; i !=package.size() ; ++i)
    //   printBits(package[i]);
    std::cout << std::endl;
  }

} // unnamed namespace

namespace arttest {
  class TestBitsOutput;
}

class arttest::TestBitsOutput : public art::OutputModule {
public:
  struct Config {
    fhicl::TableFragment<art::OutputModule::Config> omConfig;
    fhicl::Atom<int> bitMask{fhicl::Name("bitMask")};
    fhicl::Atom<bool> expectTriggerResults{fhicl::Name("expectTriggerResults"),
                                           true};
  };

  using Parameters =
    fhicl::WrappedTable<Config, art::OutputModule::Config::KeysToIgnore>;
  explicit TestBitsOutput(Parameters const&);

private:
  void write(art::EventPrincipal& e) override;
  void
  writeSubRun(art::SubRunPrincipal&) override
  {}
  void
  writeRun(art::RunPrincipal&) override
  {}

  std::vector<unsigned char> hltbits_{};
  art::ModuleDescription moduleDescription_{};
  int bitMask_;
  bool expectTriggerResults_;
};

// -----------------------------------------------------------------

arttest::TestBitsOutput::TestBitsOutput(Parameters const& ps)
  : OutputModule{ps().omConfig}
  , bitMask_{ps().bitMask()}
  , expectTriggerResults_{ps().expectTriggerResults()}
{}

void
arttest::TestBitsOutput::write(art::EventPrincipal& ep)
{
  ModuleContext const mc{moduleDescription_};
  auto const ev = std::as_const(ep).makeEvent(mc);
  // There should not be a TriggerResults object in the event if all
  // three of the following requirements are met:
  //
  //     1.  MakeTriggerResults has not been explicitly set true
  //     2.  There are no filter modules in any path
  //     3.  The input file of the job does not have a TriggerResults object
  //
  // The user of this test module is expected to know whether these
  // conditions are met and let the module know if no TriggerResults
  // object is expected using the configuration file.  In this case,
  // the next few lines of code will abort if a TriggerResults object
  // is found.
  if (!expectTriggerResults_) {
    try {
      art::Handle<art::TriggerResults> prod{getTriggerResults(ev)};
      // throw doesn't happen until we dereference
      *prod;
    }
    catch (cet::exception const&) {
      // We did not find one as expected, nothing else to test.
      return;
    }
    std::cerr
      << "\narttest::TestBitsOutput::write\n"
      << "Expected there to be no TriggerResults object but we found one"
      << std::endl;
    abort();
  }

  // Now deal with the other case where we expect the object to be
  // present.
  art::Handle<art::TriggerResults> prod{getTriggerResults(ev)};
  assert(prod.isValid());
  // TriggerResults objects should have no parents.
  assert(prod.provenance()->parents().empty());
  std::vector<unsigned char> vHltState;
  auto const& hlts = art::Globals::instance()->triggerPathNames();
  unsigned int hltSize = hlts.size();
  for (unsigned int i = 0; i != hltSize; ++i) {
    vHltState.push_back(prod->at(i).state());
  }
  // Pack into member hltbits_
  packIntoString(vHltState, hltbits_);
  std::cout << "Size of hltbits:" << hltbits_.size() << std::endl;
  auto intp = reinterpret_cast<char*>(&bitMask_);
  bool matched{false};
  for (int i = hltbits_.size() - 1; i != -1; --i) {
    std::cout << std::endl << "Current Bits Mask byte:";
    printBits(hltbits_[i]);
    auto tmp = static_cast<unsigned char>(*(intp + i));
    std::cout << std::endl << "Original Byte:";
    printBits(tmp);
    std::cout << std::endl;
    if (tmp == hltbits_[i]) {
      matched = true;
    }
  }
  std::cout << "\n";
  if (!matched && hltSize > 0) {
    std::cerr << "\ncfg bitMask is different from event..aborting."
              << std::endl;
    abort();
  }
  std::cout << "\nSUCCESS: Found Matching Bits" << std::endl;
}

DEFINE_ART_MODULE(arttest::TestBitsOutput)
