project('readcif', 'cpp',
  version: '0.9',
  license: 'LICENSE.md',
  default_options: ['warning_level=3', 'cpp_std=c++11']
)

prefix = get_option('prefix')
mandir = join_paths(prefix, get_option('mandir'))
man1dir = join_paths(mandir, 'man1')

subdir('src')
subdir('cifgrep')
subdir('tests')
