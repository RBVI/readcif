project('readcif', 'cpp',
  version: '0.9',
  license: 'LICENSE.md',
  default_options: ['warning_level=3', 'cpp_std=c++11']
)

headers = ['readcif.h']
sources = ['readcif.cpp']

install_headers(headers)
lib = static_library('readcif', sources,
	install: true)
shared= shared_library('readcif', sources,
	install: true)

test_data = files('3rec.cif')

test_exe = executable('cifcounts', 'counts.cpp',
  link_with : lib)
test('cifcounts', test_exe, args: test_data)
