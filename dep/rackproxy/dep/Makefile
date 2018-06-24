RACK_DIR ?= Rack

all: rack

rack:
	$(MAKE) dep -C $(RACK_DIR)
	$(MAKE) -C $(RACK_DIR)

.PHONY: all rack
.DEFAULT_GOAL := all
