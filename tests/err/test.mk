tmp_file = ${.OBJDIR}/err.log

.PHONY : test_output
test_output:
	@LD_LIBRARY_PATH=${OBJDIR_maa}; export LD_LIBRARY_PATH; \
	${.OBJDIR}/errtest 1 2>&1; echo '$$?='$$?; \
	${.OBJDIR}/errtest 2 2>&1; echo '$$?='$$?; \
	${.OBJDIR}/errtest 3 2>&1; echo '$$?='$$?; \
	${.OBJDIR}/errtest 4 2>&1 | sed 's/^main:.*$$/main: Bad file descriptor/'; \
	${.OBJDIR}/errtest 5 2>&1; echo '$$?='$$?; \
	${.OBJDIR}/errtest 6 2>&1; echo '$$?='$$?; \
	ulimit -c 0; ${.OBJDIR}/errtest 7 >${.OBJDIR}/err.log 2>&1; ex=$$?; \
	  grep -vE '^(Aborted|Abort trap)' ${tmp_file}; echo '$$?='$$ex; \
	ulimit -c 0; ${.OBJDIR}/errtest 8 >${.OBJDIR}/err.log 2>&1; ex=$$?; \
	  grep -vE '^(Aborted|Abort trap)' ${tmp_file}; echo '$$?='$$ex; \
	true

CLEANFILES +=	${tmp_file}
.include <mkc.minitest.mk>
