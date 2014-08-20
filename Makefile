# Variables
REPO ?= bulleterl
PRIVDIR ?= priv
RELDIR ?= $(REPO)
DEVRELDIR ?= dev


# The default target
all: deps compile


# Targets that don't correspond to the name of a file
.PHONY: all help compile script cert deps update-deps doc
.PHONY: clean-deps clean-cert clean-test clean distclean


.DEFAULT:
	$(error "Unrecognized target '$@'! Try 'make help'.")


# Help!
help:
	@echo "Targets:"
	@echo "    all            compile"
	@echo "    help           display this help message"
	@echo
	@echo "    deps           fetch all dependencies"
	@echo "    compile        compile the project"
	@echo "    test           run unit tests"
	@echo "    doc            generate code documentation"
	@echo
	@echo "    clean          clean up after 'compile' and 'test'"
	@echo "    clean-deps     clean up after 'deps'"
	@echo "    clean-test     clean up after 'test'"
	@echo "    distclean      clean up everything possible"


# Building
deps:
	./rebar get-deps

update-deps:
	./rebar update-deps

compile: deps
	./rebar compile

doc:
	./rebar doc skip_deps=true


# Testing
eunit test: clean-test compile
ifneq ($(suites),)
	./rebar eunit skip_deps=true suites=$(suites)
else
	./rebar eunit skip_deps=true
endif


# Cleanup
clean-deps: clean
	./rebar delete-deps
	-rm -rf deps

clean-compile:
	-rm -rf ebin priv/bulleterl.so

TEST_LOG_FILE := eunit.log
clean-test:
	-rm -f $(TEST_LOG_FILE)

clean: clean-test clean-compile
	./rebar clean

distclean: clean clean-deps clean-devcert
