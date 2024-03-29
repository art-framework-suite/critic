// ======================================================================
//
// SimpleDerivedAnalyzer
//
// ======================================================================

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/View.h"
#include "art/test/TestObjects/ToyProducts.h"
#include "canvas/Persistency/Common/PtrVector.h"
#include "canvas/Persistency/Provenance/EventID.h"
#include "cetlib_except/exception.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace arttest {
  class SimpleDerivedAnalyzer;
}

using arttest::SimpleDerivedAnalyzer;

template <typename T>
void
check_for_conversion(art::View<T> const& v)
{
  assert(v.vals().size() > 0);
}

// dummy is a type we can be sure is not used in any collections in
// the Event; no dictionary exists for it.
struct dummy {};

//--------------------------------------------------------------------
//
// Produces a SimpleProduct product instance.
//
class arttest::SimpleDerivedAnalyzer : public art::EDAnalyzer {
public:
  using SimpleDerivedProduct = std::vector<SimpleDerived>;

  struct Config {
    fhicl::Atom<std::string> input_label{fhicl::Name{"input_label"}};
    fhicl::Atom<std::string> input_label2{fhicl::Name{"input_label2"}};
    fhicl::Atom<std::size_t> nvalues{fhicl::Name{"nvalues"}};
  };
  using Parameters = Table<Config>;
  explicit SimpleDerivedAnalyzer(Parameters const& p);

private:
  void analyze(art::Event const& e) override;

  void test_getView(art::Event const& e) const;
  void test_getViewReturnFalse(art::Event const& e) const;
  void test_getViewThrowing(art::Event const& e) const;

  void test_PtrVector(art::Event const& e) const;

  static constexpr std::size_t
  expectedSize()
  {
    return 16;
  }

  template <typename DATA_TYPE>
  void test_PtrVector(art::PtrVector<DATA_TYPE> const& v,
                      art::EventNumber_t event_num,
                      std::size_t nElements = expectedSize()) const;

  art::InputTag const tagWithInstance_;
  art::InputTag const tagForCurrentProcess_;
  std::size_t const nvalues_;
  art::ProductToken<art::PtrVector<SimpleDerived>> const ptrVectorToken_;
  art::ViewToken<dummy> const dummyToken_;
  // The following tokens can be initialized from the above members.
  art::ViewToken<Simple> const simpleToken_{
    consumesView<Simple>(tagWithInstance_)};
  art::ViewToken<Simple> const simpleCurrentToken_{
    consumesView<Simple>(tagForCurrentProcess_)};
  art::ViewToken<SimpleDerived> const simpleDerivedToken_{
    consumesView<SimpleDerived>(tagWithInstance_)};
  art::ViewToken<SimpleDerived> const simpleDerivedCurrentToken_{
    consumesView<SimpleDerived>(tagForCurrentProcess_)};
}; // SimpleDerivedAnalyzer

SimpleDerivedAnalyzer::SimpleDerivedAnalyzer(Parameters const& p)
  : art::EDAnalyzer{p}
  , tagWithInstance_{p().input_label(), "derived"}
  , tagForCurrentProcess_{p().input_label(), "derived", "DEVEL"}
  , nvalues_{p().nvalues()}
  , ptrVectorToken_{consumes<art::PtrVector<SimpleDerived>>(
      art::InputTag{p().input_label2()})}
  , dummyToken_{consumesView<dummy>(art::InputTag{p().input_label()})}
{}

void
SimpleDerivedAnalyzer::analyze(art::Event const& e)
{
  test_getView(e);
  // TODO: uncomment this when getView is modified to return false
  // upon not finding the right label.
  //  test_getViewReturnFalse(e);
  test_getViewThrowing(e);
  test_PtrVector(e);
  art::View<arttest::SimpleDerived> v;
  e.getView(simpleDerivedCurrentToken_, v);
  assert(v.isValid());
  check_for_conversion(v);

  art::View<arttest::Simple> vB;
  e.getView(simpleCurrentToken_, vB);
  assert(vB.isValid());
  check_for_conversion(vB);
}

template <class T>
void
verify_elements(std::vector<T> const& ptrs,
                std::size_t sz,
                art::EventNumber_t event_num,
                std::size_t /*nvalues*/)
{
  for (std::size_t k = 0; k != sz; ++k) {
    assert((unsigned)ptrs[k]->key == sz - k + event_num);
    double expect = 1.5 * k + 100.0;
    assert(ptrs[k]->value == expect);
    assert(ptrs[k]->dummy() == 16.25);
  }
}

template <typename T>
void
test_view(art::Event const& e,
          art::ViewToken<T> const& token,
          art::ViewToken<T> const& tokenForCurrentProcess,
          std::size_t const nvalues)
{
  auto const event_num = e.id().event();
  std::vector<T const*> ptrs;
  auto sz = e.getView(token, ptrs);
  assert(sz == nvalues);
  verify_elements(ptrs, sz, event_num, nvalues);
  ptrs.clear();
  sz = e.getView(tokenForCurrentProcess, ptrs);
  assert(sz == nvalues);
  verify_elements(ptrs, sz, event_num, nvalues);

  art::View<T> v;
  assert(e.getView(token, v));
  assert(v.vals().size() == nvalues);
  verify_elements(v.vals(), v.vals().size(), event_num, nvalues);

  art::View<T> v2;
  assert(e.getView(tokenForCurrentProcess, v2));
  assert(v2.vals().size() == nvalues);
  verify_elements(v2.vals(), v2.vals().size(), event_num, nvalues);

  // Fill a PtrVector from the view...after zeroing the first element.
  v.vals().front() = nullptr;
  art::PtrVector<T> pvec;
  v.fill(pvec);
  for (std::size_t i = 0, sz = pvec.size(); i != sz; ++i) {
    assert(*pvec[i] == *v.vals()[i + 1]);
  }
  // Try to fill PtrVector from empty view
  pvec.clear();
  art::View<T> v3;
  v3.fill(pvec);
  assert(pvec.empty());
}

void
SimpleDerivedAnalyzer::test_getView(art::Event const& e) const
{
  // Make sure we can get views into products that are present.
  test_view(e, simpleToken_, simpleCurrentToken_, nvalues_);
  test_view(e, simpleDerivedToken_, simpleDerivedCurrentToken_, nvalues_);
}

void
SimpleDerivedAnalyzer::test_getViewReturnFalse(art::Event const& e) const
try {
  std::vector<int const*> ints;
  assert(e.getView("nothing with this illegal label", ints) == false);
}
catch (std::exception& exception) {
  std::cerr << exception.what() << '\n';
  assert("Unexpected exception thrown" == 0);
}
catch (...) {
  assert("Unexpected exception thrown" == 0);
}

//  Make sure attempts to get views into products that are not there fail
//  correctly.
void
SimpleDerivedAnalyzer::test_getViewThrowing(art::Event const& e) const
try {
  std::vector<dummy const*> dummies;
  e.getView(dummyToken_, dummies);
  assert("Failed to throw required exception" == 0);
}
catch (art::Exception& exception) {
  assert(exception.categoryCode() == art::errors::ProductNotFound);
}
catch (...) {
  assert("Wrong exception thrown" == 0);
}

void
SimpleDerivedAnalyzer::test_PtrVector(art::Event const& e) const
{
  auto const event_num = e.id().event();
  using product_t = art::PtrVector<arttest::SimpleDerived>;
  using base_product_t = art::PtrVector<arttest::Simple>;

  // Read the data.
  auto const& d = e.getProduct(ptrVectorToken_);
  auto const sz = d.size();
  if (sz != expectedSize()) {
    throw cet::exception("SizeMismatch")
      << "Expected a PtrVector of size " << expectedSize()
      << " but the obtained size is " << sz << '\n';
  }
  // Test the data
  test_PtrVector(d, event_num);
  // Construct from PtrVector<U>
  {
    base_product_t s{d};
    test_PtrVector(s, event_num);
    product_t p{s};
    test_PtrVector(p, event_num);
  }
  // Construct from initializer list.
  {
    auto i(d.cbegin());
    auto il = {*(i++), *i};
    base_product_t s{il};
    test_PtrVector(s, event_num, 2);
    product_t p({s.front(), s.back()});
    test_PtrVector(p, event_num, 2);
  }
  // Operator= from PtrVector<U>.
  {
    base_product_t s;
    s = d;
    test_PtrVector(s, event_num);
    product_t p;
    p = s;
    test_PtrVector(p, event_num);
  }
  // Operator= from initializer list.
  {
    auto i(d.cbegin());
    base_product_t s;
    s = {*(i++), *i};
    test_PtrVector(s, event_num, 2);
    product_t p;
    p = {s.front(), s.back()};
    test_PtrVector(p, event_num, 2);
  }
  // Assign from Ptr<U>.
  {
    base_product_t s{d};
    s.assign(1, d.front());
    test_PtrVector(s, event_num, 1);
    product_t p{s};
    p.assign(1, s.front());
    test_PtrVector(p, event_num, 1);
  }
  // Assign from iterators.
  {
    base_product_t s{d};
    s.assign(d.cbegin(), d.cend());
    test_PtrVector(s, event_num);
    product_t p{s};
    p.assign(s.cbegin(), s.cend());
    test_PtrVector(p, event_num);
  }
  // Assign from initializer list.
  {
    auto i(d.cbegin());
    base_product_t s{d};
    s.assign({*(i++), *i});
    test_PtrVector(s, event_num, 2);
    product_t p{s};
    p.assign({s.front(), s.back()});
    test_PtrVector(p, event_num, 2);
  }
  // Push back.
  {
    base_product_t s;
    s.push_back(d.front());
    test_PtrVector(s, event_num, 1);
    product_t p;
    p.push_back(s.front());
    test_PtrVector(p, event_num, 1);
  }
  // Insert from Ptr<U>.
  {
    base_product_t s({d[1]});
    s.insert(s.begin(), d.front());
    test_PtrVector(s, event_num, 2);
    base_product_t p({s.back()});
    p.insert(p.begin(), s.front());
    test_PtrVector(p, event_num, 2);
  }
  // Insert from iterators.
  {
    base_product_t s;
    s.insert(s.begin(), d.cbegin(), d.cend());
    test_PtrVector(s, event_num);
    product_t p;
    p.insert(p.begin(), s.cbegin(), s.cend());
    test_PtrVector(p, event_num);
  }
  // Erase.
  {
    base_product_t s{d};
    s.erase(s.end() - 1);
    test_PtrVector(s, event_num, expectedSize() - 1);
    s.erase(s.begin() + 1, s.end());
    test_PtrVector(s, event_num, 1);
  }
}

template <typename DATA_TYPE>
void
SimpleDerivedAnalyzer::test_PtrVector(art::PtrVector<DATA_TYPE> const& v,
                                      art::EventNumber_t const event_num,
                                      std::size_t const nElements) const
{
  auto const sz(v.size());
  if (sz != nElements) {
    throw cet::exception("SizeMismatch")
      << "Expected size " << nElements << " but got " << sz << ".\n";
  }
  auto const b = v.cbegin();
  auto const e = v.cbegin() + nElements;
  for (auto i = b; i != e; ++i) {
    std::size_t const k = i - b;
    if (static_cast<std::size_t>((*i)->key) != expectedSize() - k + event_num) {
      throw cet::exception("KeyMismatch")
        << "At position " << k << " expected key "
        << expectedSize() - k + event_num << " but obtained " << (*i)->key
        << ".\n";
    }
    assert(*i == *i); // Check operator ==.
    if (k == 0 && sz > 1) {
      assert(*i != *(i + 1));  // Check operator !=.
      assert(*(i) < *(i + 1)); // Check operator <.
    }

    double const expect{1.5 * k + 100.0};
    if ((*i)->value != expect) {
      throw cet::exception("ValueMismatch")
        << "At position " << k << " expected value " << expect
        << " but obtained " << (*i)->value << ".\n";
    }
    if ((*i)->dummy() != 16.25) {
      throw cet::exception("ValueMismatch")
        << "At position " << k << " expected dummy value " << 16.25
        << " but obtained " << (*i)->dummy() << ".\n";
    }
  }
} // test_PtrVector()

DEFINE_ART_MODULE(SimpleDerivedAnalyzer)
