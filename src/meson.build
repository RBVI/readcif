soversion = '0'

rst2man = find_program('rst2man', required: false)
headers = ['readcif.h']
sources = ['readcif.cpp']

install_headers(headers)
lib = static_library('readcif', sources,
	install: true)
shared = shared_library('readcif', sources,
	soversion: soversion,
	# TODO: gnu_symbol_visibility: 'hidden',
	install: true)

pkg_mod = import('pkgconfig')
pkg_mod.generate(
	name: "libreadcif",
	filebase: "libreadcif"
	description: "a fast C++ CIF and mmCIF parser",
	libraries: [lib, shlib]
)
