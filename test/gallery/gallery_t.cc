
#include "canvas/Persistency/Provenance/EventAuxiliary.h"
#include "gallery/Event.h"
#include "gallery/TypeLabelInstanceKey.h"
#include "test/CriticTestObjects/LitePtrTestProduct.h"
#include "test/CriticTestObjects/ToyProducts.h"
#include "canvas/Persistency/Common/TriggerResults.h"
#include "canvas/Utilities/TypeID.h"
#include "TFile.h"

#include <iostream>
#include <cassert>
#include <string>
#include <vector>

// Here is a summary of what was written into the input
// files. It is all arbitrary for test purposes. Here
// we are just testing that we have read what was written.
// 1 PROD1 runs modules m1 thru m6 writing all products 5 events
// 2 PROD2 same except m4 not run and m6 does not put its products, input from PROD1
// 3 PROD3 same except m3, m4, and m6 not run, m5 not put, m7 added but not put
//       and also ptr1 run, input from PROD2
// 4 PROD3 same as PROD3 except m7 and ptr1 write products also
// 5 MERGE merges the output of PROD3 and PROD4 and drops one of the Ptrs in
//       the LitePtrTestProduct
// 6 PROD2EXTRA input from 2, runs m7, adds an extra process in its output file
// 7 PROD3 reads input from 6, similar to 3 but adds m0 and does not have m7
//   the purpose of 6 and 7 is to produce a second input file to test multifile
//   input with different BranchIDLists and ProcessHistory

int main() {

  art::InputTag inputTagTriggerResults("TriggerResults", "", "PROD1");
  art::InputTag inputTagIncorrect("IncorrectLabel", "IncorrectInstance", "PROD1");
  art::InputTag inputTagEventID("m1", "eventID", "PROD1");

  art::InputTag inputTag111(std::string("m1::PROD1"));
  art::InputTag inputTag121("m1:i2:PROD1");
  art::InputTag inputTag131(std::string("m1"), std::string("i3"), std::string("PROD1"));

  art::InputTag inputTags111("m1", "", "PROD1");
  art::InputTag inputTags121("m1", "i2", "PROD1");
  art::InputTag inputTags131("m1", "i3", "PROD1");

  art::InputTag inputTag211("m2", "", "PROD1");
  art::InputTag inputTag221("m2", "i2", "PROD1");

  art::InputTag inputTag112("m1", "", "PROD2");
  art::InputTag inputTag122("m1", "i2", "PROD2");

  art::InputTag inputTag113("m1", "", "PROD3");
  art::InputTag inputTag123("m1", "i2", "PROD3");

  art::InputTag inputTag11("m1");
  art::InputTag inputTag12("m1:i2");

  art::InputTag inputTags11("m1", "");
  art::InputTag inputTags12("m1", "i2");

  art::InputTag inputTag21("m2", "");
  art::InputTag inputTag22("m2", "i2");

  art::InputTag inputTag31("m3", "");
  art::InputTag inputTag32("m3", "i2");

  art::InputTag inputTag312("m3", "", "PROD2");
  art::InputTag inputTag322("m3", "i2", "PROD2");

  art::InputTag inputTag41("m4", "");
  art::InputTag inputTag42("m4", "i2");

  art::InputTag inputTag411("m4", "", "PROD1");
  art::InputTag inputTag421("m4", "i2", "PROD1");

  art::InputTag inputTag51("m5", "");
  art::InputTag inputTag52("m5", "i2");

  art::InputTag inputTags61("m6", "");
  art::InputTag inputTags62("m6", "i2");

  art::InputTag inputTagFile2(std::string("m0::PROD3"));

  art::InputTag inputTagPtrTest("ptr1");

  // Test InputTag
  assert(inputTag121 != inputTags131);
  assert(inputTag121 != inputTag122);
  assert(inputTag122 != inputTag322);
  art::InputTag inputTagTest("m3", "i2", "PROD2");
  assert(!(inputTag322 != inputTagTest));
  std::cout << inputTagTest << std::endl;

  // Test TypeLabelInstance class
  art::TypeID typeIntProduct(typeid(critictest::IntProduct));
  art::TypeID typeStringProduct(typeid(critictest::StringProduct));
  gallery::TypeLabelInstanceKey key1(typeIntProduct, "a", "b");
  assert(key1.typeID() == typeIntProduct &&
         std::string(key1.label()) == "a" &&
         std::string(key1.instance()) == "b");
  gallery::TypeLabelInstanceKey key2(typeStringProduct, "a", "b");
  gallery::TypeLabelInstanceKey key3(typeIntProduct, "c", "b");
  gallery::TypeLabelInstanceKey key4(typeIntProduct, "a", "c");
  assert(key1 < key2 || key2 < key1);
  assert(key1 < key3 && key1 < key4);

  // Read values from a ROOT file and test that we get the correct
  // products with the arbitrary values we know were put into them.
  std::vector<std::string> filenames {"test_gallery8.root",
                                      "test_gallery5.root",
                                      "test_gallery8.root",
                                      "test_gallery7.root",
                                      "test_gallery8.root"};

  gallery::Event ev(filenames, true, 1);
  assert(ev.numberOfEventsInFile() == 10);

  unsigned int iEvent = 1;
  unsigned int counter = 0;
  for( ; ! ev.atEnd(); ev.next(), ++iEvent, ++counter) {
    assert(ev.isValid());
    assert(ev.eventEntry() == counter % 10);

    if (counter / 10 == 0) assert(ev.fileEntry() == 1);
    else assert(ev.fileEntry() == 3);

    art::EventAuxiliary const& aux = ev.eventAuxiliary();
    std::cout << aux.id() << "\n";
    if (iEvent == 6) iEvent = 1; // Events numbers go from 1 to 5 and repeat 1 to 5
    if (aux.id().event() != iEvent) {
      std::cout << "Unexpected event number read from EventAuxiliary.\n";
      return 1;
    }

    std::cout << "ProcessHistoryID: " << ev.processHistoryID() << std::endl;

    gallery::Handle<art::TriggerResults> triggerResultsHandle;
    assert(!triggerResultsHandle.isValid());
    assert(!triggerResultsHandle.whyFailed());

    assert(ev.getByLabel(inputTagTriggerResults, triggerResultsHandle));
    assert(triggerResultsHandle.isValid());
    assert(!triggerResultsHandle.whyFailed());
    std::string test1 = triggerResultsHandle->parameterSetID().to_string();
    std::string test2 = (*triggerResultsHandle).parameterSetID().to_string();
    assert(!ev.getByLabel(inputTagIncorrect, triggerResultsHandle));
    assert(!triggerResultsHandle.isValid());
    assert(triggerResultsHandle.whyFailed());
    bool exceptionWasThrown = true;
    try {
      triggerResultsHandle->parameterSetID().to_string();
      exceptionWasThrown = false;
    } catch (cet::exception const&) {
    }
    assert(exceptionWasThrown);

    auto triggerResults = ev.getValidHandle<art::TriggerResults>(inputTagTriggerResults);
    std::cout << "psetID = " <<  triggerResults->parameterSetID().to_string() << "\n";
    std::string test3 = triggerResults->parameterSetID().to_string();
    assert(test1 == test3);
    assert(test2 == test3);

    auto eventIDInt = ev.getValidHandle<critictest::IntProduct>(inputTagEventID);
    assert(static_cast<unsigned int>(eventIDInt->value) == aux.id().event());

    auto intProduct111 = ev.getValidHandle<critictest::IntProduct>(inputTag111);
    assert(intProduct111->value == 111);

    auto intProduct121 = ev.getValidHandle<critictest::IntProduct>(inputTag121);
    assert((*intProduct121).value == 121);

    auto intProduct131 = ev.getValidHandle<critictest::IntProduct>(inputTag131);
    assert(intProduct131.product()->value == 131);

    auto stringProduct111 = ev.getValidHandle<critictest::StringProduct>(inputTags111);
    assert(stringProduct111->name_ == "s111");

    auto stringProduct121 = ev.getValidHandle<critictest::StringProduct>(inputTags121);
    assert(stringProduct121->name_ == "s121");

    auto stringProduct131 = ev.getValidHandle<critictest::StringProduct>(inputTags131);
    assert(stringProduct131->name_ == "s131");

    auto intProduct211 = ev.getValidHandle<critictest::IntProduct>(inputTag211);
    assert(intProduct211->value == 211);

    auto intProduct221 = ev.getValidHandle<critictest::IntProduct>(inputTag221);
    assert(intProduct221->value == 221);

    auto intProduct112 = ev.getValidHandle<critictest::IntProduct>(inputTag112);
    assert(intProduct112->value == 112);

    auto intProduct122 = ev.getValidHandle<critictest::IntProduct>(inputTag122);
    assert(intProduct122->value == 122);

    auto intProduct113 = ev.getValidHandle<critictest::IntProduct>(inputTag113);
    assert(intProduct113->value == 113);

    auto intProduct123 = ev.getValidHandle<critictest::IntProduct>(inputTag123);
    assert(intProduct123->value == 123);

    auto intProduct11 = ev.getValidHandle<critictest::IntProduct>(inputTag11);
    assert(intProduct11->value == 113);

    auto intProduct12 = ev.getValidHandle<critictest::IntProduct>(inputTag12);
    assert(intProduct12->value == 123);

    auto stringProduct11 = ev.getValidHandle<critictest::StringProduct>(inputTags11);
    assert(stringProduct11->name_ == "s113");

    auto stringProduct12 = ev.getValidHandle<critictest::StringProduct>(inputTags12);
    assert(stringProduct12->name_ == "s123");

    auto intProduct21 = ev.getValidHandle<critictest::IntProduct>(inputTag21);
    assert(intProduct21->value == 213);

    auto intProduct22 = ev.getValidHandle<critictest::IntProduct>(inputTag22);
    assert(intProduct22->value == 223);

    auto intProduct31 = ev.getValidHandle<critictest::IntProduct>(inputTag31);
    assert(intProduct31->value == 312);

    auto intProduct32 = ev.getValidHandle<critictest::IntProduct>(inputTag32);
    assert(intProduct32->value == 322);

    auto intProduct312 = ev.getValidHandle<critictest::IntProduct>(inputTag312);
    assert(intProduct312->value == 312);

    auto intProduct322 = ev.getValidHandle<critictest::IntProduct>(inputTag322);
    assert(intProduct322->value == 322);

    auto intProduct41 = ev.getValidHandle<critictest::IntProduct>(inputTag41);
    assert(intProduct41->value == 411);

    auto intProduct42 = ev.getValidHandle<critictest::IntProduct>(inputTag42);
    assert(intProduct42->value == 421);

    auto intProduct411 = ev.getValidHandle<critictest::IntProduct>(inputTag411);
    assert(intProduct411->value == 411);

    auto intProduct421 = ev.getValidHandle<critictest::IntProduct>(inputTag421);
    assert(intProduct421->value == 421);


    auto intProduct51 = ev.getValidHandle<critictest::IntProduct>(inputTag51);
    assert(intProduct51->value == 512);

    auto intProduct52 = ev.getValidHandle<critictest::IntProduct>(inputTag52);
    assert(intProduct52->value == 522);

    auto stringProduct61 = ev.getValidHandle<critictest::StringProduct>(inputTags61);
    assert(stringProduct61->name_ == "s611");

    auto stringProduct62 = ev.getValidHandle<critictest::StringProduct>(inputTags62);
    assert(stringProduct62->name_ == "s621");

    if (counter > 9) {
      auto intProductFile2 = ev.getValidHandle<critictest::IntProduct>(inputTagFile2);
      assert(intProductFile2->value == 2000);
    }

    auto ptrTestProduct = ev.getValidHandle<critictest::LitePtrTestProduct>(inputTagPtrTest);

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

    bool exceptionThrown = true;
    try {
      auto x = *ptrTestProduct->ptrIntoContainerToBeDropped;
      std::cout << x << "\n";
      exceptionThrown = false;
    } catch (art::Exception const&) {
    }
    assert(exceptionThrown);

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
    // gallery. The same behavior should exist in the full art framework.

    assert(!ptrTestProduct->nullDroppedPtr &&
           !ptrTestProduct->nullDroppedPtr.isAvailable() &&
           ptrTestProduct->nullDroppedPtr.isNull());

    // Note some of the documentation says an invalid Ptr is nonNull,
    // but that is not the way the Ptr class is written.
    assert(!ptrTestProduct->invalidPtr &&
           !ptrTestProduct->invalidPtr.isAvailable() &&
           ptrTestProduct->invalidPtr.isNull());

  }
  assert(counter == 15u);
  assert(!ev.isValid());
}
