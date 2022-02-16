#ifndef critic_test_art_GenericOneSimpleProductAnalyzer_h
#define critic_test_art_GenericOneSimpleProductAnalyzer_h

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "canvas/Persistency/Provenance/BranchType.h"
#include "cetlib_except/exception.h"
#include "critic/test/art/RunTimeConsumes.h"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/types/Atom.h"

#include <string>
#include <type_traits>

namespace art::test {
  template <typename V, typename P>
  class GenericOneSimpleProductAnalyzer;
  namespace detail {

    // Metaprogramming to decide whether our product P has a "value"
    // member and that is convertible to V.

    template <typename V, typename P, typename = void>
    struct has_value_member : std::false_type {};

    template <typename V, typename P>
    struct has_value_member<
      V,
      P,
      std::void_t<decltype(std::declval<V>() = std::declval<P>().value)>>
      : std::true_type {};

  } // namespace detail
} // namespace art::test

template <typename V, typename P>
class art::test::GenericOneSimpleProductAnalyzer : public EDAnalyzer {
public:
  struct Config {

    Config()
      : input_label{fhicl::Name{"input_label"}}
      , branch_type{fhicl::Name{"branch_type"}, art::InEvent}
      , require_presence{fhicl::Name{"require_presence"},
                         fhicl::Comment{
                           "If 'require_presence' is true, then the product\n"
                           "must be successfully retrievable.  If false, then\n"
                           "the product must NOT be successfully "
                           "retrievable--i.e.\n"
                           "a call to 'getByLabel' must return false."},
                         true}
      , expected_value{
          fhicl::Name{"expected_value"},
          fhicl::Comment{"The value of the following parameter is retrieved\n"
                         "only if require_presence is 'true'."},
          [this] { return require_presence(); }}
    {}

    fhicl::Atom<std::string> input_label;
    fhicl::Atom<unsigned> branch_type;
    fhicl::Atom<bool> require_presence;
    fhicl::Atom<V> expected_value;
  };

  using Parameters = Table<Config>;
  GenericOneSimpleProductAnalyzer(Parameters const& ps)
    : EDAnalyzer{ps}
    , input_label_{ps().input_label()}
    , branch_type_{BranchType(ps().branch_type())}
    , require_presence_{ps().require_presence()}
  {
    run_time_consumes<P>(consumesCollector(), branch_type_, input_label_);
    if (require_presence_) {
      value_ = ps().expected_value();
    }
  }

  void
  verify_value(BranchType const bt [[maybe_unused]], Handle<P> const& h) const
  {
    if constexpr (detail::has_value_member<V, P>::value) {
      if (h->value == value_) {
        return;
      }
      throw cet::exception("ValueMismatch")
        << "The value for \"" << input_label_ << "\", branchType \"" << bt
        << "\" is " << h->value << " but was supposed to be " << value_ << '\n';
    }
  }

  void
  analyze(Event const& e) override
  {
    if (branch_type_ != InEvent)
      return;
    auto handle = e.getHandle<P>(input_label_);
    assert(handle.isValid() == require_presence_);
    if (require_presence_) {
      verify_value(InEvent, handle);
    }
  }

  void
  endSubRun(SubRun const& sr) override
  {
    if (branch_type_ != InSubRun)
      return;
    auto handle = sr.getHandle<P>(input_label_);
    assert(handle.isValid() == require_presence_);
    if (require_presence_) {
      verify_value(InSubRun, handle);
    }
  }

  void
  endRun(Run const& r) override
  {
    if (branch_type_ != InRun)
      return;
    auto handle = r.getHandle<P>(input_label_);
    assert(handle.isValid() == require_presence_);
    if (require_presence_) {
      verify_value(InRun, handle);
    }
  }

private:
  V value_{};
  std::string input_label_;
  BranchType branch_type_;
  bool require_presence_;
};

#endif /* critic_test_art_GenericOneSimpleProductAnalyzer_h */

// Local Variables:
// mode: c++
// End:
