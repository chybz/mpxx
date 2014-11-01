PROJECT_NAME=mpxx
PROJECT_VERSION=0.1
PACKAGE_NAME=mpxx
GCOV=
BASEDIR=$(shell pwd)
PRJ_BINDIR=/Users/chybz/dev/mpxx/build/bin
CB_JOBS=-j4

all: build-project

build-project:
	@$(MAKE) $(CB_JOBS) -C build


binaries: 


libraries: 


plugins: 


tests: 

test: tests
	@cd build && env PRJ_BINDIR=$(PRJ_BINDIR) prove -r -m -e '' t

test-verbose: tests
	@cd build && env PRJ_BINDIR=$(PRJ_BINDIR) prove -v -r -m -e '' t

test-report: tests
	@mkdir -p build/quality-reports/tests
	@cd build && env PRJ_BINDIR=$(PRJ_BINDIR) prove \
	    -v -r -m -e '' \
	    -P HTML=outfile:quality-reports/tests/index.html,force_inline_css:1,force_inline_js:1 \
	    t || true

coverage-report:
	@rm -rf build/coverage
	@mkdir -p build/coverage
	@echo "building coverage data"

	@cd build && ( \
		gcovr \
			-e '/usr/include.*' \
			--html --html-details \
			-o coverage/index.html \
	    ; \
	    sed -i"" -r -e 's,>[^<]+/sources/,>,g' coverage/*.html \
	)

doc:
	@cd build/doc && doxygen 2> doxygen.log

package:
	@cd build && ( \
		rm -rf usr etc && \
		make DESTDIR=packaging install && \
		cd packaging && dpkg-buildpackage -b -uc \
	)

install:
	@sudo dpkg -i DEBS/*.deb

upload:
	@cd DEBS && erepo-upload

clean:
	@rm -rf build
	@rm -f DEBS/$(PACKAGE_NAME)*

distclean: clean
	@rm -rf .cbuild
