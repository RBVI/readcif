// vi: set expandtab shiftwidth=4:
//
// Output the number of rows and fields for all categories in a CIF file
//

#include "readcif.h"
#include <iostream>

readcif::CIFFile extract;

void
counts()
{
    size_t num_fields = extract.colnames().size();
    std::cout << extract.category() << ": " << num_fields << " field";
    if (num_fields > 1)
        std::cout << 's';
    readcif::CIFFile::ParseValues pv;
    unsigned long num_rows = 0;
    while (extract.parse_row(pv))
        ++num_rows;
    std::cout << ", " << num_rows << " row";
    if (num_rows > 1)
        std::cout << 's';
    std::cout << '\n';
}

int
main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << "error: missing mmCIF filename\n";
        goto usage;
    }
    extract.register_heuristic_stylized_detection();
    extract.set_unregistered_callback(counts);
    try {
        extract.parse_file(argv[1]);
        return 0;
    } catch (std::exception &e) {
        std::cerr << "error: " << e.what() << '\n';
        goto usage;
    }
usage:
    std::cerr << "usage: " << argv[0] << " mmCIF-filename\n";
    std::cerr << "    Show number of rows and columns for each category in file\n";
    return 1;
}
