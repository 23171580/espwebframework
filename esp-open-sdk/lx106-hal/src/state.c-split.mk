state--all_extra_align.c: state.c Makefile
	echo "#define __SPLIT__all_extra_align" >"$@.splittmp"
	echo "#include \"state.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state--all_extra_align.o
libhal_a_OBJECTS += state--all_extra_align.o
state--all_extra_size.c: state.c Makefile
	echo "#define __SPLIT__all_extra_size" >"$@.splittmp"
	echo "#include \"state.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state--all_extra_size.o
libhal_a_OBJECTS += state--all_extra_size.o
state--cp_id_mappings.c: state.c Makefile
	echo "#define __SPLIT__cp_id_mappings" >"$@.splittmp"
	echo "#include \"state.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state--cp_id_mappings.o
libhal_a_OBJECTS += state--cp_id_mappings.o
state--cp_mask.c: state.c Makefile
	echo "#define __SPLIT__cp_mask" >"$@.splittmp"
	echo "#include \"state.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state--cp_mask.o
libhal_a_OBJECTS += state--cp_mask.o
state--cp_mask_mappings.c: state.c Makefile
	echo "#define __SPLIT__cp_mask_mappings" >"$@.splittmp"
	echo "#include \"state.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state--cp_mask_mappings.o
libhal_a_OBJECTS += state--cp_mask_mappings.o
state--cp_max.c: state.c Makefile
	echo "#define __SPLIT__cp_max" >"$@.splittmp"
	echo "#include \"state.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state--cp_max.o
libhal_a_OBJECTS += state--cp_max.o
state--cp_names.c: state.c Makefile
	echo "#define __SPLIT__cp_names" >"$@.splittmp"
	echo "#include \"state.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state--cp_names.o
libhal_a_OBJECTS += state--cp_names.o
state--cp_num.c: state.c Makefile
	echo "#define __SPLIT__cp_num" >"$@.splittmp"
	echo "#include \"state.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state--cp_num.o
libhal_a_OBJECTS += state--cp_num.o
state--cpregs_align.c: state.c Makefile
	echo "#define __SPLIT__cpregs_align" >"$@.splittmp"
	echo "#include \"state.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state--cpregs_align.o
libhal_a_OBJECTS += state--cpregs_align.o
state--cpregs_size.c: state.c Makefile
	echo "#define __SPLIT__cpregs_size" >"$@.splittmp"
	echo "#include \"state.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state--cpregs_size.o
libhal_a_OBJECTS += state--cpregs_size.o
state--extra_align.c: state.c Makefile
	echo "#define __SPLIT__extra_align" >"$@.splittmp"
	echo "#include \"state.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state--extra_align.o
libhal_a_OBJECTS += state--extra_align.o
state--init_mem_cp.c: state.c Makefile
	echo "#define __SPLIT__init_mem_cp" >"$@.splittmp"
	echo "#include \"state.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state--init_mem_cp.o
libhal_a_OBJECTS += state--init_mem_cp.o
state--init_mem_extra.c: state.c Makefile
	echo "#define __SPLIT__init_mem_extra" >"$@.splittmp"
	echo "#include \"state.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state--init_mem_extra.o
libhal_a_OBJECTS += state--init_mem_extra.o
state--num_coprocessors.c: state.c Makefile
	echo "#define __SPLIT__num_coprocessors" >"$@.splittmp"
	echo "#include \"state.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state--num_coprocessors.o
libhal_a_OBJECTS += state--num_coprocessors.o
state--extra_size.c: state.c Makefile
	echo "#define __SPLIT__extra_size" >"$@.splittmp"
	echo "#include \"state.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: state--extra_size.o
libhal_a_OBJECTS += state--extra_size.o
