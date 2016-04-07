
// Started as a copy of AssnsAnalyzer_module.cc. Modified
// to work with gallery and critic. The checks that are
// made remain about the same. The main difference being
// the checks related to View were deleted.

#include "canvas/Persistency/Common/Assns.h"
#include "canvas/Persistency/Common/detail/is_handle.h"
#include "canvas/Persistency/Common/FindMany.h"
#include "canvas/Persistency/Common/FindManyP.h"
#include "canvas/Persistency/Common/FindOne.h"
#include "canvas/Persistency/Common/FindOneP.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "canvas/Utilities/InputTag.h"
#include "cetlib/maybe_ref.h"
#include "cetlib/quiet_unit_test.hpp"
#include "gallery/Event.h"
#include "gallery/Handle.h"
#include "test/CriticTestObjects/LiteAssnTestData.h"
#include "test/CriticTestObjects/ToyProducts.h"

#include <string>

namespace critictest {

  class TestFindOne {
  public:

    inline TestFindOne(std::string const& inputLabel,
                       bool testAB,
                       bool testBA,
                       bool bCollMissing);

    template <template <typename, typename> class FO>
    void testOne(gallery::Event const & e) const;

    template <template <typename, typename> class FM>
    void testMany(gallery::Event const & e) const;

  private:

    std::string inputLabel_;
    bool testAB_;
    bool testBA_;
    bool bCollMissing_;
  };
}

namespace {

  typedef int A_t;
  typedef critictest::StringProduct B_t;
  typedef art::Assns<int, critictest::StringProduct, critictest::LiteAssnTestData> AssnsAB_t;
  typedef art::Assns<critictest::StringProduct, int, critictest::LiteAssnTestData> AssnsBA_t;
  typedef art::Assns<int, critictest::StringProduct> AssnsABV_t;
  typedef art::Assns<critictest::StringProduct, int> AssnsBAV_t;

  // function template to allow us to dereference both maybe_ref<T>
  // objects and objects that have an operator*.
  template <class R, class W> R const& dereference(W const& wrapper);

  // Common case, can dereference (eg Ptr<T>).
  template <typename T, template <typename> class WRAP>
  typename std::enable_if<boost::has_dereference<WRAP<T> >::value, T const &>::type
  dereference(WRAP<T> const & wrapper) {
     return *wrapper;
  }

  // maybe_ref<T>.
  template <typename T, template <typename> class WRAP>
  typename std::enable_if<std::is_same<WRAP<T>, cet::maybe_ref<T> >::value, T const &>::type
  dereference(WRAP<T> const & wrapper) {
     return wrapper.ref();
  }

  // These are the expected answers; they are global data used in
  // check_get_one_impl.
  char const* const X[] = { "zero", "one", "two" };
  char const* const A[] = { "A", "B", "C" };
  size_t const AI[] = { 2, 0, 1 }; // Order in Acoll
  size_t const BI[] = { 1, 2, 0 }; // Order in Bcoll

  // check_get_one_impl is the common implementation for the check_get
  // functions.
  template <typename I, typename D, typename F, typename FV>
  void
  check_get_one_impl(F const & fA, FV const & fAV) {
    I item;
    D data;
    for (size_t i = 0; i < 3; ++i) {
      fA.get(i, item, data);
      BOOST_CHECK_EQUAL(dereference(item), BI[i]);
      BOOST_CHECK_EQUAL(dereference(data).d1, AI[i]);
      BOOST_CHECK_EQUAL(dereference(data).d2, i);
      fAV.get(i, item);
      BOOST_CHECK_EQUAL(dereference(item), BI[i]);
    }
  }

  // check_get tests that both fA and fAV both contain the
  // expected information, as recorded in the global variables.
  // Each looks like:
  //    template <typename T, typename D, template <typename, typename> class F>
  //    void
  //    check_get(F<T,D> const&, F<T,void> const&);
  // with complications using enable_if to control which overload is
  // called in a given case.

  // check_get specialized for FindOne
  template <typename T, typename D,
            template <typename, typename> class FO>
  typename std::enable_if<std::is_same<FO<T, void>, art::FindOne<T, void> >::value>::type
  check_get(FO<T, D> const & fA,
            FO<T, void> const & fAV) {
    typedef cet::maybe_ref<typename FO<T, void>::assoc_t const> item_t;
    typedef cet::maybe_ref<typename FO<T, D>::data_t const> data_t;
    check_get_one_impl<item_t, data_t>(fA, fAV);
  }

  // check_get specialized for FindOneP
  template <typename T, typename D,
            template <typename, typename> class FO>
  typename std::enable_if<std::is_same<FO<T, void>, art::FindOneP<T, void> >::value>::type
  check_get(FO<T, D> const & fA,
            FO<T, void> const & fAV) {
    typedef art::Ptr<typename FO<T, void>::assoc_t> item_t;
    typedef cet::maybe_ref<typename FO<T, D>::data_t const> data_t;
    check_get_one_impl<item_t, data_t>(fA, fAV);
  }

  // check_get specialized for FindMany and FindManyP
  template <typename T, typename D,
            template <typename, typename> class FM>
  typename std::enable_if<std::is_same<FM<T, void>, art::FindMany<T, void> >::value || std::is_same<FM<T, void>, art::FindManyP<T, void> >::value>::type
  check_get(FM<T, D> const & fA,
            FM<T, void> const & fAV) {
    typename FM<T, void>::value_type item;
    typename FM<T, D>::dataColl_t::value_type data;
    BOOST_CHECK_EQUAL((fAV.get(0ul, item)), 1ul);
    BOOST_CHECK_EQUAL((fAV.get(1ul, item)), 2ul);
    BOOST_CHECK_EQUAL((fAV.get(2ul, item)), 1ul);
    BOOST_CHECK_EQUAL((fA.get(0ul, item, data)), 1ul);
    BOOST_CHECK_EQUAL((fA.get(1ul, item, data)), 2ul);
    BOOST_CHECK_EQUAL((fA.get(2ul, item, data)), 1ul);
  }
}

critictest::TestFindOne::
TestFindOne(std::string const& inputLabel,
                       bool testAB,
                       bool testBA,
                       bool bCollMissing) :
  inputLabel_(inputLabel),
  testAB_(testAB),
  testBA_(testBA),
  bCollMissing_(bCollMissing)
{
}

template <template <typename, typename> class FO>
void
critictest::TestFindOne::
testOne(gallery::Event const & e) const
{
  gallery::Handle<AssnsAB_t> hAB;
  gallery::Handle<AssnsBA_t> hBA;
  gallery::Handle<AssnsABV_t> hABV;
  gallery::Handle<AssnsBAV_t> hBAV;
  if (testAB_) {
    BOOST_REQUIRE(e.getByLabel(inputLabel_, hAB));
    BOOST_REQUIRE_EQUAL(hAB->size(), 3ul);
    BOOST_REQUIRE(e.getByLabel(inputLabel_, hABV));
    BOOST_REQUIRE_EQUAL(hABV->size(), 3ul);
  }
  if (testBA_) {
    BOOST_REQUIRE(e.getByLabel(inputLabel_, hBA));
    BOOST_REQUIRE_EQUAL(hBA->size(), 3ul);
    BOOST_REQUIRE(e.getByLabel(inputLabel_, hBAV));
    BOOST_REQUIRE_EQUAL(hBAV->size(), 3ul);
  }
  // Construct a FO using a handle to a collection.
  gallery::Handle<std::vector<int> > hAcoll;
  BOOST_REQUIRE(e.getByLabel(inputLabel_, hAcoll));
  auto vhAcoll = e.getValidHandle<std::vector<int> >(inputLabel_);
  // First, check we can make an FO on a non-existent label without
  // barfing immediately.
  FO<critictest::StringProduct, critictest::LiteAssnTestData> foDead(hAcoll, e, "noModule");
  BOOST_REQUIRE(!foDead.isValid());
  BOOST_REQUIRE_EXCEPTION(foDead.size(),                                \
                          art::Exception,                               \
                          [](art::Exception const & e)                  \
                          {                                             \
                            return e.categoryCode() == art::errors::LogicError && \
                              e.history().back() == "ProductNotFound";  \
                          });                                           \
  BOOST_REQUIRE_EXCEPTION(foDead.data(0),                               \
                          art::Exception,                               \
                          [](art::Exception const & e)                  \
                          {                                             \
                            return e.categoryCode() == art::errors::LogicError && \
                              e.history().back() == "ProductNotFound";  \
                          });

  // Now do our normal checks.
  gallery::Handle<std::vector<critictest::StringProduct> > hBcoll;
  std::unique_ptr<FO<int, critictest::LiteAssnTestData> > foA;
  std::unique_ptr<FO<int, void> > foAV;
  if (! bCollMissing_) {
    BOOST_REQUIRE(e.getByLabel(inputLabel_, hBcoll));
    if (testBA_) {
      foA.reset(new FO<int, critictest::LiteAssnTestData>(hBcoll, e, inputLabel_));
      foAV.reset(new FO<int, void>(hBcoll, e, inputLabel_));
    }
  }
  FO<critictest::StringProduct, critictest::LiteAssnTestData> foB(hAcoll, e, inputLabel_);
  FO<critictest::StringProduct, critictest::LiteAssnTestData> foB2(vhAcoll, e, inputLabel_);
  FO<critictest::StringProduct, void> foBV(hAcoll, e, inputLabel_);
  std::vector<art::Ptr<int> > vp;
  vp.reserve(3);
  for (size_t i = 0; i < 3; ++i) {
    vp.emplace_back(art::ProductID(), &hAcoll.product()->at(i), i);
    if (testAB_) {
      BOOST_CHECK_EQUAL(*(*hAB)[i].first, i);
      if (! bCollMissing_) {
        BOOST_CHECK_EQUAL(*(*hAB)[i].second, B_t(std::string(X[i])));
      }
      BOOST_CHECK_EQUAL((*hAB).data(i).d1, (*hAB)[i].first.key());
      BOOST_CHECK_EQUAL((*hAB).data(i).d2, (*hAB)[i].second.key());
      BOOST_CHECK_EQUAL((*hAB).data(i).label, std::string(A[i]));
      BOOST_CHECK_EQUAL(*(*hABV)[i].first, i);
      if (! bCollMissing_) {
        BOOST_CHECK_EQUAL(*(*hABV)[i].second, B_t(std::string(X[i])));
      }
    }
    if (testBA_) {
      if (! bCollMissing_) {
        BOOST_CHECK_EQUAL(*(*hBA)[i].first, B_t(std::string(X[i])));
      }
      BOOST_CHECK_EQUAL(*(*hBA)[i].second, i);
      BOOST_CHECK_EQUAL((*hBA).data(i).d2, (*hBA)[i].first.key());
      BOOST_CHECK_EQUAL((*hBA).data(i).d1, (*hBA)[i].second.key());
      BOOST_CHECK_EQUAL((*hBA).data(i).label, std::string(A[i]));
      if (! bCollMissing_) {
        BOOST_CHECK_EQUAL(*(*hBAV)[i].first, B_t(std::string(X[i])));
      }
      BOOST_CHECK_EQUAL(*(*hBAV)[i].second, i);
    }
    if (bCollMissing_) {
      BOOST_CHECK(!foBV.at(i));
    }
    else {
      BOOST_CHECK_EQUAL(dereference(foBV.at(i)), B_t(std::string(X[AI[i]])));
      if (testBA_) {
        BOOST_CHECK_EQUAL(dereference(foA->at(i)), BI[i]);
        BOOST_CHECK_EQUAL(dereference(foA->data(i)).d1, AI[i]);
        BOOST_CHECK_EQUAL(dereference(foA->data(i)).d2, i);
        BOOST_CHECK_EQUAL(dereference(foAV->at(i)), BI[i]);
        BOOST_CHECK_NO_THROW(check_get(*foA, *foAV));
      }
    }
    for (auto const & f : { foB, foB2 } ) {
      if (!bCollMissing_) {
        BOOST_CHECK_EQUAL(dereference(f.at(i)), B_t(std::string(X[AI[i]])));
      }
      BOOST_CHECK_EQUAL(dereference(f.data(i)).d1, i);
      BOOST_CHECK_EQUAL(dereference(f.data(i)).d2, BI[i]);
    }
  }

  // Check alternative accessors and range checking for Assns.
  if (testAB_) {
    BOOST_CHECK_THROW((*hAB).at(3), std::out_of_range);
    BOOST_CHECK_EQUAL(&(*hAB).data(0), &(*hAB).data((*hAB).begin()));
    BOOST_CHECK_THROW((*hAB).data(3), std::out_of_range);
  }
  if (testBA_) {
    BOOST_CHECK_THROW((*hBA).at(3), std::out_of_range);
    BOOST_CHECK_EQUAL(&(*hBA).data(0), &(*hBA).data((*hBA).begin()));
    BOOST_CHECK_THROW((*hBA).data(3), std::out_of_range);
  }

  // Check FindOne looking into a map_vector.
  BOOST_REQUIRE(hAcoll.isValid());
  art::InputTag tag(inputLabel_, "mapvec");
  FO<B_t, critictest::LiteAssnTestData> foBmv(hAcoll, e, tag);
  if (! bCollMissing_) {
    BOOST_CHECK_EQUAL(dereference(foBmv.at(0)), dereference(foB.at(0)));
    BOOST_CHECK_EQUAL(foBmv.data(0), foB.data(0));
    BOOST_CHECK_EQUAL(dereference(foBmv.at(1)), dereference(foB.at(1)));
    BOOST_CHECK_EQUAL(foBmv.data(1), foB.data(1));
    BOOST_CHECK_EQUAL(dereference(foBmv.at(2)), dereference(foB.at(2)));
    BOOST_CHECK_EQUAL(foBmv.data(2), foB.data(2));
  }
}

template <template <typename, typename> class FM>
void
critictest::TestFindOne::
testMany(gallery::Event const & e) const
{
  gallery::Handle<std::vector<int> > hAcoll;
  BOOST_REQUIRE(e.getByLabel(inputLabel_, hAcoll));

  // First, check we can make an FM on a non-existent label without
  // barfing immediately.
  FM<B_t, critictest::LiteAssnTestData> fmDead(hAcoll, e, "noModule");
  BOOST_REQUIRE(!fmDead.isValid());
  BOOST_REQUIRE_EXCEPTION(fmDead.size(),                                \
                          art::Exception,                               \
                          [](art::Exception const & e)                  \
                          {                                             \
                            return e.categoryCode() == art::errors::LogicError && \
                              e.history().back() == "ProductNotFound";  \
                          });                                           \
  BOOST_REQUIRE_EXCEPTION(fmDead.data(0),                               \
                          art::Exception,                               \
                          [](art::Exception const & e)                  \
                          {                                             \
                            return e.categoryCode() == art::errors::LogicError && \
                              e.history().back() == "ProductNotFound";  \
                          });

  // Now do our normal checks.
  // Check FindMany.
  FM<B_t, critictest::LiteAssnTestData>
    fmB(hAcoll, e, art::InputTag(inputLabel_, "many"));
  art::Ptr<int> larry(art::ProductID(), &hAcoll.product()->at(0), 0), curly(art::ProductID(), &hAcoll.product()->at(1), 1), mo(art::ProductID(),&hAcoll.product()->at(2), 2);
  FM<B_t, critictest::LiteAssnTestData>
    fmB2({larry, curly, mo}, e, art::InputTag(inputLabel_, "many"));
  for (auto const & f : { fmB, fmB2 }) {
    BOOST_REQUIRE_EQUAL(f.size(), 3ul);
    if (bCollMissing_) {
      BOOST_CHECK_EQUAL(f.at(0).size(), 0ul);
      BOOST_CHECK_EQUAL(f.at(1).size(), 0ul);
      BOOST_CHECK_EQUAL(f.at(2).size(), 0ul);
    } else {
      BOOST_CHECK_EQUAL(f.at(0).size(), 1ul);
      BOOST_CHECK_EQUAL(f.at(1).size(), 2ul);
      BOOST_CHECK_EQUAL(f.at(2).size(), 1ul);
    }
    BOOST_CHECK_EQUAL(f.data(0).size(), 1ul);
    BOOST_CHECK_EQUAL(f.data(1).size(), 2ul);
    BOOST_CHECK_EQUAL(f.data(2).size(), 1ul);
  }
  FM<B_t, void> fmBV(hAcoll, e, art::InputTag(inputLabel_, "many"));
  if (bCollMissing_) {
    BOOST_CHECK_EQUAL(fmBV.at(0).size(), 0ul);
    BOOST_CHECK_EQUAL(fmBV.at(1).size(), 0ul);
    BOOST_CHECK_EQUAL(fmBV.at(2).size(), 0ul);
  } else {
    BOOST_CHECK_EQUAL(fmBV.at(0).size(), 1ul);
    BOOST_CHECK_EQUAL(fmBV.at(1).size(), 2ul);
    BOOST_CHECK_EQUAL(fmBV.at(2).size(), 1ul);
    BOOST_CHECK_NO_THROW(check_get(fmB, fmBV));
  }

  // Check FindMany on map_vector
  FM<B_t, void> fmvBV(hAcoll, e, art::InputTag(inputLabel_, "manymapvec"));
  if (!bCollMissing_) {
    BOOST_CHECK_EQUAL(fmvBV.at(0).size(), 1ul);
    BOOST_CHECK_EQUAL(fmvBV.at(1).size(), 2ul);
    BOOST_CHECK_EQUAL(fmvBV.at(2).size(), 1ul);
    BOOST_CHECK_NO_THROW(check_get(fmB, fmBV));
  }
}