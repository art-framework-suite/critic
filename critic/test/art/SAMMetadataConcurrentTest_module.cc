#include "boost/test/unit_test.hpp"

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Services/System/FileCatalogMetadata.h"
#include "art/Framework/Services/System/TriggerNamesService.h"
#include "cetlib/canonical_string.h"
#include "hep_concurrency/simultaneous_function_spawner.h"

#include <string>
#include <vector>

using namespace std;
using namespace std::string_literals;

using collection_type = art::FileCatalogMetadata::collection_type;

BOOST_TEST_DONT_PRINT_LOG_VALUE(collection_type::iterator)
BOOST_TEST_DONT_PRINT_LOG_VALUE(collection_type::const_iterator)

namespace {

  auto
  mockMetadata()
  {
    collection_type result;
    for (unsigned i{}; i != 10; ++i) {
      auto const str_index = std::to_string(i);
      result.emplace_back("k" + str_index, "v" + str_index);
    }
    return result;
  }

  class SAMMetadataConcurrentTest : public art::EDAnalyzer {
  public:
    struct Config {};
    using Parameters = art::EDAnalyzer::Table<Config>;
    explicit SAMMetadataConcurrentTest(Parameters const& p) : art::EDAnalyzer{p}
    {}

  private:
    void analyze(art::Event const&) override{};

    void
    beginJob() override
    {
      vector<function<void()>> tasks;
      auto mockMD = mockMetadata();
      // Cannot yet create a ServiceHandle on a different thread.
      // Will create the handle and share it across all threads.
      art::ServiceHandle<art::FileCatalogMetadata> md;
      cet::transform_all(mockMD, back_inserter(tasks), [&md](auto const& pr) {
        return [&md, &pr] { md->addMetadataString(pr.first, pr.second); };
      });
      hep::concurrency::simultaneous_function_spawner sfs{tasks};
    }

    void
    endJob() override
    {
      collection_type coll;
      art::ServiceHandle<art::FileCatalogMetadata const>()->getMetadata(coll);
      cet::sort_all(coll);

      // Check that 'file_type' and 'art.process_name' were
      // automatically inserted.  Then remove so that we can check the
      // pairs that were inserted by us.
      using value_type = decltype(coll)::value_type;

      auto const& process_name =
        art::ServiceHandle<art::TriggerNamesService const>()->getProcessName();

      auto file_type_it = cet::find_in_all(
        coll, value_type{"file_type", cet::canonical_string("unknown")});
      BOOST_TEST_REQUIRE(file_type_it != coll.cend());
      coll.erase(file_type_it);

      auto process_name_it = cet::find_in_all(
        coll,
        value_type{"art.process_name", cet::canonical_string(process_name)});
      BOOST_TEST_REQUIRE(process_name_it != coll.cend());
      coll.erase(process_name_it);

      auto const& mockMD = mockMetadata();
      BOOST_TEST(coll.size() == mockMD.size());
      for (auto c = coll.cbegin(), m = mockMD.cbegin(); c != coll.cend();
           ++c, ++m) {
        BOOST_TEST(c->first == m->first);
        BOOST_TEST(c->second == cet::canonical_string(m->second));
      }
    }
  };

} // namespace

DEFINE_ART_MODULE(SAMMetadataConcurrentTest)
