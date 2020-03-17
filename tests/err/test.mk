.PHONY : test_output
test_output:
	@LD_LIBRARY_PATH=${OBJDIR_maa}; export LD_LIBRARY_PATH; \
	${.OBJDIR}/errtest 1 2>&1; \
	echo '$$?='$$?; \
	true

.include <mkc.minitest.mk>
