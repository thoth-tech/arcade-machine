# Contributing

Contributing to the Arcade Machine code

## Code Style/Naming Guidelines

### Naming

- User-defined types (classes, structs) should use pascal case, e.g. `MyClass`
- Functions and variables should use camel case, e.g. `myFuction()`, `myVariable`
- Constants and #defines should be all uppercase with underscores denoting spaces, e.g. `MY_CONSTANT`
- Private data should be prefixed with `m_` for "member", e.g. `m_myPrivateVariable`

### File names

- Each class should be declared in a header file named exactly after the class, e.g. `MyClass` is declared in MyClass.h
- Class declaration should stay in the .h file, definitions should be in a .cpp file, e.g. `MyClass` is defined in MyClass.cpp
- Only #include what is needed
- Use "" for including local files and <> for including library files

#### Namespaces
Don't use `using`, e.g. instead of:

```cpp
using std
using std::vector

vector<Button> buttons
```

do this instead

```cpp
#include <vector>

std::vector<Button> buttons
```

### clang-format

Investigate use of clang-format to automate style checks
