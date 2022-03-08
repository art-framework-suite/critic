////////////////////////////////////////////////////////////////////////
// Class:       RandomNumberSaveTest
// Module Type: filter
// File:        RandomNumberSaveTest_module.cc
//
// Module for testing (with appropriate configuration) both aspects of
// random number state storage / retrieval: file and data product.
//
// Generated at Wed Oct 12 11:44:35 2011 by Chris Green using artmod
// from art v1_00_00.
////////////////////////////////////////////////////////////////////////
#include "boost/test/unit_test.hpp"

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "canvas/Utilities/Exception.h"
#include "cetlib/container_algorithms.h"
#include "fhiclcpp/types/Atom.h"

#include "CLHEP/Random/RandFlat.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <vector>

using namespace fhicl;

namespace arttest {
  class RandomNumberSaveTest;
}

class arttest::RandomNumberSaveTest : public art::EDProducer {
public:
  using prod_t = std::vector<size_t>;

  struct Config {
    Atom<long> seed{Name{"seed"}, -1};
    Atom<size_t> dieOnNthEvent{Name{"dieOnNthEvent"}, 0};
    Atom<bool> genUnsaved{Name{"genUnsaved"}, true};
  };
  using Parameters = Table<Config>;
  explicit RandomNumberSaveTest(Parameters const& p);

private:
  void produce(art::Event& e) override;

  std::string const myLabel_;
  CLHEP::RandFlat dist_;
  size_t const dieOnNthEvent_;
  size_t eventN_{};
  bool const genUnsaved_;
};

std::ostream&
operator<<(std::ostream& os, arttest::RandomNumberSaveTest::prod_t const& v)
{
  cet::copy_all(
    v,
    std::ostream_iterator<arttest::RandomNumberSaveTest::prod_t::value_type>(
      os, ", "));
  return os;
}

arttest::RandomNumberSaveTest::RandomNumberSaveTest(Parameters const& p)
  : EDProducer{p}
  , myLabel_{p.get_PSet().get<std::string>("module_label")}
  , dist_{createEngine(p().seed())}
  , dieOnNthEvent_{p().dieOnNthEvent()}
  , genUnsaved_{p().genUnsaved()}
{
  produces<prod_t>();
}

void
arttest::RandomNumberSaveTest::produce(art::Event& e)
{
  if (++eventN_ == dieOnNthEvent_) {
    throw art::Exception(art::errors::Configuration)
      << "Throwing while processing ordinal event " << eventN_
      << " as requested.\n";
  }
  prod_t nums;
  constexpr size_t nums_size{5};
  constexpr size_t random_range{1000};
  nums.reserve(nums_size);
  generate_n(std::back_inserter(nums), nums_size, [this] {
    return dist_.fireInt(random_range);
  });
  std::cerr << "nums: " << nums << "\n";
  if (auto hp = e.getHandle<prod_t>(myLabel_)) {
    std::cerr << "(*hp): " << *hp << "\n";
    // Reading.
    BOOST_TEST_REQUIRE((*hp) == nums);
    if (genUnsaved_) {
      // Intentionally attempt to throw the sequence off for the next event.
      dist_.fireInt(random_range);
    }
  } else {
    // Writing.
    e.put(std::make_unique<prod_t>(nums));
  }
}

DEFINE_ART_MODULE(arttest::RandomNumberSaveTest)
