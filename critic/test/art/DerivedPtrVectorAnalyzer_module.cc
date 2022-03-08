#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "art/test/TestObjects/ToyProducts.h"
#include "canvas/Persistency/Common/PtrVector.h"
#include "fhiclcpp/types/Atom.h"

#include <cassert>

using namespace fhicl;

namespace art::test {
  class DerivedPtrVectorAnalyzer : public EDAnalyzer {
  public:
    struct Config {
      Atom<InputTag> input_tag{Name{"input_tag"}};
      Atom<bool> require_presence{Name{"require_presence"}};
    };
    using Parameters = Table<Config>;
    explicit DerivedPtrVectorAnalyzer(Parameters const&);

  private:
    void analyze(Event const& e) override;

    using product_t = PtrVector<arttest::SimpleDerived>;
    ProductToken<product_t> const productToken_;
    bool const requirePresence_;
  };

  DerivedPtrVectorAnalyzer::DerivedPtrVectorAnalyzer(Parameters const& p)
    : EDAnalyzer{p}
    , productToken_{consumes<product_t>(p().input_tag())}
    , requirePresence_{p().require_presence()}
  {}

  void
  DerivedPtrVectorAnalyzer::analyze(Event const& e)
  {
    auto h = e.getHandle(productToken_);
    assert(requirePresence_ == h.isValid());
  }
}

DEFINE_ART_MODULE(art::test::DerivedPtrVectorAnalyzer)
