
void gallery_t() {

  std::vector<std::string> filenames { "test_gallery5.root", "test_gallery7.root" };

  gallery::Event ev(filenames);

  art::InputTag inputTagEventID("m1", "eventID", "PROD1");
  art::InputTag inputTagTriggerResults("TriggerResults", "", "PROD1");
  art::InputTag inputTagIncorrect("IncorrectLabel", "IncorrectInstance", "PROD1");
  art::InputTag inputTags62("m6", "i2");
  art::InputTag inputTagPtrTest("ptr1");

  unsigned int iEvent = 1;
  for(ev.toBegin(); ! ev.atEnd(); ++ev, ++iEvent) {

    art::EventAuxiliary const& aux = ev.eventAuxiliary();
    std::cout << aux.id() << "\n";
    if (iEvent == 6) iEvent = 1; // Events numbers go from 1 to 5 and repeat 1 to 5

    // NOTE, In the .cc file I used assert, but in ROOT 6 at the moment the asserts
    // are usually not run in scripts. ROOT sets the NDEBUG flag when it does its
    // Just-In-Time compilation so the assert statement are elided. To get ROOT
    // not to set this flag you must build ROOT in debug mode and set the
    // LLVMDEV environment variable before running configure which you run
    // before actually building ROOT. So I do not use assert here.

    if (aux.id().event() != iEvent) abort();

    gallery::Handle<art::TriggerResults> triggerResultsHandle;
    if (triggerResultsHandle.isValid()) abort();
    if (triggerResultsHandle.whyFailed()) abort();
    if (!ev.getByLabel(inputTagTriggerResults, triggerResultsHandle)) abort();
    if (!triggerResultsHandle.isValid()) abort();
    if (triggerResultsHandle.whyFailed()) abort();
    std::string test1 = triggerResultsHandle->parameterSetID().to_string();
    std::string test2 = (*triggerResultsHandle).parameterSetID().to_string();

    if (ev.getByLabel(inputTagIncorrect, triggerResultsHandle)) abort();
    if (triggerResultsHandle.isValid()) abort();
    if (!triggerResultsHandle.whyFailed()) abort();
    bool exceptionWasThrown = true;
    try {
      triggerResultsHandle->parameterSetID().to_string();
      exceptionWasThrown = false;
    } catch (cet::exception const&) {
    }
    if (!exceptionWasThrown) abort();

    auto eventIDInt = ev.getValidHandle<critictest::IntProduct>(inputTagEventID);
    if (static_cast<unsigned int>(eventIDInt->value) != aux.id().event());

    auto triggerResults = ev.getValidHandle<art::TriggerResults>(inputTagTriggerResults);
    std::cout << "psetID = " <<  triggerResults->parameterSetID().to_string() << "\n";
    std::string test3 = triggerResults->parameterSetID().to_string();
    if (test1 != test3 || test2 != test3) abort();

    auto stringProduct62 = ev.getValidHandle<critictest::StringProduct>(inputTags62);
    if (stringProduct62->name_ != "s621") abort();

    auto ptrTestProduct = ev.getValidHandle<critictest::LitePtrTestProduct>(inputTagPtrTest);

    int eventInt = static_cast<int>(aux.id().event());
    if (*ptrTestProduct->ptrInt1 != 111 + eventInt) abort();
    if (!ptrTestProduct->ptrInt1.isAvailable()) abort();
    if (*ptrTestProduct->ptrInt2 != 122) abort();
    if (*ptrTestProduct->ptrInt3 != 133) abort();

    if (ptrTestProduct->ptrSimple1->key != 111) abort();
    if (ptrTestProduct->ptrSimple2->key != 122) abort();
    if (ptrTestProduct->ptrSimple3->key != 133) abort();
    if (ptrTestProduct->ptrSimple3->dummy() != 16.25) abort();

    if (ptrTestProduct->ptrSimpleDerived1->key != 111) abort();
    if (ptrTestProduct->ptrSimpleDerived2->key != 122) abort();
    if (ptrTestProduct->ptrSimpleDerived3->key != 133) abort();
    if (ptrTestProduct->ptrSimpleDerived3->dummy() != 16.25) abort();

    if (*ptrTestProduct->ptrVectorInt.at(0) != 111 + eventInt) abort();
    if (!ptrTestProduct->ptrVectorInt.isAvailable()) abort();
    if (*ptrTestProduct->ptrVectorInt.at(1) != 131) abort();

    if (ptrTestProduct->ptrVectorSimple.at(0)->key != 111) abort();
    if (ptrTestProduct->ptrVectorSimple.at(1)->key != 131) abort();
    if (ptrTestProduct->ptrVectorSimple.at(1)->dummy() != 16.25) abort();

    if (ptrTestProduct->ptrVectorSimpleDerived.at(0)->key != 111) abort();
    if (ptrTestProduct->ptrVectorSimpleDerived.at(1)->key != 131) abort();
    if (ptrTestProduct->ptrVectorSimpleDerived.at(1)->dummy() != 16.25) abort();

    if (!(ptrTestProduct->ptrInt1 &&
          ptrTestProduct->ptrInt1.isAvailable() &&
          !ptrTestProduct->ptrInt1.isNull())) abort();

    if (!(!ptrTestProduct->ptrIntoContainerToBeDropped &&
          !ptrTestProduct->ptrIntoContainerToBeDropped.isAvailable() &&
          !ptrTestProduct->ptrIntoContainerToBeDropped.isNull())) abort();

    // I cannot get this part of the test to work in a ROOT script
    // It does throw properly, but I cannot test it because it is
    // not catching the throw properly. I do not understand why.
    // bool exceptionThrown = true;
    // try {
    //   auto x = *ptrTestProduct->ptrIntoContainerToBeDropped;
    //   std::cout << x << "\n";
    //   exceptionThrown = false;
    // } catch (art::Exception const&) {
    // }
    // assert(exceptionThrown);

    if (!(!ptrTestProduct->nullPtr &&
          ptrTestProduct->nullPtr.isAvailable() &&
          ptrTestProduct->nullPtr.isNull())) abort();

    // I chose not to test the behavior if a null or invalid Ptr is
    // dereferenced. As of this writing the behavior is this:
    //
    // Ptr::get() will return a null pointer for a null or invalid Ptr
    // Ptr::operator*() will dereference whatever "get" returns (so segfault)
    // Ptr::operator->() throws an exception for invalid Ptrs and
    // I am guessing undefined behavior for null Ptrs
    //
    // This is all in the Ptr class and really has nothing to do with
    // gallery. The same behavior should exist in the full art framework.
    
    if (!(!ptrTestProduct->nullDroppedPtr &&
          !ptrTestProduct->nullDroppedPtr.isAvailable() &&
          ptrTestProduct->nullDroppedPtr.isNull())) abort();

    // Note some of the documentation says an invalid Ptr is nonNull,
    // but that is not the way the Ptr class is written.
    if (!(!ptrTestProduct->invalidPtr &&
          !ptrTestProduct->invalidPtr.isAvailable() &&
          ptrTestProduct->invalidPtr.isNull())) abort();

  }
}
