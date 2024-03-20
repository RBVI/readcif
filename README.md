# readcif -- a fast C++ CIF and mmCIF parser

Version: 0.9

## Table of Contents

* [Overview](#overview)
* [License](#license)
* [API](docs/api.rst)
* [Comparison](docs/compare.rst)
* [Compiling](#compiling)
* [Contributions](#contributions)

## Overview

**readcif** is a [C++11](http://isocpp.org/wiki/faq/cpp11)
library for quickly extracting data
from [mmCIF](http://mmcif.wwpdb.org/)
and [CIF](http://www.iucr.org/resources/cif) files.
It fully conforms to the CIF 1.1 standard for data files,
and can be easily extended to handle CIF dictionaries.
In addition, it supports stylized PDBx/mmCIF files for even
quicker parsing.

## LICENSE

The **readcif** library [license](LICENSE.md) is an
[open source license](https://opensource.org/licenses)
with an acknowledgement clause.
So if you use it, acknowledge that fact in your documentation.

## Compiling

A simple [meson](https://mesonbuild.com/) setup is provided.
For simplest use, just copy the one C++ header file and source
file into your code.
```
% meson setup builddir
% cd builddir
% ninja install
```

## Contributors

Contributions should be made a pull requests on [github](https://github.com).
