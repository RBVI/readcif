..  vim: set expandtab shiftwidth=4:

readcif C++ API
===============

    All of the public symbols are in the **readcif** namespace.

.. cpp:type:: StringVector

    A std::vector of std::string's.

.. cpp:function:: int is_whitespace(char c)

    **is_whitespace** and **is_not_whitespace** are
    inline functions to determine if a character is CIF whitespace or not.
    They are similar to the C/C++ standard library's **isspace** function,
    but only recognize ASCII HT (9), LF (10), CR (13), and SPACE (32)
    as whitespace characters.  They are not inverses because
    ASCII NUL (0) is both not is_whitespace and not is_not_whitespace.

.. cpp:function:: int is_not_whitespace(char c)

    See :cpp:func:`is_whitespace`.

.. cpp:function:: double str_to_float(const char* s)

    Non-error checking inline function to convert a string to a
    floating point number.  It is similar to the C/C++ standard library's
    **atof** function, but returns NaN if no digits are found.
    Benchmarked by itself, it is slower than **atof**,
    but is empirically much faster when used in shared libraries.
    This is probably due to CPU cache behavior, but needs further investigation.

.. cpp:function:: int str_to_int(const char* s)

    Non-error inline function to convert a string to an integer.
    It is similar to the C/C++ standard library's **atoi** function.
    Same rational for use as :cpp:func:`str_to_float`.
    Returns zero if no digits are found.

.. cpp:class:: CIFFile

    The CIFFile is designed to be subclassed by an application to extract
    the data the application is interested in.

    Public section:

        .. cpp:type:: ParseCategory

            A typedef for **std::function<void (bool in_loop)>**.

        .. cpp:function:: void register_category(const std::string& category, \
            ParseCategory callback, \
            const StringVector& dependencies = StringVector())

            Register a callback function for a particular category.

            :param category: name of the category
            :param callback: function to retrieve data from category
            :param dependencies: a list of categories that must be parsed
                before this category.

            A null callback function removes the category.
            Dependencies must be registered first.
            A category callback function can find out which category
            it is processing with :cpp:func:`~CIFFile::category`.

        .. cpp:function:: void set_unregistered_callback(ParseCategory callback)

            Set callback function that will be called
            for unregistered categories.

        .. cpp:function:: void parse_file(const char* filename)

            :param filename: Name of file to be parsed

            If possible, memory-map the given file to get the buffer
            to hand off to :cpp:func:`parse`.  On POSIX systems,
            files whose size is a multiple of the system page size,
            have to be read into an allocated buffer instead.

        .. cpp:function:: void parse(const char* buffer)

            Parse the input and invoke registered callback functions

            :param buffer: Null-terminated text of the CIF file

            The text must be terminated with a null character.
            A common technique is to memory map a file
            and pass in the address of the first character.
            The whole file is required to simplify backtracking
            since data tables may appear in any order in a file.
            Stylized parsing is reset each time :cpp:func:`parse` is called.

        .. cpp:function:: int get_column(const char* name, bool required=false)
            
            :param tag: column name to search for
            :param required: true if tag is required

            Search the current categories tags to figure out which column
            the name corresponds to.
            If the name is not present,
            then -1 is returned unless it is required,
            then an error is thrown.

        .. cpp:type:: ParseValue1
         
            **typedef std::function<void (const char* start)> ParseValue1;**

        .. cpp:type:: ParseValue2
         
            **typedef std::function<void (const char* start, const char* end)> ParseValue2;**

        .. cpp:class:: ParseColumnn
        
            .. cpp:member:: int column_offset

                The column offset for a given tag,
                returned by :cpp:func:`get_column`.

            .. cpp:member:: bool need_end

                **true** if the end of the column needed -- not needed for numbers,
                since all columns are terminated by whitespace.

            .. cpp:member:: ParseValue1 func1

                The function to call if :cpp:member:`need_end` is **false**.

            .. cpp:member:: ParseValue2 func2

                The function to call if :cpp:member:`need_end` is **true**.

            .. cpp:function:: ParseColumn(int c, ParseValue1 f)

                Set :cpp:member:`column_offset` and :cpp:member:`func1`.

            .. cpp:function:: ParseColumn(int c, ParseValue2 f)

                Set :cpp:member:`column_offset` and :cpp:member:`func2`.

        .. cpp:type:: ParseValues

            **typedef std::vector<ParseColumn> ParseValues;**

        .. cpp:function:: bool parse_row(ParseValues& pv)

            Parse a single row of a table

            :param pv: The per-column callback functions
            :return: if a row was parsed

            The category callback functions should call :cpp:func:`parse_row`:
            to parse the values for columns it is interested in.  If in a loop,
            :cpp:func:`parse_row`: should be called until it returns false,
            or to skip the rest of the values, just return from the category
            callback.
            The first time :cpp:func:`parse_row` is called for a category,
            *pv* will be sorted in ascending order.
            Columns with negative offsets are skipped.

        .. cpp:function:: StringVector& parse_whole_category()

            Return complete contents of a category as a vector of strings.

            :return: vector of strings

        .. cpp:function:: void parse_whole_category(ParseValue2 func)

            Tokenize complete contents of category
            and call function for each item in it.

            :param func: callback function

        .. cpp:function:: const std::string& version()

            :return: the version of the CIF file if it is given

            For mmCIF files it is typically empty.

        .. cpp:function:: const std::string& category()

           :return: the category that is currently being parsed

           Only valid within a :cpp:type:`ParseCategory` callback.

        .. cpp:function:: const std::string& block_code()

           :return: the data block code that is currently being parsed

           Only valid within a :cpp:type:`ParseCategory` callback
           and :cpp:func:`finished_parse`.

        .. cpp:function:: const StringVector& colnames()
           :return: the set of column names for the current category

           Only valid within a :cpp:type:`ParseCategory` callback.

        .. cpp:function:: bool multiple_rows() const

            :return: if current category may have multiple rows 

        .. cpp:function:: size_t line_number() const

            :return: current line number

        .. cpp:function:: std::runtime_error error(const std::string& text)

            :param text: the error message
            :return: a exception with " on line #" appended
            :rtype: std::runtime_error

            Localize error message with the current line number
            within the input.
            # is the current line number.

    Stylized parsing support:

        .. cpp:function:: void register_heuristic_stylized_detection()

            Convenience function that registers
            both :cpp:func:`parse_audit_syntax` for the **audit_syntax** category
            and :cpp:func:`parse_audit_conform` for the **audit_conform** category,
            for detecting whether or not to use stylized parsing.

        .. cpp:function: void parse_audit_syntax()

            Parser for the **audit_syntax** category that turns on and
            off stylized parsing.  Not a heuristic.

        .. cpp:function: void parse_audit_confirm()

            Parser for the **audit_conform** category that
            implements a heuristic to turn on stylized parsing
            for the **atom_site** and **atom_site_anisotrop** categories.

        .. cpp:function:: void set_PDBx_keywords(bool stylized)

            Turn on and off PDBx/mmCIF keyword styling as described in
            `PDBx/mmCIF Styling`.

            :param stylized: if true, assume PDBx/mmCIF keyword style

            This is reset every time :cpp:func:`CIFFile::parse` 
            or :cpp:func:`CIFFile::parse_file` is called.
            It may be switched on and off at any time,
            *e.g.*, within a particular category callback function.

        .. cpp:function:: bool PDBx_keywords() const

            Return if the PDBx_keywords flag is set.
            See :cpp:func:`set_PDBx_keywords`.

        .. cpp:function:: void set_PDBx_fixed_width_columns(const std::string& category)

            Turn on PDBx/mmCIF fixed width column parsing for a given
            category as described in `PDBx/mmCIF Styling`.

            :param category: name of category

            This option must be set in each category callback that is needed.
            This option is ignored if :cpp:func:`PDBx_keywords` is false.
            This is not a global option because there is no reliable way
            to detect if the preconditions are met for each record without
            losing all of the speed advantages.

        .. cpp:function:: bool has_PDBx_fixed_width_columns() const

            Return if there were any fixed width column categories specified.
            See :cpp:func:`set_PDBx_fixed_width_columns`.

        .. cpp:function:: bool PDBx_fixed_width_columns() const

            Return if the current category has fixed width columns.
            See :cpp:func:`set_PDBx_fixed_width_columns`.

    Protected section:

        .. cpp:function:: void data_block(const std::string& name)

            :param name: name of data block

            **data_block** is a virtual function that is called whenever
            a new data block is found.
            Defaults to being ignored.
            Replace in subclass if needed.

        .. cpp:function:: void save_frame(const std::string& code)

            :param code: the same frame code

            **save_fame** is a virtual function that is called
            when a save frame header or terminator is found.
            It defaults to throwing an exception.
            It should be replaced if the application
            were to try to parse a CIF dictionary.

        .. cpp:function:: void global_block()

            **global_block** is a virtual function that is called whenever
            the **global\_** reserved word is found.
            It defaults to throwing an exception.
            In CIF files, **global\_** is unused.
            However, some CIF-like files, *e.g.*, the CCP4 monomer library,
            use the global\_ keyword.

        .. cpp:function:: void reset_parse()

            **reset_parse** is a virtual function that is called whenever
            the parse function is called.
            For example, PDB stylized parsing can be turned on here.

        .. cpp:function:: void finished_parse()

            **finished_parse** is a virtual function that is called whenever
            the parse function has successfully finished parsing.
