# racktool

A very early days, work in progress tool, for eventually doing even more useful things with VCV Rack plugins. For now only the low-level tool (rackproxy) of a bigger software stack has been made.

## Installing

You don't need to build racktool yourself to play with it. Binary packages are available on the [releases page](https://github.com/rubyglow/racktool/releases). Just download the right .zip file for your platform (mac, win, lin) and extract it to any location.

## Setting up your development environment

The requirements for your development environment are identical to those of Rack. Please follow the instructions for [setting up your development environment](https://github.com/VCVRack/Rack/blob/v0.6.2b/README.md#setting-up-your-development-environment), in the Rack README.

## Building

Building is fairly straight forward, and takes about as long as building Rack, since a local copy of Rack is built in the process.

Make sure there are no spaces or unicode characters in the path you are building in, as this breaks many build systems.

Run the following to fetch the source and build:

```
git clone https://github.com/rubyglow/racktool.git
cd racktool
git submodule update --init --recursive
cd dep/rackproxy
make -j$(nproc) dep
make -j$(nproc)
```
On Mac use $(sysctl -n hw.ncpu) instead of $(nproc)

To run a quick test of rackproxy: `make test`

## Running

You can always run `./rackproxy help` to see usage information.

To load all your installed plugins: `./rackproxy plugins /path/to/Rack/plugins`

On Windows `rackproxy.exe` is by necessity called `rack.exe` instead ([issue #3](//github.com/rubyglow/racktool/issues/3)). I'll see if that can be fixed (help welcomed).

## Licenses

Copyright (c) 2018, Lars Bjerregaard.
Source code in this repository is licensed under [BSD-3-Clause](LICENSE).

The code in dep/rackproxy uses code from VCV Rack, Copyright (c) 2016 Andrew Belt.

The "VCV" name is trademarked by Andrew Belt.

Big thanks to Andrew for creating VCV Rack and making it Open Source!