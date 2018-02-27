##################################################

tests     =	arg base basics bit debug hash list log memstr memobj \
		prime pr prm set sl string stack
.for d in ${tests}
LIBDEPS   +=	maa:tests/${d}
SUBPRJ    +=	tests/${d}:test
test-tests/${d}: all-tests/${d}
.endfor

SUBPRJ    +=	doc

NODEPS     =	install-tests/*:*

##################################################

MKC_REQD  =	0.29.1

##################################################
.include <mkc.subprj.mk>
