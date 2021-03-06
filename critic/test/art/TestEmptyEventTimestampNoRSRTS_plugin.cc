////////////////////////////////////////////////////////////////////////
// Class:       TestEmptyEventTimestamp
// Module Type: emptyeventtimestampplugin
// File:        TestEmptyEventTimestamp_plugin.cc
//
// Generated at Thu Aug 21 07:53:15 2014 by Christopher Green using artmod
// from cetpkgsupport v1_06_02.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EmptyEventTimestampPlugin.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "canvas/Persistency/Provenance/Timestamp.h"

#include <iostream>

namespace arttest {
  class TestEmptyEventTimestamp;
}

class arttest::TestEmptyEventTimestamp : public art::EmptyEventTimestampPlugin {
public:
  explicit TestEmptyEventTimestamp(fhicl::ParameterSet const& p);
  // The destructor generated by the compiler is fine for classes
  // without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  TestEmptyEventTimestamp(TestEmptyEventTimestamp const&) = delete;
  TestEmptyEventTimestamp(TestEmptyEventTimestamp&&) = delete;
  TestEmptyEventTimestamp& operator=(TestEmptyEventTimestamp const&) = delete;
  TestEmptyEventTimestamp& operator=(TestEmptyEventTimestamp&&) = delete;

  // Required functions.
  art::Timestamp eventTimestamp(art::EventID const& eid) override;

  // Selected optional functions.
  void beginJob() override;
  void beginRun(art::Run const& r) override;
#ifndef TEST_USE_LAST_EVENT_TIMESTAMP
  art::Timestamp beginRunTimestamp(art::RunID const& rid) override;
#endif
  void beginSubRun(art::SubRun const& sr) override;
#ifndef TEST_USE_LAST_EVENT_TIMESTAMP
  art::Timestamp beginSubRunTimestamp(art::SubRunID const& srid) override;
#endif
  void endJob() override;

private:
  // Declare member data here.
  art::TimeValue_t timeCount_;
};

namespace {
  void
  announce(std::string const& caller)
  {
    std::cout << "TestEmptyEventTimestamp: in " << caller << '.' << std::endl;
  }
} // namespace

arttest::TestEmptyEventTimestamp::TestEmptyEventTimestamp(
  fhicl::ParameterSet const& p)
  : EmptyEventTimestampPlugin(p), timeCount_(0ul)
{
  announce("constructor");
}

art::Timestamp
arttest::TestEmptyEventTimestamp::eventTimestamp(art::EventID const&)
{
  announce("eventTimestamp");
  return timeCount_++;
}

void
arttest::TestEmptyEventTimestamp::beginJob()
{
  announce("beginJob");
}

void
arttest::TestEmptyEventTimestamp::beginRun(art::Run const& r)
{
  announce("beginRun");
  std::cout << "r.beginTime() =  " << r.beginTime().value() << std::endl;
}

#ifndef TEST_USE_LAST_EVENT_TIMESTAMP
art::Timestamp
arttest::TestEmptyEventTimestamp::beginRunTimestamp(art::RunID const&)
{
  announce("beginRunTimestamp");
  return (timeCount_ - 1);
}
#endif

void
arttest::TestEmptyEventTimestamp::beginSubRun(art::SubRun const& sr)
{
  announce("beginSubRun");
  std::cout << "sr.beginTime() = " << sr.beginTime().value() << std::endl;
}

#ifndef TEST_USE_LAST_EVENT_TIMESTAMP
art::Timestamp
arttest::TestEmptyEventTimestamp::beginSubRunTimestamp(art::SubRunID const&)
{
  announce("beginSubRunTimestamp");
  return (timeCount_ - 1);
}
#endif

void
arttest::TestEmptyEventTimestamp::endJob()
{
  announce("endJob");
}

DEFINE_ART_EMPTYEVENTTIMESTAMP_PLUGIN(arttest::TestEmptyEventTimestamp)
