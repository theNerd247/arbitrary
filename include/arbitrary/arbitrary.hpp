/**
 * @file Arbitrary.h
 *
 * API for generating arbitrary testing data.
 * This library is much inspired by Haskell's QuickCheck Arbitrary typeclass
 *
 * @author theNerd247 (noah.harvey247 gm)
 * @copyright
 */

#ifndef ARBITRARY_H
#define ARBITRARY_H

#include <memory>
#include <utility>
#include <vector>
#include <random>
#include <type_traits>
#include <string>

/** Call this macro after including this file in your main. This sets a static
 * variable in the Gen class.
 *
 */
#define INIT_ARBITRARY() std::random_device arbitraryRandDev;\
  arbitrary::Gen::GenType arbitrary::Gen::gen = arbitrary::Gen::GenType(arbitraryRandDev());

namespace arbitrary
{

/** Our general exception class
 *
 *
 *
 */
class ArbitraryException : public std::exception
{
  public:
    ArbitraryException()
      : message("Empty Exception") {};

    ArbitraryException(std::string msg)
      : message(msg)
    {};

    virtual ~ArbitraryException() {};

    virtual const char* what()
    {
      return this->message.c_str();
    }

  private:
    std::string message;
};

/** Default struct for the number generator.
 *
 * Users should ignore this.
 *
 */
struct Gen
{
  public:
    using GenType = std::default_random_engine;
    static GenType gen;
};

/** Data generation function for arbitrary values of the given type.
 *
 * Implement an overload of the function to make use of the functions in this
 * library. It's main purpose is to make it easier to generate testing data for
 * test suites.
 *
 */
template<typename A, class Enable = void>
struct Arbitrary
{
  static A generate();
};

/** A wrapper around Arbitrary::generate()
 *
 */
template<typename A>
inline A arbitrary()
{
  return Arbitrary<A>::generate();
};

/** Selects an element in the given inclusive range.
 *
 * A must be implemented for std::uniform_int_distribution() or
 * std::uniform_real_distribution() - if not then feel free to provide your own
 * overload of this function.
 *
 */
template<typename A, class Enable = void>
struct Choose
{
  static A choose(A const& start, A const& end);
};

template<typename A>
struct Choose<A,typename std::enable_if<std::is_integral<A>::value>::type> :
  public Gen
{
  static A choose(A const& start, A const& end)
  {
    return std::uniform_int_distribution<A>(start,end)(gen);
  }
};

template<typename A>
struct Choose<A,typename std::enable_if<std::is_floating_point<A>::value>::type> :
  public Gen
{
  static A choose(A const& start, A const& end)
  {
    return std::uniform_real_distribution<A>(start,end)(gen);
  }
};

template<typename A>
A choose(A const& start, A const& end)
{
  return Choose<A>::choose(start,end);
}

/** This allows us to generate a container of data of a given size.
 *
 * The following example generates an arbitrary map with 10 key-value pairs:
 *
 *    std::map<K,T> myMap = arbitrary::size<10,std::map<K,T>>();
 *
 * **NOTE:** Container must implement the Container concept (see official C++
 * documentation). Also, arbitrary<Container::value_type> must exist.
 */
template<size_t Size, typename Container>
Container sizedContainer()
{
  Container data;
  size_t i;

  for(i = 0; i < Size; i++)
  {
    auto d = arbitrary<typename Container::value_type>();
    data.insert(d);
  }

  return data;
}

/** Selects a random element from the given container.
 *
 */
template<typename Container>
typename Container::value_type& element(Container&& data)
{
  return *(data.begin() + choose<typename Container::size_type>(0,data.size()-1));
}

template<size_t Size,typename A>
std::vector<A> vectorOf()
{
  std::vector<A> data;

  for(size_t i = 0; i < Size; i++)
    data.push_back(arbitrary<A>());
}

/** Generates A such that the given predicate holds true
 *
 * @pred - the predicate with type-signature: `bool (A&)`
 *
 * This function will throw an exception after N retries and no suitable data is
 * generated.
 *
 */
template<typename A, typename Pred>
A suchThat(Pred&& pred, size_t N = 100)
{
  A data = arbitrary<A>();
  size_t n = N;

  while(!pred(data) && n--)
    data = arbitrary<A>();

  if(!n && !pred(data))
    throw ArbitraryException("Couldn't generate A in suchThat");

  return data;
}

template<typename A>
struct Arbitrary<A*>
{
  static A* generate()
  {
    auto data = new A();
    *data = arbitrary<A>();
    return data;
  }
};

template<typename A>
struct Arbitrary<std::shared_ptr<A>>
{
  static std::shared_ptr<A> generate()
  {
    return std::shared_ptr<A>(arbitrary<A>());
  }
};

template<typename A>
struct Arbitrary<A, typename std::enable_if<std::is_arithmetic<A>::value>::type>
{
  static A generate()
  {
    return choose<A>(std::numeric_limits<A>::min(),std::numeric_limits<A>::max());
  }
};

template<typename A, typename B>
struct Arbitrary<std::pair<A,B>>
{
  static std::pair<A,B> generate()
  {
    return std::make_pair(arbitrary<A>(),arbitrary<B>());
  }
};

/** Generates types for strings up to 256 chars
 *
 */
template<class CharT, class Traits, class Allocator>
struct Arbitrary<std::basic_string<CharT,Traits,Allocator>>
{
  using StringType = std::basic_string<CharT,Traits,Allocator>;

  static StringType generate()
  {
    using TraitsType = std::char_traits<CharT>;
    using SizeType = typename StringType::size_type;

    StringType str;

    SizeType strSize = choose<SizeType>(0,256);

    //32 - 126 is the visible character range on the ACSII table (includes
    //space character).
    for(SizeType i = 0; i < strSize; i++)
      str.push_back(TraitsType::to_char_type(choose<typename TraitsType::int_type>(32,
                                             126)));

    return str;
  }
};

} /* Arbitrary */


#endif
