# Variables
REPO ?= bulleterl
PRIVDIR ?= priv
RELDIR ?= $(REPO)
DEVRELDIR ?= dev


# The default target
all: deps compile devrel


# Targets that don't correspond to the name of a file
.PHONY: all help compile script cert rel deps update-deps doc
.PHONY: clean-deps clean-cert clean-rel clean-test clean distclean


.DEFAULT:
	$(error "Unrecognized target '$@'! Try 'make help'.")


# Help!
help:
	@echo "Targets:"
	@echo "    all            comple and devrel"
	@echo "    help           display this help message"
	@echo
	@echo "    deps           fetch all dependencies"
	@echo "    compile        compile the project"
	@echo "    rel            generate a production release"
	@echo "    devrel         generate a development release, and a ./devrel runner script"
	@echo "    test           run unit tests"
	@echo "    doc            generate code documentation"
	@echo
	@echo "    clean          clean up after 'compile' and 'test'"
	@echo "    clean-deps     clean up after 'deps'"
	@echo "    clean-rel      clean up after 'rel'"
	@echo "    clean-devrel   clean up after 'devrel'"
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


# Building releases
rel/$(RELDIR): compile
	./rebar generate target_dir=$(@F) skip_deps=true

rel: rel/$(RELDIR)

rel/$(DEVRELDIR):
	./rebar generate target_dir=$(@F) skip_deps=true

devrel: rel/$(DEVRELDIR)
	@echo "#!/bin/bash" > devrel
	@echo 'exec $(abspath rel/$(DEVRELDIR)/bin/$(REPO)) "$$@"' >> devrel
	chmod +x devrel
	-rm -rf rel/$(DEVRELDIR)/lib/$(REPO)*
	ln -sf $(abspath .) rel/$(DEVRELDIR)/lib/$(REPO)-1
	$(foreach subapp,$(shell ls apps), \
		rm -rf rel/$(DEVRELDIR)/lib/$(subapp)*; \
		ln -sf $(abspath ./apps/$(subapp)) rel/$(DEVRELDIR)/lib/$(subapp)-1; \
	)


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

clean-rel:
	-rm -rf rel/$(RELDIR)

clean-devrel:
	-rm -rf devrel rel/$(DEVRELDIR)

TEST_LOG_FILE := eunit.log
clean-test:
	-rm -f $(TEST_LOG_FILE)

clean: clean-test
	./rebar clean

distclean: clean clean-deps clean-devcert clean-rel clean-devrel
