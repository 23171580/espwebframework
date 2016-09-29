interrupts--deprecated.c: interrupts.c Makefile
	echo "#define __SPLIT__deprecated" >"$@.splittmp"
	echo "#include \"interrupts.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: interrupts--deprecated.o
libhal_a_OBJECTS += interrupts--deprecated.o
interrupts--excm_level.c: interrupts.c Makefile
	echo "#define __SPLIT__excm_level" >"$@.splittmp"
	echo "#include \"interrupts.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: interrupts--excm_level.o
libhal_a_OBJECTS += interrupts--excm_level.o
interrupts--get_int_vpri.c: interrupts.c Makefile
	echo "#define __SPLIT__get_int_vpri" >"$@.splittmp"
	echo "#include \"interrupts.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: interrupts--get_int_vpri.o
libhal_a_OBJECTS += interrupts--get_int_vpri.o
interrupts--get_vpri_locklevel.c: interrupts.c Makefile
	echo "#define __SPLIT__get_vpri_locklevel" >"$@.splittmp"
	echo "#include \"interrupts.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: interrupts--get_vpri_locklevel.o
libhal_a_OBJECTS += interrupts--get_vpri_locklevel.o
interrupts--intlevel.c: interrupts.c Makefile
	echo "#define __SPLIT__intlevel" >"$@.splittmp"
	echo "#include \"interrupts.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: interrupts--intlevel.o
libhal_a_OBJECTS += interrupts--intlevel.o
interrupts--intlevel_andbelow_mask.c: interrupts.c Makefile
	echo "#define __SPLIT__intlevel_andbelow_mask" >"$@.splittmp"
	echo "#include \"interrupts.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: interrupts--intlevel_andbelow_mask.o
libhal_a_OBJECTS += interrupts--intlevel_andbelow_mask.o
interrupts--intlevel_mask.c: interrupts.c Makefile
	echo "#define __SPLIT__intlevel_mask" >"$@.splittmp"
	echo "#include \"interrupts.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: interrupts--intlevel_mask.o
libhal_a_OBJECTS += interrupts--intlevel_mask.o
interrupts--intlevel_to_vpri.c: interrupts.c Makefile
	echo "#define __SPLIT__intlevel_to_vpri" >"$@.splittmp"
	echo "#include \"interrupts.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: interrupts--intlevel_to_vpri.o
libhal_a_OBJECTS += interrupts--intlevel_to_vpri.o
interrupts--inttype.c: interrupts.c Makefile
	echo "#define __SPLIT__inttype" >"$@.splittmp"
	echo "#include \"interrupts.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: interrupts--inttype.o
libhal_a_OBJECTS += interrupts--inttype.o
interrupts--inttype_mask.c: interrupts.c Makefile
	echo "#define __SPLIT__inttype_mask" >"$@.splittmp"
	echo "#include \"interrupts.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: interrupts--inttype_mask.o
libhal_a_OBJECTS += interrupts--inttype_mask.o
interrupts--num_interrupts.c: interrupts.c Makefile
	echo "#define __SPLIT__num_interrupts" >"$@.splittmp"
	echo "#include \"interrupts.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: interrupts--num_interrupts.o
libhal_a_OBJECTS += interrupts--num_interrupts.o
interrupts--set_int_vpri.c: interrupts.c Makefile
	echo "#define __SPLIT__set_int_vpri" >"$@.splittmp"
	echo "#include \"interrupts.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: interrupts--set_int_vpri.o
libhal_a_OBJECTS += interrupts--set_int_vpri.o
interrupts--set_vpri_locklevel.c: interrupts.c Makefile
	echo "#define __SPLIT__set_vpri_locklevel" >"$@.splittmp"
	echo "#include \"interrupts.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: interrupts--set_vpri_locklevel.o
libhal_a_OBJECTS += interrupts--set_vpri_locklevel.o
interrupts--timer_interrupt.c: interrupts.c Makefile
	echo "#define __SPLIT__timer_interrupt" >"$@.splittmp"
	echo "#include \"interrupts.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: interrupts--timer_interrupt.o
libhal_a_OBJECTS += interrupts--timer_interrupt.o
interrupts--trampolines.c: interrupts.c Makefile
	echo "#define __SPLIT__trampolines" >"$@.splittmp"
	echo "#include \"interrupts.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: interrupts--trampolines.o
libhal_a_OBJECTS += interrupts--trampolines.o
interrupts--vpri.c: interrupts.c Makefile
	echo "#define __SPLIT__vpri" >"$@.splittmp"
	echo "#include \"interrupts.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: interrupts--vpri.o
libhal_a_OBJECTS += interrupts--vpri.o
interrupts--vpri_int_disable.c: interrupts.c Makefile
	echo "#define __SPLIT__vpri_int_disable" >"$@.splittmp"
	echo "#include \"interrupts.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: interrupts--vpri_int_disable.o
libhal_a_OBJECTS += interrupts--vpri_int_disable.o
interrupts--vpri_int_enable.c: interrupts.c Makefile
	echo "#define __SPLIT__vpri_int_enable" >"$@.splittmp"
	echo "#include \"interrupts.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: interrupts--vpri_int_enable.o
libhal_a_OBJECTS += interrupts--vpri_int_enable.o
interrupts--vpri_to_intlevel.c: interrupts.c Makefile
	echo "#define __SPLIT__vpri_to_intlevel" >"$@.splittmp"
	echo "#include \"interrupts.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: interrupts--vpri_to_intlevel.o
libhal_a_OBJECTS += interrupts--vpri_to_intlevel.o
interrupts--num_intlevels.c: interrupts.c Makefile
	echo "#define __SPLIT__num_intlevels" >"$@.splittmp"
	echo "#include \"interrupts.c\"" >>"$@.splittmp"
	mv "$@.splittmp" "$@"
libhal.a: interrupts--num_intlevels.o
libhal_a_OBJECTS += interrupts--num_intlevels.o
