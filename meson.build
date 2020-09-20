project('Builder', 'c')
gtkdep = dependency('gtk4')
some_args='-UHEADS'
if get_option('intern_heads')
	some_args='-DHEADS'
endif
executable('builder', 'main.c', c_args : some_args, dependencies : gtkdep)
