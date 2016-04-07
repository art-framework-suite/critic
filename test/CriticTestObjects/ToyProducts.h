#ifndef test_CriticTestObjects_ToyProducts_h
#define test_CriticTestObjects_ToyProducts_h

// ======================================================================
//
// EDProducts for testing purposes
//
// ======================================================================

#include <string>
#include <ostream>

namespace critictest
{

  struct IntProduct
  {
    explicit IntProduct(int i = 0) : value(i) { }
    ~IntProduct() { }

    int value;
  };

  struct StringProduct
  {
    StringProduct() : name_() {}
    explicit StringProduct(const std::string& s) : name_(s){}
    std::string name_;
    bool operator==(StringProduct const& r) const { return name_ == r.name_; }

  };

  inline
  std::ostream&
  operator<<(std::ostream& os, const StringProduct& p) {
    os << p.name_;
    return os;
  }

  struct Simple
  {
    Simple() : key(0), value(0.0) { }
    virtual ~Simple() { }
    typedef int key_type;

    key_type    key;
    double      value;

    key_type id() const { return key; }
    virtual double dummy() const { return -3.14; }
    virtual Simple* clone() const { return new Simple(*this); }
  };

  struct SimpleDerived : public Simple
  {
    SimpleDerived() : Simple(), dummy_(16.25) { }

    SimpleDerived( SimpleDerived const & other)
      : Simple(other), dummy_(other.dummy_)
    { }

    double dummy_;
    double dummy() const { return dummy_; }
    SimpleDerived* clone() const { return new SimpleDerived(*this); }
  };
}

#endif

// Local Variables:
// mode: c++
// End:
