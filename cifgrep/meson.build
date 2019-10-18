cifgrep = executable('cifgrep', 'cifgrep.cpp',
  cpp_args: ['-I../src'],
  link_with: lib,
  install: true)

if rst2man.found()
	rst = files('cifgrep.rst')
	man = custom_target('cifgrep.1',
		input: rst,
		output: ['cifgrep.1'],
		command: [rst2man, '@INPUT@', '@OUTPUT@'],
		install: true,
		install_dir: man1dir)
	#install_man(man)
endif
