test_data = files('3rec.cif')
test_exe = executable('cifcounts', 'counts.cpp',
  cpp_args: ['-I../src'],
  link_with: lib)
test('cifcounts', test_exe, args: test_data)
