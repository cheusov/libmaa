tmp_file = ${.OBJDIR}/err.log
replace_cmd =	sed -e 's/Bad.*$$/Bad file descriptor/'

.PHONY : test_output
test_output:
	@LD_LIBRARY_PATH=${OBJDIR_maa}; export LD_LIBRARY_PATH; \
	${.OBJDIR}/errtest 1 2>&1; echo '$$?='$$?; \
	${.OBJDIR}/errtest 2 2>&1; echo '$$?='$$?; \
	${.OBJDIR}/errtest 3 2>&1; echo '$$?='$$?; \
	${.OBJDIR}/errtest 4 2>&1 | ${replace_cmd}; \
	${.OBJDIR}/errtest 5 2>&1; echo '$$?='$$?; \
	${.OBJDIR}/errtest 6 2>&1; echo '$$?='$$?; \
	ulimit -c 0; ${.OBJDIR}/errtest 7 >${.OBJDIR}/err.log 2>&1; ex=$$?; \
	  grep -vE '^(Aborted|Abort trap)' ${tmp_file}; echo '$$?='$$ex; \
	ulimit -c 0; ${.OBJDIR}/errtest 8 >${.OBJDIR}/err.log 2>&1; ex=$$?; \
	  grep -vE '^(Aborted|Abort trap)' ${tmp_file}; echo '$$?='$$ex; \
	echo ==================== log file ====================; \
	awk 'NF > 0 {$$1="ddd"; $$2="mmm"; $$3="DD"; $$4="hh:mm:ss"; \
		$$5="YYYY"; $$6="hostname"; \
		sub(/errtest[^ ]+/, "errtest[nnnn]:")} \
		{print $$0}' ${LOGFILE} | \
	  ${replace_cmd}; \
	\
	${MAKE} ${MAKEFLAGS} cleandir >/dev/null 2>&1; \
	true

CLEANFILES +=	${LOGFILE} ${tmp_file}
.include <mkc.minitest.mk>
