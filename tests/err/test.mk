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
	echo ==================== log file ====================; \
	awk 'NF > 0 {$$1="ddd"; $$2="mmm"; $$3="DD"; $$4="hh:mm:ss"; \
		$$5="YYYY"; sub(/errtest[^ ]+/, "errtest[nnnn]:")} \
		{print $$0}' ${LOGFILE}; \
	\
	${MAKE} ${MAKEFLAGS} cleandir >/dev/null 2>&1; \
	true

CLEANFILES +=	${LOGFILE}

.include <mkc.minitest.mk>
