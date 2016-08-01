
#
# This is a simple top-level wrapper makefile for the rest of the project.
#

all: docs debug coverage

.PHONY: docs setup clean

setup:
	./bin/project.sh

docs:
	$(MAKE) -C ./build/debug -B veridoc-docs

debug:
	$(MAKE) -C ./build/debug veridoc parser

coverage:
	$(MAKE) -C ./build/coverage veridoc parser

clean:
	$(MAKE) -C ./build/coverage clean
	$(MAKE) -C ./build/debug    clean
	rm -rf ./build/docs


test-all: test-debug test-coverage
	
test-debug: debug
	$(MAKE) -C ./build/debug test
	
test-coverage: coverage 
	$(MAKE) -C ./build/coverage test

test-coverage-report: test-coverage
	cd ./build/coverage/src/CMakeFiles/veridoccore.dir/ ; \
	gcov -abcf *.o  ; \
    lcov --directory . -c -o cov.info -t "veridoc" ; \
	genhtml -o ../../../../cov-report -t "veridoc" --num-spaces 4 cov.info
	@echo "Coverage report available in:"
	@echo "./build/cov-report/index.html"
