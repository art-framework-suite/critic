import os
import ROOT

# Normally, when you setup a product with UPS, an environment
# variable XX_INC (where XX is the UPS product) is defined
# and you can use that to find the header files.
# But at the moment when you build with mrb and run from
# build area the XX_INC environment variables are not defined
# and you need to check for that and use the XX_DIR variables
# instead. This should be temporary. At some point in the future
# the XX_INC variables should always be defined and always used.
# At that point in time, the following code can be simplified
# and the if-else blocks removed.

if os.getenv('GALLERY_INC') is not None :
  ROOT.gInterpreter.AddIncludePath(os.environ.get('GALLERY_INC'))
else :
  ROOT.gInterpreter.AddIncludePath(os.environ.get('GALLERY_DIR'))

if os.getenv('CANVAS_INC') is not None :
  ROOT.gInterpreter.AddIncludePath(os.environ.get('CANVAS_INC'))
else :
  ROOT.gInterpreter.AddIncludePath(os.environ.get('CANVAS_DIR'))

if os.getenv('CETLIB_INC') is not None :
  ROOT.gInterpreter.AddIncludePath(os.environ.get('CETLIB_INC'))
else :
  ROOT.gInterpreter.AddIncludePath(os.environ.get('CETLIB_DIR'))

ROOT.gROOT.ProcessLine('#include "gallery/ValidHandle.h"')
ROOT.gROOT.ProcessLine('#include "gallery/Handle.h"')

ROOT.gROOT.ProcessLine('template gallery::ValidHandle<arttest::StringProduct> gallery::Event::getValidHandle<arttest::StringProduct>(art::InputTag const&) const;')
ROOT.gROOT.ProcessLine('template gallery::ValidHandle<art::TriggerResults> gallery::Event::getValidHandle<art::TriggerResults>(art::InputTag const&) const;')
ROOT.gROOT.ProcessLine('template gallery::ValidHandle<critictest::LitePtrTestProduct> gallery::Event::getValidHandle<critictest::LitePtrTestProduct>(art::InputTag const&) const;')

ROOT.gROOT.ProcessLine('template bool gallery::Event::getByLabel<art::TriggerResults>(art::InputTag const&, gallery::Handle<art::TriggerResults>&) const;')
ROOT.gROOT.ProcessLine('template bool gallery::Event::getByLabel<art::Assns<arttest::StringProduct, int, critictest::LiteAssnTestData> >(art::InputTag const&, gallery::Handle<art::Assns<arttest::StringProduct, int, critictest::LiteAssnTestData> >&) const;')

filenames = ROOT.vector(ROOT.string)()
filenames.push_back("test_gallery5.root")
filenames.push_back("test_gallery7.root")
ev = ROOT.gallery.Event(filenames)

inputTagEventID = ROOT.art.InputTag("m1", "eventID", "PROD1");
inputTagTriggerResults = ROOT.art.InputTag("TriggerResults", "", "PROD1");
inputTagIncorrect = ROOT.art.InputTag("IncorrectLabel", "IncorrectInstance", "PROD1");
inputTags62 = ROOT.art.InputTag("m6", "i2");
inputTagPtrTest = ROOT.art.InputTag("ptr1");

inputTag111 = ROOT.art.InputTag("m1", "", "PROD1");
inputTagAssnTest1 = ROOT.art.InputTag("ptr1");
inputTagAssnTest2 = ROOT.art.InputTag("ptr1", "", "PROD3");

iEvent = 1
ev.toBegin()
while ( not ev.atEnd()) :
  aux = ev.eventAuxiliary()
  print 'Event number = {}"'.format(aux.id().event())
  if iEvent == 6 :
    iEvent = 1; # Events numbers go from 1 to 5 and repeat 1 to 5
  assert(iEvent == aux.id().event()), "Unexpected event number in EventAuxiliary"

  eventIDInt = ev.getValidHandle(ROOT.arttest.IntProduct)(inputTagEventID);
  assert(eventIDInt.value == aux.id().event());

  triggerResultsHandle = ROOT.gallery.Handle(ROOT.art.TriggerResults)();
  assert( not triggerResultsHandle.isValid() );
  assert( not triggerResultsHandle.whyFailed() );

  assert(ev.getByLabel(inputTagTriggerResults, triggerResultsHandle));
  assert(triggerResultsHandle.isValid());
  assert( not triggerResultsHandle.whyFailed() );
  test1 = triggerResultsHandle.parameterSetID().to_string();
  test2 = (triggerResultsHandle).parameterSetID().to_string();
  assert( not ev.getByLabel(inputTagIncorrect, triggerResultsHandle) );
  assert( not triggerResultsHandle.isValid());
  assert(triggerResultsHandle.whyFailed());

  # This will throw
  #test4 = triggerResultsHandle.parameterSetID().to_string();

  triggerResults = ev.getValidHandle(ROOT.art.TriggerResults)(inputTagTriggerResults);
  print triggerResults.parameterSetID().to_string()

  test3 = triggerResults.parameterSetID().to_string();
  assert(test1 == test3);
  assert(test2 == test3);

  stringProduct62 = ev.getValidHandle(ROOT.arttest.StringProduct)(inputTags62);
  assert(stringProduct62.name_ == "s621");

  ptrTestProduct = ev.getValidHandle(ROOT.critictest.LitePtrTestProduct)(inputTagPtrTest);

  eventInt = aux.id().event();
  value = ptrTestProduct.ptrInt1.get()[0]
  print value
  assert(value == 111 + eventInt);

  assert(ptrTestProduct.ptrInt1.isAvailable());
  assert(ptrTestProduct.ptrInt2.get()[0] == 122);
  assert(ptrTestProduct.ptrInt3.get()[0] == 133);

  assert(ptrTestProduct.ptrSimple1.get().key == 111);
  assert(ptrTestProduct.ptrSimple2.get().key == 122);
  print ptrTestProduct.ptrSimple2.get().key
  assert(ptrTestProduct.ptrSimple3.get().key == 133);
  assert(ptrTestProduct.ptrSimple3.get().dummy() == 16.25);

  assert(ptrTestProduct.ptrSimpleDerived1.get().key == 111);
  assert(ptrTestProduct.ptrSimpleDerived2.get().key == 122);
  assert(ptrTestProduct.ptrSimpleDerived3.get().key == 133);
  assert(ptrTestProduct.ptrSimpleDerived3.get().dummy() == 16.25);

  assert(ptrTestProduct.ptrVectorInt.at(0).get()[0] == 111 + eventInt);
  assert(ptrTestProduct.ptrVectorInt.isAvailable());
  assert(ptrTestProduct.ptrVectorInt.at(1).get()[0] == 131);

  assert(ptrTestProduct.ptrVectorSimple.at(0).get().key == 111);
  assert(ptrTestProduct.ptrVectorSimple.at(1).get().key == 131);
  assert(ptrTestProduct.ptrVectorSimple.at(1).get().dummy() == 16.25);

  assert(ptrTestProduct.ptrVectorSimpleDerived.at(0).get().key == 111);
  assert(ptrTestProduct.ptrVectorSimpleDerived.at(1).get().key == 131);
  assert(ptrTestProduct.ptrVectorSimpleDerived.at(1).get().dummy() == 16.25);

  assert(ptrTestProduct.ptrInt1 and
         ptrTestProduct.ptrInt1.isAvailable() and
         not ptrTestProduct.ptrInt1.isNull());

  assert(not ptrTestProduct.ptrIntoContainerToBeDropped and
         not ptrTestProduct.ptrIntoContainerToBeDropped.isAvailable() and
         not ptrTestProduct.ptrIntoContainerToBeDropped.isNull());

  # This will throw because the product was dropped.
  # x = ptrTestProduct.ptrIntoContainerToBeDropped.get();

  assert(not ptrTestProduct.nullPtr and
         ptrTestProduct.nullPtr.isAvailable() and
         ptrTestProduct.nullPtr.isNull());

  assert(not ptrTestProduct.nullDroppedPtr and
         not ptrTestProduct.nullDroppedPtr.isAvailable() and
         ptrTestProduct.nullDroppedPtr.isNull());

  assert(not ptrTestProduct.invalidPtr and
         not ptrTestProduct.invalidPtr.isAvailable() and
         ptrTestProduct.invalidPtr.isNull());

  # This is work in progress. An attempt needs to be made to convert this
  # to python and get it working.
  #assnsABHandle1 = ROOT.gallery.Handle(ROOT.art.Assns(ROOT.arttest.StringProduct, int, ROOT.critictest.LiteAssnTestData));
  #ev.getByLabel(inputTagAssnTest1, assnsABHandle1);

  #gallery::Handle<art::Assns<arttest::StringProduct, int, critictest::LiteAssnTestData> > assnsABHandle2;
  #ev.getByLabel(inputTagAssnTest2, assnsABHandle2);

  #gallery::Handle<art::Assns<int, arttest::StringProduct, critictest::LiteAssnTestData> > assnsBAHandle3;
  #ev.getByLabel(inputTagAssnTest1, assnsBAHandle3);

  #gallery::Handle<art::Assns<int, arttest::StringProduct, critictest::LiteAssnTestData> > assnsBAHandle4;
  #ev.getByLabel(inputTagAssnTest2, assnsBAHandle4);

  #assert(assnsABHandle1.isValid() &&
  #       assnsABHandle2.isValid() &&
  #       assnsBAHandle3.isValid() &&
  #       assnsBAHandle4.isValid());

  #// handles 1 and 2 should be the same
  #// handles 3 and 4 should be the same
  #// 1 and 3 are also the same except first and second of the pair<Ptr,Ptr>
  #// stored in the Assns are reversed.

  #if (ev.fileEntry() == 1) {
  #  assert((*assnsABHandle1)[0].first->name_ == std::string("s111"));
  #  assert(*(*assnsABHandle2)[0].second == 121);
  #  assert(assnsABHandle1->data(0).label == std::string("A"));

  #  assert((*assnsABHandle1)[1].first->name_ == std::string("s121"));
  #  assert(*(*assnsABHandle2)[1].second == 131);
  #  assert(assnsABHandle1->data(1).label == std::string("B"));
  #} else {
  #  assert(assnsABHandle1->at(0).first->name_ == std::string("s131"));
  #  assert(*(*assnsABHandle2)[0].second == 131);
  #  assert(assnsABHandle1->data(0).label == std::string("C"));

  #  assert((*assnsABHandle1)[1].first->name_ == std::string("s111"));
  #  assert(*(*assnsABHandle2)[1].second == 121);
  #  assert(assnsABHandle1->data(1).label == std::string("D"));
  #}

  #if (ev.fileEntry() == 1) {
  #  assert((*assnsBAHandle3)[0].second->name_ == std::string("s111"));
  #  assert(*(*assnsBAHandle4)[0].first == 121);
  #  assert(assnsBAHandle3->data(0).label == std::string("A"));

  #  assert((*assnsBAHandle3)[1].second->name_ == std::string("s121"));
  #  assert(*(*assnsBAHandle4)[1].first == 131);
  #  assert(assnsBAHandle3->data(1).label == std::string("B"));
  #} else {
  #  assert(assnsBAHandle3->at(0).second->name_ == std::string("s131"));
  #  assert(*(*assnsBAHandle4)[0].first == 131);
  #  assert(assnsBAHandle3->data(0).label == std::string("C"));

  #  assert((*assnsBAHandle3)[1].second->name_ == std::string("s111"));
  #  assert(*(*assnsBAHandle4)[1].first == 121);
  #  assert(assnsBAHandle3->data(1).label == std::string("D"));
  #}

  #gallery::Handle<std::vector<arttest::StringProduct> > hVStringProduct;
  #ev.getByLabel(inputTag111, hVStringProduct);
  #assert(hVStringProduct.isValid());

  #art::FindOne<int, critictest::LiteAssnTestData> findOne(hVStringProduct, ev, inputTagAssnTest1);
  #assert(findOne.at(0).isValid());
  #if (ev.fileEntry() == 1) {
  #  assert(findOne.at(0).ref() == 121);
  #  assert(findOne.at(1).ref() == 131);
  #  assert(findOne.data(0).ref().label == std::string("A"));
  #  assert(findOne.data(1).ref().label == std::string("B"));
  #} else {
  #  assert(findOne.at(0).ref() == 121);
  #  assert(findOne.at(2).ref() == 131);
  #  assert(findOne.data(0).ref().label == std::string("D"));
  #  assert(findOne.data(2).ref().label == std::string("C"));
  #}

  #gallery::Handle<std::vector<int> > hB;
  #ev.getByLabel(inputTag111, hB);
  #assert(hB.isValid());

  #art::FindOne<arttest::StringProduct, critictest::LiteAssnTestData> findOneBA(hB, ev, inputTagAssnTest1);
  #assert(findOneBA.at(1).isValid());
  #if (ev.fileEntry() == 1) {
  #  assert(findOneBA.at(1).ref() == arttest::StringProduct(std::string("s111")));
  #  assert(findOneBA.at(2).ref() == arttest::StringProduct(std::string("s121")));
  #  assert(findOneBA.data(1).ref().label == std::string("A"));
  #  assert(findOneBA.data(2).ref().label == std::string("B"));
  #} else {
  #  assert(findOneBA.at(1).ref() == arttest::StringProduct(std::string("s111")));
  #  assert(findOneBA.at(2).ref() == arttest::StringProduct(std::string("s131")));
  #  assert(findOneBA.data(1).ref().label == std::string("D"));
  #  assert(findOneBA.data(2).ref().label == std::string("C"));
  #}

  ev.next()
  iEvent += 1
