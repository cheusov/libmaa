##################################################

PROJECTNAME =	libmaa

tests     =	arg base basics bit debug hash list log memstr memobj \
		prime pr prm set sl string stack err

.for d in ${tests}
LIBDEPS   +=	maa:tests/${d}      # all tests depend on maa library
SUBPRJ    +=	tests/${d}:test     # target "test" is for tests/*
test-tests/${d}: all-tests/${d}     # new inter-target dependency
.endfor

SUBPRJ    +=	doc

SUBPRJ_DFLT = maa test

NODEPS    +=	install-tests/*:*     # do not install tests
NODEPS    +=	all-tests/*:*         # do not build tests by default
NODEPS    +=	test-doc:* test-maa:* # nothing to test in doc/ and maa/

##################################################

MKC_REQD  =	0.34.0

##################################################
.include "help.mk"
.include "Makefile.common"
.include <mkc.subprj.mk>
