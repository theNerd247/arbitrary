arbitrary++
===========

This is a header library for providiing a quick and easy API for generating
testing data. It's heavily influenced from the ideas introduced by the Arbitrary
typeclass in Haskell.

Building 
--------

Note when cloning: this repo uses submodules for building.

```
git clone --recursive https://github.com/theNerd247/arbitrary++.git
mkdir build && cd build && cmake ..
make
```

Usage 
-----

To make your custom types to be generatable simply write a
template-specification for the arbitrary::Arbitrary struct. Here's an example: 

```c++
//we define a sample class
struct ExampleClass
{
  public:
    int x;
    std::string foo;
    ...
}

//make sure your definition is in the arbitrary namespace
namespace arbitrary
{
  template<>
  struct Arbitrary<ExampleClass>
  {
    //this is the function you need to overload.
    static ExampleClass generate()
    {
      //create the new data container.
      ExampleClass data;

      //we'll use the arbitrary::arbitrary<>() function to generate the member
      //variable data. This function is simply a wrapper around: 
      //arbitrary::Arbitrary<...>::generate()
      data.x = arbitrary<int>();
      data.foo = arbitrary<std::string>();
      ...

      return data;
    }
  }
}
```
And that's it! There's a couple of utility functions provided for making
generating test data easier. See the header file currently for these functions.
