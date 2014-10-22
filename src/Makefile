include projdef.mk

all :
	@for module in ${MODULES}; do \
		make -C $${module}; \
	done

test :
	@for module in ${MODULES}; do \
		if [ -d $${module}/test ]; then \
			make -C $${module}/test; \
		fi \
	done

clean :
	@for module in ${MODULES}; do \
		make -C $${module} clean; \
		if [ -d $${module}/test ]; then \
			make -C $${module}/test clean; \
		fi \
	done
