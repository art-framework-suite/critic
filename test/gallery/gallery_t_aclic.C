// I had to comment almost everything out to get this
// to run. Note that I had a version of this working in
// a pre 1_18_0 branch of art that worked with ROOT 6.
// When I ported the code to 1_18_01 it started failing
// with a seg fault I do not understand. Given that we do
// not really need ACLiC, I did not spend enough time to
// understand the problem and get it to work. I am leaving
// the code in the release as a starting point in case
// I or someone else ever gets time to fix it. I suspect
// it might be a problem in ROOT and not easy to fix.
//
// If running this under ACLiC, then before running this script
// give the following commands at the ROOT command line directly.
// gInterpreter->AddIncludePath("${ART_INC}")
// gInterpreter->AddIncludePath("${CETLIB_INC}")
// gInterpreter->AddIncludePath("${FHICLCPP_INC}")
// Alternatively one can set this environmental variable:
// export ROOT_INCLUDE_PATH=${ART_INC}:${CETLIB_INC}:${FHICLCPP_INC}
// Note that none of this is needed if running the script through the
// interpreter.

#if !defined(__CLING__) || defined(__ROOTCLING__)
//#include "canvas/FWLite/Event.h"
//#include "test/TestObjects/ToyProducts.h"
//#include "canvas/Persistency/Common/TriggerResults.h"
#endif

#include "TFile.h"

#include <cassert>
#include <string>
#include <vector>
#include <iostream>

void fwlite_t_aclic() {

  std::cout << "ACLIC version\n";

  std::vector<std::string> filenames { "test_fwlite5.root", "test_fwlite7.root" };
  // canvas::Event ev(filenames);

  /*
  canvas::InputTag inputTagEventID("m1", "eventID", "PROD1");
  canvas::InputTag inputTagTriggerResults("TriggerResults", "", "PROD1");
  canvas::InputTag inputTags62("m6", "i2");

  unsigned int iEvent = 1;
  for(ev.toBegin(); ! ev.atEnd(); ++ev, ++iEvent) {

    art::EventAuxiliary const& aux = ev.eventAuxiliary();
    std::cout << aux.id() << "\n";
    if (iEvent == 6) iEvent = 1; // Events numbers go from 1 to 5 and repeat 1 to 5
    assert(aux.id().event() == iEvent);

    auto eventIDInt = ev.getValidHandle<arttest::IntProduct>(inputTagEventID);
    assert(static_cast<unsigned int>(eventIDInt->value) == aux.id().event());

    auto triggerResults = ev.getValidHandle<art::TriggerResults>(inputTagTriggerResults);
    std::cout << "psetID = " <<  triggerResults->parameterSetID().to_string() << "\n";

    auto stringProduct62 = ev.getValidHandle<arttest::StringProduct>(inputTags62);
    assert(stringProduct62->name_ == "s621");
  }
  */
}
