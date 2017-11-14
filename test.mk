.PHONY : test_output do_all do_test

do_test: post_all

test_output:
	@set -e; \
	LD_LIBRARY_PATH=${OBJDIR_maa}; \
	export LD_LIBRARY_PATH; \
	${.OBJDIR}/${TEST_PROG:U${PROG}}

CLEANFILES +=	check_mkc_err_msg

.include <mkc.minitest.mk>
