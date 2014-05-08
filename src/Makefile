include projdef.mk

all :
	@for module in ${MODULES}; do \
		make -C $${module}; \
	done

test :
	@for module in ${MODULES}; do \
		make -C $${module}/test; \
	done

clean :
	@for module in ${MODULES}; do \
		make -C $${module} clean; \
		make -C $${module}/test clean; \
	done
