# Build rackproxy

# Usage:
# make -j dep	Build dependencies (first)
# make -j		Build rackproxy
# make test		Run a few commands with rackproxy to verify it
# make clean	Clean rackproxy build (not deps)

RACK_DIR = dep/Rack
VERSION = 0.6.0-alpha1
RACK_VERSION = 0.6.1
API_LEVEL = 0.6

# Detect architecture
MACHINE = $(shell $(CC) -dumpmachine)
ifneq (, $(findstring apple, $(MACHINE)))
	ARCH_MAC := 1
	ARCH := mac
else ifneq (, $(findstring mingw, $(MACHINE)))
	ARCH_WIN := 1
	ARCH := win
	ifneq ( ,$(findstring x86_64, $(MACHINE)))
		ARCH_WIN_64 := 1
		BITS := 64
	else ifneq (, $(findstring i686, $(MACHINE)))
		ARCH_WIN_32 := 1
		BITS := 32
	endif
else ifneq (, $(findstring linux, $(MACHINE)))
	ARCH_LIN := 1
	ARCH := lin
else
$(error Could not determine architecture of $(MACHINE))
endif

STRIP ?= strip

# Add .cpp and .c files to the build
SOURCES += $(wildcard src/*.cpp)

ifdef ARCH_MAC
	TARGET := rackproxy
	FLAGS += -DARCH_MAC
	CXXFLAGS += -stdlib=libc++
	MAC_SDK_FLAGS = -mmacosx-version-min=10.7
	FLAGS += $(MAC_SDK_FLAGS)
	LDFLAGS += $(MAC_SDK_FLAGS) \
		-stdlib=libc++ -lpthread -ldl \
		-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -framework CoreAudio -framework CoreMIDI \
		$(RACK_DIR)/dep/lib/libglfw3.a $(RACK_DIR)/dep/lib/libGLEW.a $(RACK_DIR)/dep/lib/libjansson.a \
		$(RACK_DIR)/dep/lib/libspeexdsp.a $(RACK_DIR)/dep/lib/libzip.a $(RACK_DIR)/dep/lib/libz.a \
		$(RACK_DIR)/dep/lib/librtaudio.a $(RACK_DIR)/dep/lib/librtmidi.a $(RACK_DIR)/dep/lib/libcrypto.a \
		$(RACK_DIR)/dep/lib/libssl.a $(RACK_DIR)/dep/lib/libcurl.a
endif

ifdef ARCH_WIN
	# Rack plugins on Windows have a hard-dependency on "rack.exe"
	# It would be nice to "fix" this somehow (issue #2)
	TARGET := rack.exe
	FLAGS += -DARCH_WIN -D_USE_MATH_DEFINES
	LDFLAGS += -static \
		-Wl,--export-all-symbols \
		-lglew32 -lglfw3 -ljansson -lspeexdsp -lzip -lz -lcurl -lssl -lcrypto -lrtaudio -lrtmidi \
		-lpthread -lopengl32 -lgdi32 -lws2_32 -lcomdlg32 -lole32 -ldsound -lwinmm -lksuser -lshlwapi
endif

ifdef ARCH_LIN
	TARGET := rackproxy
	FLAGS += -DARCH_LIN
	CFLAGS += $(shell pkg-config --cflags gtk+-2.0)
	LDFLAGS += -rdynamic \
		-Wl,-Bstatic -lglfw3 -lGLEW -ljansson -lspeexdsp -lzip -lz -lrtmidi -lrtaudio -lcurl -lssl -lcrypto \
		-Wl,-Bdynamic -lpthread -lGL -ldl -lX11 -lasound -ljack \
		$(shell pkg-config --libs gtk+-2.0)
endif

# Include local + Rack includes
FLAGS += -Iinclude -I$(RACK_DIR)/include -I$(RACK_DIR)/dep/include

# Program version and Rack API level
FLAGS += -DVERSION=$(VERSION) -DAPI_LEVEL=$(API_LEVEL) -DRACK_VERSION=$(RACK_VERSION)

# Generate dependency files alongside the object files
FLAGS += -MMD -MP
FLAGS += -g

# Optimization
FLAGS += -O3 -march=nocona -ffast-math -fno-finite-math-only
FLAGS += -Wall -Wextra -Wno-unused-parameter

# Link against dependencies built by Rack
LDFLAGS += -L$(RACK_DIR)/dep/lib

ifneq ($(ARCH), mac)
	CXXFLAGS += -Wsuggest-override
endif

CXXFLAGS += -std=c++11 $(FLAGS)

# Derive object files from sources and place them before user-defined objects
OBJECTS := $(patsubst %, build/%.o, $(SOURCES))
DEPENDENCIES := $(patsubst %, build/%.d, $(SOURCES))

# Link in all Rack object files, except for main.cpp.o
# TODO: Trim to list of actually needed objects (unfortunately probably most)
RACK_OBJECTS = $(filter-out $(RACK_DIR)/build/src/main.cpp.o, $(wildcard $(RACK_DIR)/build/src/*.o $(RACK_DIR)/build/src/*/*.o))
RACK_OBJECTS += $(wildcard $(RACK_DIR)/build/dep/nanovg/src/*.o $(RACK_DIR)/build/dep/osdialog/*.o)

# Convenience targets

all: $(TARGET)

dep:
	$(MAKE) -C dep

clean:
	rm -rfv build dist $(TARGET)

test: $(TARGET)
	echo
	./$< version
	echo
	./$< help
	echo
	./$< platform
	echo
	./$< tags
	echo
	./$< plugin core
	echo
	./$< plugin dep/TemplatePlugin

dist: all
	rm -rf dist
	mkdir -p dist
	mkdir -p dist/racktool
	cp ../../LICENSE* ../../README* dist/racktool/
	cp $(TARGET) dist/racktool/
ifdef ARCH_MAC
	$(STRIP) -S dist/racktool/$(TARGET)
endif
ifdef ARCH_WIN
	$(STRIP) -s dist/racktool/$(TARGET)
	cp /mingw64/bin/libwinpthread-1.dll dist/racktool/
	cp /mingw64/bin/libstdc++-6.dll dist/racktool/
	cp /mingw64/bin/libgcc_s_seh-1.dll dist/racktool/
endif
ifdef ARCH_LIN
	$(STRIP) -s dist/racktool/$(TARGET)
endif
	cd dist && zip -9 -r racktool-$(VERSION)-$(ARCH).zip racktool

# Final targets

$(TARGET): $(OBJECTS) $(RACK_OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

-include $(DEPENDENCIES)

build/%.cpp.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: all dep clean test dist
.DEFAULT_GOAL := all
