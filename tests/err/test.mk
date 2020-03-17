.PHONY : test_output
test_output:
	@LD_LIBRARY_PATH=${OBJDIR_maa}; export LD_LIBRARY_PATH; \
	${.OBJDIR}/errtest 1 2>&1; echo '$$?='$$?; \
	${.OBJDIR}/errtest 2 2>&1; echo '$$?='$$?; \
	${.OBJDIR}/errtest 3 2>&1; echo '$$?='$$?; \
	${.OBJDIR}/errtest 4 2>&1 | sed 's/^main:.*$$/main: Bad file descriptor/'; \
	${.OBJDIR}/errtest 5 2>&1; echo '$$?='$$?; \
	${.OBJDIR}/errtest 6 2>&1; echo '$$?='$$?; \
	ulimit -c 0; ${.OBJDIR}/errtest 7 2>&1; echo '$$?='$$?; \
	ulimit -c 0; ${.OBJDIR}/errtest 8 2>&1; echo '$$?='$$?; \
	true

.include <mkc.minitest.mk>
