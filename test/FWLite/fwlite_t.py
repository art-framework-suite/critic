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

if os.getenv('ART_INC') is not None :
  ROOT.gInterpreter.AddIncludePath(os.environ.get('ART_INC'))
else :
  ROOT.gInterpreter.AddIncludePath(os.environ.get('ART_DIR'))

if os.getenv('CETLIB_INC') is not None :
  ROOT.gInterpreter.AddIncludePath(os.environ.get('CETLIB_INC'))
else :
  ROOT.gInterpreter.AddIncludePath(os.environ.get('CETLIB_DIR'))

if os.getenv('FHICLCPP_INC') is not None :
  ROOT.gInterpreter.AddIncludePath(os.environ.get('FHICLCPP_INC'))
else :
  ROOT.gInterpreter.AddIncludePath(os.environ.get('FHICLCPP_DIR'))

ROOT.gROOT.ProcessLine('#include "canvas/FWLite/ValidHandle.h"')

ROOT.gROOT.ProcessLine('template canvas::ValidHandle<arttest::StringProduct> canvas::Event::getValidHandle<arttest::StringProduct>(canvas::InputTag const&) const;')
ROOT.gROOT.ProcessLine('template canvas::ValidHandle<art::TriggerResults> canvas::Event::getValidHandle<art::TriggerResults>(canvas::InputTag const&) const;')
ROOT.gROOT.ProcessLine('template canvas::ValidHandle<arttest::LitePtrTestProduct> canvas::Event::getValidHandle<arttest::LitePtrTestProduct>(canvas::InputTag const&) const;')

filenames = ROOT.vector(ROOT.string)()
filenames.push_back("test_fwlite5.root")
filenames.push_back("test_fwlite7.root")
ev = ROOT.canvas.Event(filenames)

inputTagEventID = ROOT.canvas.InputTag("m1", "eventID", "PROD1");
inputTagTriggerResults = ROOT.canvas.InputTag("TriggerResults", "", "PROD1");
inputTags62 = ROOT.canvas.InputTag("m6", "i2");
inputTagPtrTest = ROOT.canvas.InputTag("ptr1");

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

  triggerResults = ev.getValidHandle(ROOT.art.TriggerResults)(inputTagTriggerResults);
  print triggerResults.parameterSetID().to_string()

  stringProduct62 = ev.getValidHandle(ROOT.arttest.StringProduct)(inputTags62);
  assert(stringProduct62.name_ == "s621");

  ptrTestProduct = ev.getValidHandle(ROOT.arttest.LitePtrTestProduct)(inputTagPtrTest);

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
