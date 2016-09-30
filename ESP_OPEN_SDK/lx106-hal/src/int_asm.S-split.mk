int_asm--get_interrupt.S: int_asm.S Makefile
	echo "#define __SPLIT__get_interrupt" >"$@.splittmp"
	echo "#include \"int_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: int_asm--get_interrupt.o
libhal_a_OBJECTS += int_asm--get_interrupt.o
int_asm--get_intpending_nw.S: int_asm.S Makefile
	echo "#define __SPLIT__get_intpending_nw" >"$@.splittmp"
	echo "#include \"int_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: int_asm--get_intpending_nw.o
libhal_a_OBJECTS += int_asm--get_intpending_nw.o
int_asm--get_vpri.S: int_asm.S Makefile
	echo "#define __SPLIT__get_vpri" >"$@.splittmp"
	echo "#include \"int_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: int_asm--get_vpri.o
libhal_a_OBJECTS += int_asm--get_vpri.o
int_asm--set_intclear.S: int_asm.S Makefile
	echo "#define __SPLIT__set_intclear" >"$@.splittmp"
	echo "#include \"int_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: int_asm--set_intclear.o
libhal_a_OBJECTS += int_asm--set_intclear.o
int_asm--set_intenable.S: int_asm.S Makefile
	echo "#define __SPLIT__set_intenable" >"$@.splittmp"
	echo "#include \"int_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: int_asm--set_intenable.o
libhal_a_OBJECTS += int_asm--set_intenable.o
int_asm--set_intset.S: int_asm.S Makefile
	echo "#define __SPLIT__set_intset" >"$@.splittmp"
	echo "#include \"int_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: int_asm--set_intset.o
libhal_a_OBJECTS += int_asm--set_intset.o
int_asm--set_vpri.S: int_asm.S Makefile
	echo "#define __SPLIT__set_vpri" >"$@.splittmp"
	echo "#include \"int_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: int_asm--set_vpri.o
libhal_a_OBJECTS += int_asm--set_vpri.o
int_asm--vpri_lock.S: int_asm.S Makefile
	echo "#define __SPLIT__vpri_lock" >"$@.splittmp"
	echo "#include \"int_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: int_asm--vpri_lock.o
libhal_a_OBJECTS += int_asm--vpri_lock.o
int_asm--vpri_unlock.S: int_asm.S Makefile
	echo "#define __SPLIT__vpri_unlock" >"$@.splittmp"
	echo "#include \"int_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: int_asm--vpri_unlock.o
libhal_a_OBJECTS += int_asm--vpri_unlock.o
int_asm--get_intenable.S: int_asm.S Makefile
	echo "#define __SPLIT__get_intenable" >"$@.splittmp"
	echo "#include \"int_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: int_asm--get_intenable.o
libhal_a_OBJECTS += int_asm--get_intenable.o
