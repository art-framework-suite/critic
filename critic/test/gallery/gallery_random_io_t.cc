#include "gallery/Event.h"
#include "canvas/Persistency/Provenance/EventAuxiliary.h"
#include "canvas/Persistency/Provenance/EventID.h"

#include <cassert>
#include <string>
#include <vector>

#include <iostream>

using std::string;
using std::vector;

int main(int argc, char* argv[]) {
  vector<string> args(argv+1, argv+argc);
  assert(args.size() == 1);

  gallery::Event ev(args);
  // Make sure the file is this size we expect.
  assert(ev.numberOfEventsInFile() == 5);
  // Make sure the event knows what file we're on: the one is slot 0
  // of the input vector.
  assert(ev.fileEntry() == 0);
  // Make sure we start on the right event: the one at entry 0 in the
  // TTree.
  assert(ev.eventEntry() == 0);
  assert(ev.eventAuxiliary().id() == art::EventID(1,0,1));

  // Now move around some, and make sure we're where we expect to be.
  ev.next();
  assert(ev.isValid());
  assert(ev.eventEntry() == 1);
  assert(ev.eventAuxiliary().id() == art::EventID(1,0,2));

  ev.previous();
  assert(ev.isValid());
  assert(ev.eventEntry() == 0);
  assert(ev.eventAuxiliary().id() == art::EventID(1,0,1));
  ++ev;
  --ev;
  assert(ev.eventEntry() == 0);
  assert(ev.eventAuxiliary().id() == art::EventID(1,0,1));

  // Drive to the end of the file ...
  ev.next(); // event 2
  assert(ev.isValid());
  ev.next(); // event 3
  assert(ev.isValid());
  ev.next(); // event 4
  assert(ev.isValid());
  ev.next(); // event 5
  assert(ev.isValid());
  ev.next(); // and we're done

  // If we have  reached the end, we can't decrement.  This is similar
  // to the rules for C++ standard library containers;
  assert(ev.atEnd());
  assert(!ev.isValid());
  try  { ev.previous(); assert("Failed to throw required exception!" == nullptr) ; }
  catch ( art::Exception& x ) { assert(x.categoryCode() == art::errors::LogicError); }
  catch (...) { assert("Threw wrong type of exception!" == nullptr); }
}
