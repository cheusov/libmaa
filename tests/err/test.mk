.PHONY : test_output
test_output:
	@${.OBJDIR}/errtest 1 2>&1; \
	echo '$$?='$$?; \
	true

.include <mkc.minitest.mk>
