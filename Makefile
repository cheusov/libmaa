##################################################

tests     =	arg base basics bit debug hash list log memstr memobj \
		prime pr prm set sl string stack err
.for d in ${tests}
LIBDEPS   +=	maa:tests/${d}      # all tests depend on maa library
SUBPRJ    +=	tests/${d}:test     # target "test" is for tests/*
test-tests/${d}: all-tests/${d}     # new inter-target dependency
.endfor

SUBPRJ    +=	doc

NODEPS    +=	install-tests/*:*     # do not install tests
NODEPS    +=	all-tests/*:*         # do not build tests by default
NODEPS    +=	test-doc:* test-maa:* # nothing to test in doc/ and maa/

##################################################

MKC_REQD  =	0.29.1

##################################################
.include "Makefile.common"
.include <mkc.subprj.mk>
