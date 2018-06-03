# Build rackproxy

VERSION = 0.6.0-beta.1
RACK_VERSION = 0.6.1dev
API_LEVEL = 0.6

# Remember to: export RACK_DIR=/path/to/Rack (the full Rack sourcetree)
ifndef RACK_DIR
$(error RACK_DIR is not defined)
endif

STRIP ?= strip

# Add .cpp and .c files to the build
SOURCES += $(wildcard src/*.cpp)

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

# Detect architecture if ARCH is not defined
ifndef ARCH
MACHINE = $(shell $(CC) -dumpmachine)
ifneq (,$(findstring linux,$(MACHINE)))
	# Linux
	ARCH = lin
else ifneq (,$(findstring apple,$(MACHINE)))
	# Mac
	ARCH = mac
else ifneq (,$(findstring mingw,$(MACHINE)))
	# Windows
	ARCH = win
else
	$(error Could not determine machine type)
endif
endif

# Link against dependencies built by Rack
LDFLAGS += -L$(RACK_DIR)/dep/lib

ifeq ($(ARCH), lin)
	TARGET := rackproxy.lin
	FLAGS += -DARCH_LIN
	CFLAGS += $(shell pkg-config --cflags gtk+-2.0)
	LDFLAGS += -rdynamic \
		-Wl,-Bstatic -lglfw3 -lGLEW -ljansson -lspeexdsp -lzip -lz -lrtmidi -lrtaudio -lcurl -lssl -lcrypto \
		-Wl,-Bdynamic -lpthread -lGL -ldl -lX11 -lasound -ljack \
		$(shell pkg-config --libs gtk+-2.0)
endif

ifeq ($(ARCH), mac)
	TARGET := rackproxy.mac
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

ifeq ($(ARCH), win)
	TARGET := rackproxy.exe
	FLAGS += -DARCH_WIN -D_USE_MATH_DEFINES
	LDFLAGS += -static \
		-lglew32 -lglfw3 -ljansson -lspeexdsp -lzip -lz -lcurl -lssl -lcrypto -lrtaudio -lrtmidi \
		-lpthread -lopengl32 -lgdi32 -lws2_32 -lcomdlg32 -lole32 -ldsound -lwinmm -lksuser -lshlwapi
endif

# The install location for `make install`
DEP_LOCAL ?= .

# Commands
ifeq ($(ARCH), win)
	CMAKE := cmake -G 'MSYS Makefiles' -DCMAKE_INSTALL_PREFIX="$(realpath $(DEP_LOCAL))"
else
	CMAKE := cmake -DCMAKE_INSTALL_PREFIX="$(realpath $(DEP_LOCAL))"
endif

ifneq ($(ARCH), mac)
	CXXFLAGS += -Wsuggest-override
endif

CXXFLAGS += -std=c++11 $(FLAGS)

# Derive object files from sources and place them before user-defined objects
OBJECTS := $(patsubst %, build/$(ARCH)/%.o, $(SOURCES)) $(OBJECTS)
DEPENDENCIES := $(patsubst %, build/$(ARCH)/%.d, $(SOURCES))

# Link in all Rack object files, except for main.cpp.o
# TODO: Trim to list of actually needed objects (unfortunately probably most)
RACK_OBJECTS = $(filter-out $(RACK_DIR)/build/src/main.cpp.o, $(wildcard $(RACK_DIR)/build/src/*.o $(RACK_DIR)/build/src/*/*.o))
RACK_OBJECTS += $(wildcard $(RACK_DIR)/build/dep/nanovg/src/*.o $(RACK_DIR)/build/dep/osdialog/*.o)

# Final targets
$(TARGET): $(OBJECTS) $(RACK_OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

-include $(DEPENDENCIES)

build/$(ARCH)/%.cpp.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(TARGET)

clean:
	rm -rfv build/$(ARCH) $(TARGET) dist

.DEFAULT_GOAL := all
