// vim: set sw=2 expandtab :
#include "art/Framework/Core/SharedFilter.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/SubRun.h"
#include "critic/test/art/product-aggregation/physics-workflow/CalibConstants.h"
#include "critic/test/art/product-aggregation/physics-workflow/TrackEfficiency.h"
#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Name.h"
#include "fhiclcpp/types/TupleAs.h"

#include <atomic>
#include <memory>
#include <string>
#include <vector>

using namespace art;
using namespace std;

namespace arttest {

  class Reconstruction : public SharedFilter {
  public:
    struct Config {
      fhicl::TupleAs<InputTag(string)> inputTag{fhicl::Name("inputTag")};
      fhicl::Atom<double> threshold{fhicl::Name("energyThreshold")};
    };
    using Parameters = Table<Config>;
    explicit Reconstruction(Parameters const&, ProcessingFrame const&);

  private:
    void beginSubRun(SubRun&, ProcessingFrame const&) override;
    void endSubRun(SubRun&, ProcessingFrame const&) override;
    bool filter(Event&, ProcessingFrame const&) override;

    double const threshold_;
    ProductToken<vector<double>> const particleEnergiesTkn_;
    std::atomic<unsigned> numerator_{};
    std::atomic<unsigned> denominator_{};
  };

  Reconstruction::Reconstruction(Parameters const& config,
                                 ProcessingFrame const&)
    : SharedFilter{config}
    , threshold_{config().threshold()}
    , particleEnergiesTkn_{consumes<vector<double>>(config().inputTag())}
  {
    produces<arttest::CalibConstants, InSubRun>("CalibConstants");
    produces<arttest::TrackEfficiency, InSubRun>("TrackEfficiency");
    async<InEvent>();
  }

  void
  Reconstruction::beginSubRun(SubRun& sr, ProcessingFrame const&)
  {
    sr.put(make_unique<arttest::CalibConstants>(sr.subRun()),
           "CalibConstants",
           fullSubRun());
  }

  void
  Reconstruction::endSubRun(SubRun& sr, ProcessingFrame const&)
  {
    sr.put(make_unique<arttest::TrackEfficiency>(numerator_.load(),
                                                 denominator_.load()),
           "TrackEfficiency",
           subRunFragment());
    numerator_ = 0u;
    denominator_ = 0u;
  }

  bool
  Reconstruction::filter(Event& e, ProcessingFrame const&)
  {
    auto const& particleEnergies = e.getProduct(particleEnergiesTkn_);
    bool pass = false;
    for (auto const& val : particleEnergies) {
      if (val >= threshold_) {
        pass = true;
        ++numerator_;
        break;
      }
    }
    ++denominator_;
    return pass;
  }

} // namespace arttest

DEFINE_ART_MODULE(arttest::Reconstruction)
