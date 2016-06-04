/**
 * @file main.cpp 
 *
 * Test suites for arbitrary.
 *
 * @author theNerd247 (noah.harvey247 gm)
 * @copyright 
 */

#include "arbitrary/arbitrary.hpp"

#include "gtest/gtest.h"

INIT_ARBITRARY()

namespace
{
  TEST(Running, returnsTrue)
  {
    ASSERT_TRUE(true);
  }
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc,argv);
  return RUN_ALL_TESTS();
	return 0;
}
