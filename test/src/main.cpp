/**
 * @file main.cpp
 *
 * Test suites for arbitrary.
 *
 * @author theNerd247 (noah.harvey247 gm)
 * @copyright
 */

#include <algorithm>
#include <map>
#include <vector>
#include <array>
#include <deque>
#include <forward_list>
#include <list>

#include "arbitrary/arbitrary.hpp"

#include "gtest/gtest.h"

INIT_ARBITRARY()

namespace
{
/** A gtest fixture class for wrapping various types to test with
 *
 */
template<typename T>
class ArbitraryTypeFixture : public ::testing::Test
{
  public:
    ArbitraryTypeFixture() {};
    virtual ~ArbitraryTypeFixture() {};
};

template<typename T>
class ContainerFixture : public ::testing::Test
{
  public:
    ContainerFixture() {};
    virtual ~ContainerFixture() {};

    using MapType = std::map<T,T>;
    using VecType = std::vector<T>;
    using QueType = std::deque<T>;
    using LstType = std::list<T>; 
};

using ArbitraryTypeFixtureTypes = ::testing::Types<
                                  char
                                  ,unsigned char
                                  ,int
                                  ,unsigned int
                                  ,long
                                  ,unsigned long
                                  ,long long
                                  ,unsigned long long
                                  ,float
                                  ,double
                                  ,std::string
                                  ,std::wstring
                                  >;

TYPED_TEST_CASE(ArbitraryTypeFixture, ArbitraryTypeFixtureTypes);
TYPED_TEST_CASE(ContainerFixture, ArbitraryTypeFixtureTypes);

//tests to see if the conversion worked
TYPED_TEST(ArbitraryTypeFixture, CanGenerate)
{
  TypeParam data = arbitrary::arbitrary<TypeParam>();

  SUCCEED();
}

template<typename T>
inline void testSizedContainer()
{
  T data = arbitrary::sizedContainer<100,T>();

  EXPECT_EQ(100,data.size());
}

TYPED_TEST(ContainerFixture, SizedContainer)
{
  testSizedContainer<typename TestFixture::MapType>();
  testSizedContainer<typename TestFixture::VecType>();
  testSizedContainer<typename TestFixture::QueType>();
  testSizedContainer<typename TestFixture::LstType>();
}

}

int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc,argv);
  return RUN_ALL_TESTS();
  return 0;
}
