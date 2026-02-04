# tuim

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![GitHub issues](https://img.shields.io/github/issues/xorrad/tuim)](https://github.com/xorrad/tuim/issues)
[![GitHub stars](https://img.shields.io/github/stars/xorrad/tuim.svg?style=flat&label=stars)](https://github.com/xorrad/tuim)

An header-only immediate mode text-based user interface for terminals.

> [!WARNING]  
> I scrapped everything to rewrite the entire project from scratch, so if you're looking for the previous version, which is of course unsupported, you can click [here](https://github.com/Xorrad/tuim/tree/legacy).

## Table of Contents

- [Installation](#installation)
- [Features](#features)
- [Examples](#examples)
- [Contributing](#contributing)
- [License](#license)

## Installation

```cmake
include(FetchContent)
FetchContent_Declare(
  tuim
  GIT_REPOSITORY https://github.com/Xorrad/tuim
  GIT_TAG master
)
set(TUIM_BUILD_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(tuim)

target_link_libraries(your_app PRIVATE tuim::tuim)
```

```cpp
#include <tuim.hpp>
```

## Features

## Examples

There are a few examples available in the [examples](https://github.com/Xorrad/tuim/tree/master/examples) directory. Do not hesitate to check them to learn more about using predefined generators and making new ones.

## Contributing

Contributions to the project are highly appreciated! There are several ways to get involved: you can contribute by reporting any issues you encounter, suggesting new features that could enhance the project, or even by actively participating in the development process through the submission of pull requests.

## License

This project is licensed under the MIT License - see the [LICENSE](https://raw.githubusercontent.com/Xorrad/tuim/master/LICENSE) file for details.