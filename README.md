[![Build Status](https://travis-ci.org/sabbakumov/rst.svg?branch=master)](https://travis-ci.org/sabbakumov/rst)

# RST C++ library

This is the RST C++11 library.
It doesn't use exceptions and RTTI.

It is licensed under the _Simplified BSD License_.

Now the library contains the following components:

## Bind
  A set of std::function utilities like NullFunction and DoNothing.

## Check
  A set of macros for better programming error handling.

```cpp
void Foo(void* ptr) {
  // Aborts in debug build.
  RST_DCHECK(ptr != nullptr);
  // Aborts in debug and release builds.
  RST_CHECK(ptr != nullptr);
  // RST_DCHECK(false) actually.
  RST_NOTREACHED();
}
```

## Defer
  A Go-like (also a Microsoft GSL finally-like) method of deferring the action
  execution at the end of the function.

```cpp
void Foo() {}

void Bar() {
  // When block ends, calls Foo().
  RST_DEFER(Foo);
}
```
## Files
  File utils.

## Format
  A Python-like string format library

```cpp
const std::string res = "{} {}, {}{}{}"_format(1234, "Hello", "wor", 'l', 'd');
EXPECT_EQ(res, "1234 Hello, world");
```

## GUID
  GUID generation.

```cpp
// Returns GUID as described in RFC 4122.
const std::string guid = GenerateGUID();
```

## Hidden String
  A compile time string encryption. It's useful when you need to have a
  "secret" strings in a program that are not findable in a hex editor.

```cpp
RST_HIDDEN_STRING(kFirst, "abcDEF");
EXPECT_EQ(kFirst.Decrypt(), "abcDEF");
```

## Legacy
  A set of features unavaliable for C++11 compilers:
  * make_unique<T>
  * Optional<T>

## Logger
  A fast flexible thread-safe logger for C++11.

```cpp
auto sink = FileNameSink::Create("path.log");
sink.Ignore();  // Handle error here.

Logger logger(std::move(*sink));
Logger::SetLogger(&logger);

LOG_DEBUG("Log");
```

## Macros
  A Google-like RST_DISALLOW_COPY_AND_ASSIGN macros.

```cpp
class Foo {
 private:
  RST_DISALLOW_COPY(Foo);
  RST_DISALLOW_ASSIGN(Foo);
  RST_DISALLOW_COPY_AND_ASSIGN(Foo);
  RST_DISALLOW_IMPLICIT_CONSTRUCTORS(Foo);
};
```
  A Chromium-like build-flag support.

```cpp
#define RST_BUILDFLAG_FOO() (true)
#define RST_BUILDFLAG_BAR() (false)

#if RST_BUILDFLAG(FOO)
// These lines compile.
#endif  // RST_BUILDFLAG(FOO)

#if RST_BUILDFLAG(BAR)
// These lines are not included to the build.
#endif  // RST_BUILDFLAG(BAR)
```

## Memory
  A set of memory related utilities like WeakPtr, WrapUnique.

```cpp
std::unique_ptr<int> ptr;
ptr = WrapUnique(new int(0));

class Foo {
 public:
  WeakPtr<Foo> AsWeakPtr() const { return weak_factory_.GetWeakPtr(); }

 private:
  WeakPtrFactory weak_factory_{this};
};

WeakPtr<Foo> ptr;
{
  Foo foo;
  ptr = foo.AsWeakPtr();
}

if (ptr != nullptr) {
  // Do something here.
}
```

## NoDestructor
  A Chromium-like static storage duration support.

```cpp
const std::string& GetString() {
  static NoDestructor<std::string> str("Hello!");
  return *str;
}
```

## NotNull
  Microsoft GSL-like NotNull class that restricts a pointer or smart pointer to
  only hold non-null values.

```cpp
void Foo(NotNul<int*>) {
}

int i = 0;
Foo(&i);  // OK.
Foo(nullptr);  // Compilation error.

int* ptr = nullptr;
Foo(ptr);  // Debug assert.
```

Nullable class that permits a pointer or smart pointer to be nullptr. Must be
checked for null in order to dereference.
```cpp
void Foo(Nullable<int*> ptr) {
  if (ptr != nullptr)
    *ptr = 0;  // OK.
}

void Bar(Nullable<int*> ptr) {
  *ptr = 0;  // Debug assert.
}
```

## Preferences
  Chromium-like preferences.

## Random
  Random device to be used in one place.

## RTTI
  LLVM-like custom RTTI support.

## STL
  A set of STL utilities.

```cpp
const std::vector<int> vec = {1, 2, 3, 4};
std::string str;
for (const auto i : Reversed(&vec))
  str += std::to_string(i);
EXPECT_EQ(str, "4321");

std::vector<int> vec = {-1, 400, 10, 0, 3, -5};
Sort(&vec);
EXPECT_TRUE(std::is_sorted(std::cbegin(vec), std::cend(vec)));
// etc.
```

## Status
  A Google-like _Status_ class for recoverable error handling with small
  improvements. It's impossible now to ignore an error.

## TaskRunner
  A set of task runner utilities like PollingTaskRunner and ThreadTaskRunner.

## Threading
  A set of thread related utilities like Barrier.

## Type
  A Chromium-like StrongAlias class.

## Value
  A Chromium-like _Value_ class for representing JSON values in a program.
