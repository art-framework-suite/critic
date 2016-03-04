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

ROOT.gROOT.ProcessLine('template gallery::ValidHandle<critictest::StringProduct> gallery::Event::getValidHandle<critictest::StringProduct>(art::InputTag const&) const;')
ROOT.gROOT.ProcessLine('template gallery::ValidHandle<art::TriggerResults> gallery::Event::getValidHandle<art::TriggerResults>(art::InputTag const&) const;')
ROOT.gROOT.ProcessLine('template gallery::ValidHandle<critictest::LitePtrTestProduct> gallery::Event::getValidHandle<critictest::LitePtrTestProduct>(art::InputTag const&) const;')

ROOT.gROOT.ProcessLine('template bool gallery::Event::getByLabel<art::TriggerResults>(art::InputTag const&, gallery::Handle<art::TriggerResults>&) const;')

filenames = ROOT.vector(ROOT.string)()
filenames.push_back("test_gallery5.root")
filenames.push_back("test_gallery7.root")
ev = ROOT.gallery.Event(filenames)

inputTagEventID = ROOT.art.InputTag("m1", "eventID", "PROD1");
inputTagTriggerResults = ROOT.art.InputTag("TriggerResults", "", "PROD1");
inputTagIncorrect = ROOT.art.InputTag("IncorrectLabel", "IncorrectInstance", "PROD1");
inputTags62 = ROOT.art.InputTag("m6", "i2");
inputTagPtrTest = ROOT.art.InputTag("ptr1");

iEvent = 1
ev.toBegin()
while ( not ev.atEnd()) :
  aux = ev.eventAuxiliary()
  print 'Event number = {}"'.format(aux.id().event())
  if iEvent == 6 :
    iEvent = 1; # Events numbers go from 1 to 5 and repeat 1 to 5
  assert(iEvent == aux.id().event()), "Unexpected event number in EventAuxiliary"

  eventIDInt = ev.getValidHandle(ROOT.critictest.IntProduct)(inputTagEventID);
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

  stringProduct62 = ev.getValidHandle(ROOT.critictest.StringProduct)(inputTags62);
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

  ev.next()
  iEvent += 1
