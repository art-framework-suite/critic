#include "SimpleServiceTest.h"

arttest::SimpleServiceTest::SimpleServiceTest(fhicl::ParameterSet const&) {}

bool
arttest::SimpleServiceTest::verifyStatus() const
{
  return true;
}
