# Build dependencies of rackproxy

RACK_DIR = Rack

all: rack template

# Build a local copy of Rack which rackproxy depends on
rack:
	$(MAKE) dep -C $(RACK_DIR)
	$(MAKE) -C $(RACK_DIR)

# Build a local copy of the Template plugin used for running quick test of rackproxy
template:
	RACK_DIR=../$(RACK_DIR) $(MAKE) -C TemplatePlugin

.PHONY: all rack template
.DEFAULT_GOAL := all
