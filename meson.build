project('readcif', 'cpp',
  version: '0.9',
  default_options: ['warning_level=3', 'cpp_std=c++11']
)

lib = static_library('readcif', 'readcif.cpp',
	install: true)

test_data = files('3rec.cif')

test_exe = executable('cifcounts', 'counts.cpp',
  link_with : lib)
test('cifcounts', test_exe, args: test_data)
