void fwlite_t() {

  std::vector<std::string> filenames { "test_fwlite5.root", "test_fwlite7.root" };

  canvas::Event ev(filenames);

  canvas::InputTag inputTagEventID("m1", "eventID", "PROD1");
  canvas::InputTag inputTagTriggerResults("TriggerResults", "", "PROD1");
  canvas::InputTag inputTags62("m6", "i2");
  canvas::InputTag inputTagPtrTest("ptr1");

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

    auto ptrTestProduct = ev.getValidHandle<arttest::LitePtrTestProduct>(inputTagPtrTest);

    int eventInt = static_cast<int>(aux.id().event());
    assert(*ptrTestProduct->ptrInt1 == 111 + eventInt);
    assert(ptrTestProduct->ptrInt1.isAvailable());
    assert(*ptrTestProduct->ptrInt2 == 122);
    assert(*ptrTestProduct->ptrInt3 == 133);

    assert(ptrTestProduct->ptrSimple1->key == 111);
    assert(ptrTestProduct->ptrSimple2->key == 122);
    assert(ptrTestProduct->ptrSimple3->key == 133);
    assert(ptrTestProduct->ptrSimple3->dummy() == 16.25);

    assert(ptrTestProduct->ptrSimpleDerived1->key == 111);
    assert(ptrTestProduct->ptrSimpleDerived2->key == 122);
    assert(ptrTestProduct->ptrSimpleDerived3->key == 133);
    assert(ptrTestProduct->ptrSimpleDerived3->dummy() == 16.25);

    assert(*ptrTestProduct->ptrVectorInt.at(0) == 111 + eventInt);
    assert(ptrTestProduct->ptrVectorInt.isAvailable());
    assert(*ptrTestProduct->ptrVectorInt.at(1) == 131);

    assert(ptrTestProduct->ptrVectorSimple.at(0)->key == 111);
    assert(ptrTestProduct->ptrVectorSimple.at(1)->key == 131);
    assert(ptrTestProduct->ptrVectorSimple.at(1)->dummy() == 16.25);

    assert(ptrTestProduct->ptrVectorSimpleDerived.at(0)->key == 111);
    assert(ptrTestProduct->ptrVectorSimpleDerived.at(1)->key == 131);
    assert(ptrTestProduct->ptrVectorSimpleDerived.at(1)->dummy() == 16.25);

    assert(ptrTestProduct->ptrInt1 &&
           ptrTestProduct->ptrInt1.isAvailable() &&
           !ptrTestProduct->ptrInt1.isNull());

    assert(!ptrTestProduct->ptrIntoContainerToBeDropped &&
           !ptrTestProduct->ptrIntoContainerToBeDropped.isAvailable() &&
           !ptrTestProduct->ptrIntoContainerToBeDropped.isNull());

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

    assert(!ptrTestProduct->nullPtr &&
           ptrTestProduct->nullPtr.isAvailable() &&
           ptrTestProduct->nullPtr.isNull());

    // I chose not to test the behavior if a null or invalid Ptr is
    // dereferenced. As of this writing the behavior is this:
    //
    // Ptr::get() will return a null pointer for a null or invalid Ptr
    // Ptr::operator*() will dereference whatever "get" returns (so segfault)
    // Ptr::operator->() throws an exception for invalid Ptrs and
    // I am guessing undefined behavior for null Ptrs
    //
    // This is all in the Ptr class and really has nothing to do with
    // fwlite. The same behavior should exist in the full art framework.
    
    assert(!ptrTestProduct->nullDroppedPtr &&
           !ptrTestProduct->nullDroppedPtr.isAvailable() &&
           ptrTestProduct->nullDroppedPtr.isNull());

    // Note some of the documentation says an invalid Ptr is nonNull,
    // but that is not the way the Ptr class is written.
    assert(!ptrTestProduct->invalidPtr &&
           !ptrTestProduct->invalidPtr.isAvailable() &&
           ptrTestProduct->invalidPtr.isNull());

  }
}
