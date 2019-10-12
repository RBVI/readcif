# readcif

**readcif** is a C++11 library for quickly extracting data
from mmCIF and CIF files.
It fully conforms to the CIF 1.1 standard for data files,
and can be easily extended to handle CIF dictionaries.
In addition, it supports stylized PDBx/mmCIF files for even
quicker parsing.

## Table of Contents

* [API](docs/api.rst)
* [Comparison](docs/compare.rst)
* [Compiling](#compiling)
* [Contact](#conact)

## Compiling

A simple [meson](https://mesonbuild.com/) setup is provided.
For simpliest use, just copy the one header file and C++ source
file in your code.

## Contact

Created by [Greg Couch](mailto:gregc@cgl.ucsf.edu)

## Features needed for 1.0

* Add builtin support for **audit**\_**syntax** category 
for explicit control of sytlized PDBx/mmCIF parsing.

* Add optional ability to build shared library

* Update meson to optionally build cifgrep program

* Add documentation about **audit**\_**syntax**.

* Reorganize documentation
