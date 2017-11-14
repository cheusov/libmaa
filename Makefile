##################################################

tests     =	arg base bit debug hash list log mem prime pr set sl string
.for d in ${tests}
LIBDEPS   +=	maa:tests/${d}
SUBPRJ    +=	tests/${d}:test
test-tests/${d}: all-tests/${d}
.endfor

NODEPS     =	install-tests/*:*

##################################################

MKC_REQD  =	0.29.1

##################################################
.include <mkc.subprj.mk>
