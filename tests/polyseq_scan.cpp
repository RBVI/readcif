#include "readcif.h"
#include <iostream>
#include <unistd.h>	// for getopt

using std::string;
using std::vector;
using namespace readcif;

static const bool Required = true;

struct DoneProcessing: std::exception
{
};

struct ExtractCIF: public CIFFile {
	ExtractCIF();
	virtual void reset_parse();
	void parse_entry();
	void parse_entity_poly_seq();
	string entry;
};

ExtractCIF::ExtractCIF()
{
	// Personal preference, I like lambda functions better.
	// The lambda functions are needed because parse_XXXX
	// are member functions.
	register_category("entry", 
		[this] () {
			parse_entry();
		});
	register_category("entity_poly_seq", 
		[this] () {
			parse_entity_poly_seq();
		});
	register_heuristic_stylized_detection();
}

void
ExtractCIF::reset_parse()
{
	entry = "unknown";
}

void
ExtractCIF::parse_entry()
{
	CIFFile::ParseValues pv;
	pv.reserve(2);
	pv.emplace_back(get_column("id", Required),
		[&] (const char* start, const char* end) {
			entry = string(start, end - start);
		});
	parse_row(pv);
}

void
ExtractCIF::parse_entity_poly_seq()
{
	CIFFile::ParseValues pv;
	pv.reserve(10);
	string entity_id, mon_id;
	int num;
	bool hetero = false;
	bool first_hetero = true;
	pv.emplace_back(get_column("entity_id", Required),
			[&] (const char* start, const char* end) {
				entity_id = string(start, end - start);
			});
	pv.emplace_back(get_column("num", Required),
			[&] (const char* start, const char*) {
				num = readcif::str_to_int(start);
			});
	pv.emplace_back(get_column("mon_id", Required),
			[&] (const char* start, const char* end) {
				mon_id = string(start, end - start);
			});
	pv.emplace_back(get_column("hetero"),
			[&] (const char* start, const char*) {
				hetero = *start == 'y' || *start == 'Y';
			});
	while (parse_row(pv)) {
		if (hetero) {
			if (first_hetero) {
				std::cout << "Heterogeneous residues:\n";
				first_hetero = false;
			}
			std::cout << entry << ": " << entity_id << ' ' << num << ' ' << mon_id << '\n';
		}
	}
	throw DoneProcessing();		// no need to read rest of file
}

int
main(int argc, char **argv)
{
	bool debug = false;
	int opt;

	while ((opt = getopt(argc, argv, "dh?")) != -1) {
		switch (opt) {
			case 'd':
				debug = true;
				break;
			case 'h': case '?':
			default:
				goto usage;
		}
	}

	if (optind == argc) {
		std::cerr << "error: missing mmCIF filename(s)\n\n";
usage:
		std::cerr << "usage: " << argv[0] << " [-d] mmCIF-filename(s)\n";
		std::cerr << "    Look for heterogeneous residues in polymers\n";
		exit(EXIT_FAILURE);
	}

	ExtractCIF extract;

	for (; optind < argc; ++optind) {
		try {
			extract.parse_file(argv[optind]);
		} catch (DoneProcessing& e) {
			continue;
		} catch (std::exception& e) {
			std::cerr << argv[optind] << ": " << e.what() << '\n';
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}
