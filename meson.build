project('Builder', 'c')
deps = [dependency('gtk4'),dependency('json-glib-1.0')]
some_args='-UHEADS'
if get_option('intern_heads')
	some_args='-DHEADS'
endif
executable('builder', 'main.c', c_args : some_args, dependencies : deps, install : true)
