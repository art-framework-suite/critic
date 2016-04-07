
void print_and_abort(std::string const& msg = std::string("")) {
  std::cerr << msg << std::endl;
  abort();
}

void gallery_t() {

  std::vector<std::string> filenames { "test_gallery5.root", "test_gallery7.root" };

  gallery::Event ev(filenames);

  art::InputTag inputTagEventID("m1", "eventID", "PROD1");
  art::InputTag inputTagTriggerResults("TriggerResults", "", "PROD1");
  art::InputTag inputTagIncorrect("IncorrectLabel", "IncorrectInstance", "PROD1");
  art::InputTag inputTags62("m6", "i2");
  art::InputTag inputTagPtrTest("ptr1");

  art::InputTag inputTag111(std::string("m1::PROD1"));
  art::InputTag inputTagAssnTest1("ptr1");
  art::InputTag inputTagAssnTest2("ptr1::PROD3");

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

    if (aux.id().event() != iEvent) print_and_abort("100");

    gallery::Handle<art::TriggerResults> triggerResultsHandle;
    if (triggerResultsHandle.isValid()) print_and_abort("101");
    if (triggerResultsHandle.whyFailed()) print_and_abort("102");
    if (!ev.getByLabel(inputTagTriggerResults, triggerResultsHandle)) print_and_abort("103");
    if (!triggerResultsHandle.isValid()) print_and_abort("104");
    if (triggerResultsHandle.whyFailed()) print_and_abort("105");
    std::string test1 = triggerResultsHandle->parameterSetID().to_string();
    std::string test2 = (*triggerResultsHandle).parameterSetID().to_string();

    if (ev.getByLabel(inputTagIncorrect, triggerResultsHandle)) print_and_abort("108");
    if (triggerResultsHandle.isValid()) print_and_abort("109");
    if (!triggerResultsHandle.whyFailed()) print_and_abort("110");
    bool exceptionWasThrown = true;
    try {
      triggerResultsHandle->parameterSetID().to_string();
      exceptionWasThrown = false;
    } catch (cet::exception const&) {
    }
    if (!exceptionWasThrown) print_and_abort("111");

    auto eventIDInt = ev.getValidHandle<critictest::IntProduct>(inputTagEventID);
    if (static_cast<unsigned int>(eventIDInt->value) != aux.id().event()) print_and_abort("112");

    auto triggerResults = ev.getValidHandle<art::TriggerResults>(inputTagTriggerResults);
    std::cout << "psetID = " <<  triggerResults->parameterSetID().to_string() << "\n";
    std::string test3 = triggerResults->parameterSetID().to_string();
    if (test1 != test3 || test2 != test3) print_and_abort("113");

    auto stringProduct62 = ev.getValidHandle<critictest::StringProduct>(inputTags62);
    if (stringProduct62->name_ != "s621") print_and_abort("114");

    auto ptrTestProduct = ev.getValidHandle<critictest::LitePtrTestProduct>(inputTagPtrTest);

    int eventInt = static_cast<int>(aux.id().event());
    if (*ptrTestProduct->ptrInt1 != 111 + eventInt) print_and_abort("115");
    if (!ptrTestProduct->ptrInt1.isAvailable()) print_and_abort("116");
    if (*ptrTestProduct->ptrInt2 != 122) print_and_abort("117");
    if (*ptrTestProduct->ptrInt3 != 133) print_and_abort("118");

    if (ptrTestProduct->ptrSimple1->key != 111) print_and_abort("119");
    if (ptrTestProduct->ptrSimple2->key != 122) print_and_abort("120");
    if (ptrTestProduct->ptrSimple3->key != 133) print_and_abort("121");
    if (ptrTestProduct->ptrSimple3->dummy() != 16.25) print_and_abort("122");

    if (ptrTestProduct->ptrSimpleDerived1->key != 111) print_and_abort("123");
    if (ptrTestProduct->ptrSimpleDerived2->key != 122) print_and_abort("124");
    if (ptrTestProduct->ptrSimpleDerived3->key != 133) print_and_abort("125");
    if (ptrTestProduct->ptrSimpleDerived3->dummy() != 16.25) print_and_abort("126");

    if (*ptrTestProduct->ptrVectorInt.at(0) != 111 + eventInt) print_and_abort("127");
    if (!ptrTestProduct->ptrVectorInt.isAvailable()) print_and_abort("128");
    if (*ptrTestProduct->ptrVectorInt.at(1) != 131) print_and_abort("129");

    if (ptrTestProduct->ptrVectorSimple.at(0)->key != 111) print_and_abort("130");
    if (ptrTestProduct->ptrVectorSimple.at(1)->key != 131) print_and_abort("131");
    if (ptrTestProduct->ptrVectorSimple.at(1)->dummy() != 16.25) print_and_abort("132");

    if (ptrTestProduct->ptrVectorSimpleDerived.at(0)->key != 111) print_and_abort("133");
    if (ptrTestProduct->ptrVectorSimpleDerived.at(1)->key != 131) print_and_abort("134");
    if (ptrTestProduct->ptrVectorSimpleDerived.at(1)->dummy() != 16.25) print_and_abort("135");

    if (!(ptrTestProduct->ptrInt1 &&
          ptrTestProduct->ptrInt1.isAvailable() &&
          !ptrTestProduct->ptrInt1.isNull())) print_and_abort("136");

    if (!(!ptrTestProduct->ptrIntoContainerToBeDropped &&
          !ptrTestProduct->ptrIntoContainerToBeDropped.isAvailable() &&
          !ptrTestProduct->ptrIntoContainerToBeDropped.isNull())) print_and_abort("137");

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
          ptrTestProduct->nullPtr.isNull())) print_and_abort("138");

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
          ptrTestProduct->nullDroppedPtr.isNull())) print_and_abort("139");

    // Note some of the documentation says an invalid Ptr is nonNull,
    // but that is not the way the Ptr class is written.
    if (!(!ptrTestProduct->invalidPtr &&
          !ptrTestProduct->invalidPtr.isAvailable() &&
          ptrTestProduct->invalidPtr.isNull())) print_and_abort("140");

    gallery::Handle<art::Assns<critictest::StringProduct, int, critictest::LiteAssnTestData> > assnsABHandle1;
    ev.getByLabel(inputTagAssnTest1, assnsABHandle1);

    gallery::Handle<art::Assns<critictest::StringProduct, int, critictest::LiteAssnTestData> > assnsABHandle2;
    ev.getByLabel(inputTagAssnTest2, assnsABHandle2);

    gallery::Handle<art::Assns<int, critictest::StringProduct, critictest::LiteAssnTestData> > assnsBAHandle3;
    ev.getByLabel(inputTagAssnTest1, assnsBAHandle3);

    gallery::Handle<art::Assns<int, critictest::StringProduct, critictest::LiteAssnTestData> > assnsBAHandle4;
    ev.getByLabel(inputTagAssnTest2, assnsBAHandle4);

    if (!assnsABHandle1.isValid() ||
        !assnsABHandle2.isValid() ||
        !assnsBAHandle3.isValid() ||
        !assnsBAHandle4.isValid()) print_and_abort("141");

    // handles 1 and 2 should be the same
    // handles 3 and 4 should be the same
    // 1 and 3 are also the same except first and second of the pair<Ptr,Ptr>
    // stored in the Assns are reversed.

    if (ev.fileEntry() == 0) {
      if ((*assnsABHandle1)[0].first->name_ != std::string("s111")) print_and_abort("1");
      if (*(*assnsABHandle2)[0].second != 121) print_and_abort("2");
      if (assnsABHandle1->data(0).label != std::string("A")) print_and_abort("3");

      if ((*assnsABHandle1)[1].first->name_ != std::string("s121")) print_and_abort("4");
      if (*(*assnsABHandle2)[1].second != 131) print_and_abort("5");
      if (assnsABHandle1->data(1).label != std::string("B")) print_and_abort("6");
    } else {
      if (assnsABHandle1->at(0).first->name_ != std::string("s131")) print_and_abort("7");
      if (*(*assnsABHandle2)[0].second != 131) print_and_abort("8");
      if (assnsABHandle1->data(0).label != std::string("C")) print_and_abort("9");

      if ((*assnsABHandle1)[1].first->name_ != std::string("s111")) print_and_abort("10");
      if (*(*assnsABHandle2)[1].second != 121) print_and_abort("11");
      if (assnsABHandle1->data(1).label != std::string("D")) print_and_abort("12");
    }

    if (ev.fileEntry() == 0) {
      if ((*assnsBAHandle3)[0].second->name_ != std::string("s111")) print_and_abort("13");
      if (*(*assnsBAHandle4)[0].first != 121) print_and_abort("14");
      if (assnsBAHandle3->data(0).label != std::string("A")) print_and_abort("15");

      if ((*assnsBAHandle3)[1].second->name_ != std::string("s121")) print_and_abort("16");
      if (*(*assnsBAHandle4)[1].first != 131) print_and_abort("17");
      if (assnsBAHandle3->data(1).label != std::string("B")) print_and_abort("18");
    } else {
      if (assnsBAHandle3->at(0).second->name_ != std::string("s131")) print_and_abort("19");
      if (*(*assnsBAHandle4)[0].first != 131) print_and_abort("20");
      if (assnsBAHandle3->data(0).label != std::string("C")) print_and_abort("21");

      if ((*assnsBAHandle3)[1].second->name_ != std::string("s111")) print_and_abort("22");
      if (*(*assnsBAHandle4)[1].first != 121) print_and_abort("23");
      if (assnsBAHandle3->data(1).label != std::string("D")) print_and_abort("24");
    }

    gallery::Handle<std::vector<critictest::StringProduct> > hVStringProduct;
    ev.getByLabel(inputTag111, hVStringProduct);
    if (!hVStringProduct.isValid()) print_and_abort("25");

    /*
    DOES NOT WORK CURRENTLY DUE TO A ROOT BUG, THIS NEEDS TO BE COMMENTED BACK
    IN AFTER THE ROOT BUG IS FIXED OR A WORK AROUND IS CREATED
    art::FindOne<int, critictest::LiteAssnTestData> findOne(hVStringProduct, ev, inputTagAssnTest1);
    if (!findOne.at(0).isValid()) print_and_abort("26");
    if (ev.fileEntry() == 0) {
      if (findOne.at(0).ref() != 121) print_and_abort("27");
      if (findOne.at(1).ref() != 131) print_and_abort("28");
      if (findOne.data(0).ref().label != std::string("A")) print_and_abort("29");
      if (findOne.data(1).ref().label != std::string("B")) print_and_abort("30");
    } else {
      if (findOne.at(0).ref() != 121) print_and_abort("31");
      if (findOne.at(2).ref() != 131) print_and_abort("32");
      if (findOne.data(0).ref().label != std::string("D")) print_and_abort("33");
      if (findOne.data(2).ref().label != std::string("C")) print_and_abort("34");
    }

    gallery::Handle<std::vector<int> > hB;
    ev.getByLabel(inputTag111, hB);
    if (!hB.isValid()) print_and_abort("36");

    art::FindOne<critictest::StringProduct, critictest::LiteAssnTestData> findOneBA(hB, ev, inputTagAssnTest1);
    if(!findOneBA.at(1).isValid()) print_and_abort("37");
    if (ev.fileEntry() == 0) {
      if (findOneBA.at(1).ref().name_ != critictest::StringProduct(std::string("s111")).name_) print_and_abort("38");
      if (findOneBA.at(2).ref().name_ != critictest::StringProduct(std::string("s121")).name_) print_and_abort("39");
      if (findOneBA.data(1).ref().label != std::string("A")) print_and_abort("40");
      if (findOneBA.data(2).ref().label != std::string("B")) print_and_abort("41");
    } else {
      if (findOneBA.at(1).ref().name_ != critictest::StringProduct(std::string("s111")).name_) print_and_abort("42");
      if (findOneBA.at(2).ref().name_ != critictest::StringProduct(std::string("s131")).name_) print_and_abort("43");
      if (findOneBA.data(1).ref().label != std::string("D")) print_and_abort("44");
      if (findOneBA.data(2).ref().label != std::string("C")) print_and_abort("45");
    }
    */
  }
}
