.PHONY : test_output do_all do_test

do_test: post_all

test_output:
	@set -e; \
	LD_LIBRARY_PATH=${OBJDIR_maa}; \
	export LD_LIBRARY_PATH; \
	${TEST_PROG:U${.OBJDIR}/${PROG}} ${TEST_PROG_ARGS} |\
	awk ' \
	/^version: / { gsub(/[0-9]+/, "999") } \
	{ sub(/ at [^ ]+$$/, " at 0xF00DBEAF") } \
	{ print }'

CLEANFILES +=	check_mkc_err_msg

.include <mkc.minitest.mk>
