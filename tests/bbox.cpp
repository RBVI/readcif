#include "readcif.h"
#include <iostream>

static const bool Required = true;

struct DoneProcessing: std::exception
{
};

struct ExtractCIF: public readcif::CIFFile {
	ExtractCIF();
	void parse_atom_site();
	double min[3], max[3];
};

ExtractCIF::ExtractCIF()
{
	// Personal preference, I like lambda functions better.
	// The lambda functions are needed because parse_atom_size
	// is a member function.
	register_category("atom_site", 
		[this] () {
			parse_atom_site();
		});
	register_heuristic_stylized_detection();
}

void
ExtractCIF::parse_atom_site()
{
	CIFFile::ParseValues pv;
	pv.reserve(10);
	double xyz[3];
	double min[3], max[3];
	bool first_row = true;
	pv.emplace_back(get_column("Cartn_x", Required),
		[&] (const char* start) {
			xyz[0] = readcif::str_to_float(start);
		});
	pv.emplace_back(get_column("Cartn_y", Required),
		[&] (const char* start) {
			xyz[1] = readcif::str_to_float(start);
		});
	pv.emplace_back(get_column("Cartn_z", Required),
		[&] (const char* start) {
			xyz[2] = readcif::str_to_float(start);
		});
	while (parse_row(pv)) {
		if (first_row) {
			for (int i = 0; i < 3; ++i)
				min[i] = max[i] = xyz[i];
			first_row = false;
			continue;
		}
		for (int i = 0; i < 3; ++i) {
			if (xyz[i] < min[i])
				min[0] = xyz[i];
			else if (xyz[i] > max[i])
				max[i] = xyz[i];
		}
	}
	std::cout << "min: " << min[0] << ' ' << min[1] << ' ' << min[2] << '\n';
	std::cout << "max: " << max[0] << ' ' << max[1] << ' ' << max[2] << '\n';
	throw DoneProcessing();		// no need to read rest of file
}

int
main(int argc, char **argv)
{
	if (argc != 2) {
		std::cerr << "error: missing mmCIF filename\n\n";
		std::cerr << "usage: " << argv[0] << " mmCIF-filename\n";
		std::cerr << "    Compute bounding box of atoms\n";
		return 1;
	}

	ExtractCIF extract;

	try {
		extract.parse_file(argv[1]);
	} catch (DoneProcessing& e) {
		/*pass*/;
	} catch (std::exception& e) {
		std::cerr << argv[1] << ": " << e.what() << '\n';
		return 1;
	}
	return 0;
}
