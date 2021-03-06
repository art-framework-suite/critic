#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Utilities/make_tool.h"
#include "art/test/TestObjects/ToyProducts.h"
#include "fhiclcpp/types/DelegatedParameter.h"
#include "fhiclcpp/types/Sequence.h"

using namespace fhicl;

namespace {

  class AddIntsProducer : public art::EDProducer {
  public:
    struct Config {
      DelegatedParameter tool_table{Name("addIntsImpl")};
      Sequence<std::string> labels{Name("labels")};
    };
    using Parameters = art::EDProducer::Table<Config>;

    explicit AddIntsProducer(Parameters const& p)
      : EDProducer{p}
      , labels_{p().labels()}
      , addInts_{art::make_tool<void(int&, int)>(
          p().tool_table.get<fhicl::ParameterSet>(),
          "addInts")}
    {
      produces<arttest::IntProduct>();
    }

  private:
    void produce(art::Event& e) override;

    std::vector<std::string> const labels_;
    // Test tool invocation from within module.
    std::function<void(int&, int)> addInts_;
  };

  void
  AddIntsProducer::produce(art::Event& e)
  {
    int value{};
    for (auto const& label : labels_) {
      auto const newVal = e.getProduct<arttest::IntProduct>(label).value;
      addInts_(value, newVal);
    }
    e.put(std::make_unique<arttest::IntProduct>(value));
  }

} // namespace

DEFINE_ART_MODULE(AddIntsProducer)
