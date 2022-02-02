// ======================================================================
//
// PtrVectorAnalyzer
//
// ======================================================================

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "canvas/Persistency/Common/PtrVector.h"
#include "canvas/Utilities/InputTag.h"
#include "cetlib_except/exception.h"
#include "fhiclcpp/ParameterSet.h"

#include <string>
#include <vector>

namespace art::test {

  class PtrVectorAnalyzer : public EDAnalyzer {
  public:
    struct Config {
      fhicl::Atom<std::string> ints_label{fhicl::Name{"int_vector_label"}};
      fhicl::Atom<std::string> ptrs_label{fhicl::Name{"ptr_vector_label"}};
      fhicl::Atom<unsigned> nvalues{fhicl::Name{"nvalues"}};
    };
    using Parameters = Table<Config>;
    explicit PtrVectorAnalyzer(Parameters const& p)
      : EDAnalyzer{p}
      , ints_tag_{p().ints_label()}
      , ptrs_tag_{p().ptrs_label()}
      , nvalues_{p().nvalues()}
    {}

  private:
    void
    analyze(Event const& e) override
    {
      using product_t = PtrVector<int>;

      // Parent product
      auto const* ints_address =
        e.getValidHandle<std::vector<int>>(ints_tag_).product();

      // Container of Ptrs into parent product
      auto const& int_ptrs = e.getProduct<product_t>(ptrs_tag_);

      size_t sz = int_ptrs.size();
      if (sz != nvalues_) {
        throw cet::exception("SizeMismatch")
          << "Expected a PtrVector of size " << nvalues_
          << " but the obtained size is " << sz << '\n';
      }

      int value = e.id().event();
      size_t count = 0;
      for (auto const& ptr : int_ptrs) {
        if (*ptr != value) {
          throw cet::exception("ValueMismatch")
            << "At position " << count << " expected value " << value
            << " but obtained " << *ptr << '\n';
        }
        auto getter = e.productGetter(ptr.id());
        auto const* parent_address = &ptr.parentAs<std::vector>(getter);
        if (parent_address != ints_address) {
          throw cet::exception("ParentMismatch")
            << "The address of the parent collection does not match what has"
               "been returned.\n";
        }
        ++value;
        ++count;
      }
      if (count != sz) {
        throw cet::exception("CountMismatch")
          << "Expected to iterate over " << sz << " values, but found " << count
          << '\n';
      }

      // Make a copy of the PtrVector, so we can call sort on it.
      product_t local(int_ptrs);
      // Make sure we're not sorted yet...
      sz = local.size();
      assert(sz > 1);

      local.sort();
      assert(sz == local.size());
      for (size_t i = 1; i != sz; ++i)
        assert(*local[i - 1] < *local[i]);

      std::greater<int> gt;
      local.sort(gt);
      assert(sz == local.size());
      for (size_t i = 1; i != sz; ++i)
        assert(*local[i - 1] > *local[i]);

      // Make a new PtrVector so we can range-insert into it.
      product_t insert_test;
      auto half_size = int_ptrs.size() / 2;
      insert_test.insert(
        insert_test.begin(), int_ptrs.cbegin(), int_ptrs.cbegin() + half_size);
      auto it = insert_test.insert(
        insert_test.end(), int_ptrs.cbegin() + half_size, int_ptrs.cend());
      assert(it == insert_test.begin() + half_size);
      assert(insert_test.size() == int_ptrs.size());
      it = insert_test.insert(it, int_ptrs.cbegin(), int_ptrs.end());
      assert(it == insert_test.begin() + half_size);
      assert(insert_test.size() == int_ptrs.size() * 2);
    } // analyze()

    InputTag ints_tag_;
    InputTag ptrs_tag_;
    unsigned nvalues_;

  }; // PtrVectorAnalyzer
}

DEFINE_ART_MODULE(art::test::PtrVectorAnalyzer)
