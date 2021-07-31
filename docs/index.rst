..  vim: set expandtab shiftwidth=4:

#######################################
readcif -- a C++11 CIF and mmCIF parser
#######################################

**readcif** is a `C++11`_ library for quickly extracting data
from mmCIF_ and CIF_ files.
It fully conforms to the CIF 1.1 standard for data files,
and can be easily extended to handle CIF dictionaries.
In addition, it supports stylized PDBx/mmCIF files for even
quicker parsing.

.. _C++11: https://isocpp.org/wiki/faq/cpp11
.. _CIF: https://www.iucr.org/resources/cif
.. _mmCIF: https://mmcif.wwpdb.org/

*********
Contents:
*********

.. toctree::
    :hidden:

    license.rst

    api.rst

    compare.rst

* `Usage`_
* `PDBx/mmCIF Styling`_
* :doc:`license`
* :doc:`api`
* :doc:`compare`
* :ref:`genindex`

Usage
=====

CIF files are essentially a text version of a database.
Each table in the database corresponds to a category,
with named columns, and the rows contain the values.
CIF tags are a concatenation of the category name and the column name.

**readcif** provides a base class, :cpp:class:`CIFFile`,
that should be subclassed to implement an application's specific needs.
Virtual functions are used to support CIF reserved words,
that way the application can choose what to do if there is more than one
data block or handle dictionaries.
And callback functions are used to extract the data the
application wants from a category.
Finally, the category callback functions need to provide a set of
callback functions to parse the value of interesting columns.

So, in pseudo-code, an application's parser would look like::

    class ExtractCIF: public readcif::CIFFile {
    public:
        ExtractCIF() {
            initialize category callbacks
        }
        in each category callback:
            create parse value callback vector for interesting columns
            while (parse_row(parse_value_callbacks))
                continue;
    };

and be used by::

    ExtractCIF extract;

    const char* whole_file = ....
    extract.parse_file(filename)

See the associated :download:`example code <readcif_example.cpp>` file
for a working example
that reads a subset of the atom_site data from a PDB mmCIF file.

PDBx/mmCIF Styling
==================

`PDBx/mmCIF`_ files from the World-Wide PDB are formatted in a tabular style
that is both easier for humans to read and can be used by programs to
speed up parsing by up to 4 times.  Most of the speed up comes from exploiting
the styling for the largest tables, **atom_site** and **atom_site_anisotrop**.

Unfortunately, there is no standard annotation that tells if
a mmCIF file is in PDB's style.
There are two solutions: (1) explicitly denote the styling using the 
proposed **audit_syntax** category (see below).
And (2), use a heuristic, exploiting the fact that only mmCIF files from the PDB have the
**audit_conform** category, to detect if the mmCIF is stylized.
(If the **audit_conform.dict_name** is "mmcif_pdbx.dic_"
and **audit_conform.dict_version** is greater than 4.)
Both methods are used if :cpp:func:`CIFFile::register_heuristic_stylized_detection()` is called.
And that is shown in the :download:`example code <readcif_example.cpp>`.

**audit_syntax** category
-------------------------

Two data items in the **audit_syntax** category were proposed to the `ICUr`_ in August 2018:
**audit_syntax.case_senstive_flag** and **audit_syntax.fixed_width**.

The **case_sensitive_flag** data item is a boolean that when true means:

    1. CIF reserved words and tags only appear immediately
       after an ASCII newline.

    2. CIF reserved words are in lowercase.

    3. Tags are case sensitive (category names and item names
       are expected to match the case given in the associated dictionary,
       *e.g.*, mmcif_pdbx.dic_).

    Support for this can be explicitly controlled with the
    :cpp:func:`CIFFile::set_PDBx_keywords` function.

The **fixed_width** data item is a space separated list of data categories that are
formatted tabularly.  In those categories:

    1. All columns are left-aligned.

    2. Each row of data has all of the columns.

    3. All rows have trailing spaces so they are the same length.

    4. The end of the category’s data values is terminated by a comment line.

    Support for this is controlled with the
    :cpp:func:`CIFFile::set_PDBx_fixed_width_columns` function.

    The code can recover from a category falsely saying it has fixed width columns
    (e.g., because it has a multiline string in it) if the first row is split into
    multiple lines.


.. _mmcif_pdbx.dic: https://mmcif.wwpdb.org/dictionaries/mmcif_pdbx.dic/Index/
.. _PDBx/mmCIF: https://mmcif.wwpdb.org/docs/faqs/pdbx-mmcif-faq-general.html
.. _ICUr: https://www.iucr.org/resources/cif/dictionaries/new-item/cif_core


:Author: Greg Couch
:Organization: RBVI_, `University of California at San Francisco`_
:Contact: gregc@cgl.ucsf.edu
:Copyright: © Copyright 2014-2017 by the Regents of the University of California.  All Rights reserved.
:Last modified: 2021-7-29

.. _RBVI: https://www.rbvi.ucsf.edu/
.. _University of California at San Francisco: https://www.ucsf.edu/
