state_asm--cpregs_restore_fn.S: state_asm.S Makefile
	echo "#define __SPLIT__cpregs_restore_fn" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--cpregs_restore_fn.o
libhal_a_OBJECTS += state_asm--cpregs_restore_fn.o
state_asm--cpregs_save_fn.S: state_asm.S Makefile
	echo "#define __SPLIT__cpregs_save_fn" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--cpregs_save_fn.o
libhal_a_OBJECTS += state_asm--cpregs_save_fn.o
state_asm--get_cpenable.S: state_asm.S Makefile
	echo "#define __SPLIT__get_cpenable" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--get_cpenable.o
libhal_a_OBJECTS += state_asm--get_cpenable.o
state_asm--invalidate_cp.S: state_asm.S Makefile
	echo "#define __SPLIT__invalidate_cp" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--invalidate_cp.o
libhal_a_OBJECTS += state_asm--invalidate_cp.o
state_asm--restore_cp0.S: state_asm.S Makefile
	echo "#define __SPLIT__restore_cp0" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--restore_cp0.o
libhal_a_OBJECTS += state_asm--restore_cp0.o
state_asm--restore_cp1.S: state_asm.S Makefile
	echo "#define __SPLIT__restore_cp1" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--restore_cp1.o
libhal_a_OBJECTS += state_asm--restore_cp1.o
state_asm--restore_cp2.S: state_asm.S Makefile
	echo "#define __SPLIT__restore_cp2" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--restore_cp2.o
libhal_a_OBJECTS += state_asm--restore_cp2.o
state_asm--restore_cp3.S: state_asm.S Makefile
	echo "#define __SPLIT__restore_cp3" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--restore_cp3.o
libhal_a_OBJECTS += state_asm--restore_cp3.o
state_asm--restore_cp4.S: state_asm.S Makefile
	echo "#define __SPLIT__restore_cp4" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--restore_cp4.o
libhal_a_OBJECTS += state_asm--restore_cp4.o
state_asm--restore_cp5.S: state_asm.S Makefile
	echo "#define __SPLIT__restore_cp5" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--restore_cp5.o
libhal_a_OBJECTS += state_asm--restore_cp5.o
state_asm--restore_cp6.S: state_asm.S Makefile
	echo "#define __SPLIT__restore_cp6" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--restore_cp6.o
libhal_a_OBJECTS += state_asm--restore_cp6.o
state_asm--restore_cp7.S: state_asm.S Makefile
	echo "#define __SPLIT__restore_cp7" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--restore_cp7.o
libhal_a_OBJECTS += state_asm--restore_cp7.o
state_asm--restore_cpregs.S: state_asm.S Makefile
	echo "#define __SPLIT__restore_cpregs" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--restore_cpregs.o
libhal_a_OBJECTS += state_asm--restore_cpregs.o
state_asm--restore_extra.S: state_asm.S Makefile
	echo "#define __SPLIT__restore_extra" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--restore_extra.o
libhal_a_OBJECTS += state_asm--restore_extra.o
state_asm--save_cp0.S: state_asm.S Makefile
	echo "#define __SPLIT__save_cp0" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--save_cp0.o
libhal_a_OBJECTS += state_asm--save_cp0.o
state_asm--save_cp1.S: state_asm.S Makefile
	echo "#define __SPLIT__save_cp1" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--save_cp1.o
libhal_a_OBJECTS += state_asm--save_cp1.o
state_asm--save_cp2.S: state_asm.S Makefile
	echo "#define __SPLIT__save_cp2" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--save_cp2.o
libhal_a_OBJECTS += state_asm--save_cp2.o
state_asm--save_cp3.S: state_asm.S Makefile
	echo "#define __SPLIT__save_cp3" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--save_cp3.o
libhal_a_OBJECTS += state_asm--save_cp3.o
state_asm--save_cp4.S: state_asm.S Makefile
	echo "#define __SPLIT__save_cp4" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--save_cp4.o
libhal_a_OBJECTS += state_asm--save_cp4.o
state_asm--save_cp5.S: state_asm.S Makefile
	echo "#define __SPLIT__save_cp5" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--save_cp5.o
libhal_a_OBJECTS += state_asm--save_cp5.o
state_asm--save_cp6.S: state_asm.S Makefile
	echo "#define __SPLIT__save_cp6" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--save_cp6.o
libhal_a_OBJECTS += state_asm--save_cp6.o
state_asm--save_cp7.S: state_asm.S Makefile
	echo "#define __SPLIT__save_cp7" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--save_cp7.o
libhal_a_OBJECTS += state_asm--save_cp7.o
state_asm--save_cpregs.S: state_asm.S Makefile
	echo "#define __SPLIT__save_cpregs" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--save_cpregs.o
libhal_a_OBJECTS += state_asm--save_cpregs.o
state_asm--set_cpenable.S: state_asm.S Makefile
	echo "#define __SPLIT__set_cpenable" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--set_cpenable.o
libhal_a_OBJECTS += state_asm--set_cpenable.o
state_asm--validate_cp.S: state_asm.S Makefile
	echo "#define __SPLIT__validate_cp" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--validate_cp.o
libhal_a_OBJECTS += state_asm--validate_cp.o
state_asm--save_extra.S: state_asm.S Makefile
	echo "#define __SPLIT__save_extra" >"$@.splittmp"
	echo "#include \"state_asm.S\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state_asm--save_extra.o
libhal_a_OBJECTS += state_asm--save_extra.o
