////////////////////////////////////////////////////////////////////////
// Class:       FindManySpeedTestProducer
// Module Type: producer
// File:        FindManySpeedTestProducer_module.cc
//
// Generated at Wed Sep 10 13:46:46 2014 by Christopher Green using artmod
// from cetpkgsupport v1_07_01.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/test/TestObjects/ToyProducts.h"
#include "canvas/Persistency/Common/Assns.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include <chrono>
#include <memory>
#include <random>

namespace art::test {
  class FindManySpeedTestProducer : public EDProducer {
  public:
    explicit FindManySpeedTestProducer(fhicl::ParameterSet const& p);

  private:
    void produce(Event& e) override;

    size_t const nTracks_;
    size_t const nHits_;
    size_t const pmHitsPerTrack_;
    std::mt19937_64 gen_;
  };
}

art::test::FindManySpeedTestProducer::FindManySpeedTestProducer(
  fhicl::ParameterSet const& p)
  : EDProducer{p}
  , nTracks_{p.get<size_t>("nTracks")}
  , nHits_{p.get<size_t>("nHits")}
  , pmHitsPerTrack_{p.get<size_t>("pmHitsPerTrack")}
  , gen_{p.get<size_t>(
      "randomSeed",
      std::chrono::system_clock::now().time_since_epoch().count())}
{
  produces<std::vector<arttest::Hit>>();
  produces<std::vector<arttest::Track>>();
  produces<Assns<arttest::Track, arttest::Hit>>();
}

void
art::test::FindManySpeedTestProducer::produce(Event& e)
{
  // Hits.
  auto hits = std::make_unique<std::vector<arttest::Hit>>(nHits_);
  std::iota(hits->begin(), hits->end(), 0ul);
  auto hitsH = e.put(std::move(hits));

  // Tracks.
  auto tracks = std::make_unique<std::vector<arttest::Track>>(nTracks_);
  std::iota(tracks->begin(), tracks->end(), 0ul);
  auto tracksH = e.put(std::move(tracks));

  // Assns.
  auto assns = std::make_unique<Assns<arttest::Hit, arttest::Track>>();
  std::poisson_distribution<size_t> pdist(pmHitsPerTrack_);
  std::uniform_int_distribution<size_t> udist(0, nHits_);
  auto pdice = std::bind(pdist, gen_);
  auto udice = std::bind(udist, gen_);
  for (size_t iTrack = 0; iTrack != nTracks_; ++iTrack) {
    Ptr const trackPtr{tracksH, iTrack};
    for (size_t i = 0, nHitsTrack = pdice(); i != nHitsTrack; ++i) {
      assns->addSingle({hitsH, udice()}, trackPtr);
    }
  }
  e.put(std::move(assns));
}

DEFINE_ART_MODULE(art::test::FindManySpeedTestProducer)
