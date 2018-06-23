# racktool

A very early days, work in progress tool, for eventually doing even more useful things with VCV Rack plugins. For now only the low-level tool (rackproxy) of a bigger software stack has been made.

## Building

First build Rack per the instructions at https://github.com/VCVRack/Rack/blob/v0.6.1/README.md but instead of `git clone https://github.com/VCVRack/Rack.git` and `cd Rack` - do:

```
git clone -b v0.6.1 https://github.com/VCVRack/Rack.git Rack-0.6.1
cd Rack-0.6.1
```

Verify it's working with `make run`.

Then set the `RACK_DIR` variable pointing at the Rack code, the same way you would when building a plugin.

Go into `dep/rackproxy` and run `make`.

## Running

On Windows (only) you first need to - either copy `libwinpthread-1.dll`, `libstdc++-6.dll` and `libgcc_s_seh-1.dll`, from your `mingw64\bin\` directory of MSYS2 (I have it at `c:\msys2\mingw64\bin`) to the `dep/rackproxy` directory, or set your `PATH` to include the `mingw64\bin\` directory, e.g:
`set PATH=c:\msys2\mingw64\bin;%PATH%`

Run `./rackproxy.{exe|lin|mac}` to see help.

To see it in action, try running e.g:\
On a Mac:
```
./rackproxy.mac plugin $HOME/Documents/Rack/plugins/AS
```
or
```
./rackproxy.mac plugins $HOME/Documents/Rack/plugins
```

Or on Windows:
```
rackproxy.exe plugin %USERPROFILE%\Documents\Rack\plugins\Fundamental
```

If you can help fix the code 126 plugin loading error on Windows ([issue #2](https://github.com/rubyglow/racktool/issues/2)) you'll be my hero :-)

## Licenses

Copyright (c) 2018, Lars Bjerregaard.
Source code in this repository is licensed under [BSD-3-Clause](LICENSE).

The code in dep/rackproxy uses code from VCV Rack, Copyright (c) 2016 Andrew Belt.

The "VCV" name is trademarked by Andrew Belt.

Big thanks to Andrew for creating VCV Rack and making it Open Source!