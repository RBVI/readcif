test_data = files('1enm.cif')
test1_exe = executable('cifcounts', 'counts.cpp',
  cpp_args: ['-I../src'],
  link_with: lib)
test2_exe = executable('polyseq_scan', 'polyseq_scan.cpp',
  cpp_args: ['-I../src'],
  link_with: lib)
test3_exe = executable('bbox', 'bbox.cpp',
  cpp_args: ['-I../src'],
  link_with: lib)
test('cifcounts', test1_exe, args: test_data)
test('polyseq_scan', test2_exe, args: test_data)
test('bbox', test3_exe, args: test_data)
