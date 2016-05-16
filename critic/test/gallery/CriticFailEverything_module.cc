#include "art/Framework/Core/EDFilter.h"
#include "art/Framework/Core/ModuleMacros.h"

namespace fhicl {
  class ParameterSet;
}

namespace art {
  class Event;
}

namespace critictest
{

  class CriticFailEverything : public art::EDFilter
  {
  public:
    explicit CriticFailEverything(fhicl::ParameterSet const&);
    virtual ~CriticFailEverything();
    virtual bool filter(art::Event&);
  };

  CriticFailEverything::CriticFailEverything(fhicl::ParameterSet const&) {
  }

  CriticFailEverything::~CriticFailEverything() {
  }

  bool CriticFailEverything::filter(art::Event&) {
    return false;
  }
}

using critictest::CriticFailEverything;
DEFINE_ART_MODULE(CriticFailEverything)
