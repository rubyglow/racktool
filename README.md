# racktool

A very early days, work in progress tool, for eventually doing even more useful things with VCV Rack plugins. For now only the low-level tool (rackproxy) of a bigger software stack has been made.

## Building

First build the VCV Rack v0.6 branch per the instructions. Verify it's working with make run.

Then set the RACK_DIR variable pointing at the Rack code, the same way you would when building a plugin.

go into dep/rackproxy and run make.

## Running

Run ./rackproxy.{exe|lin|mac} to see help.

Try running e.g. ./rackproxy.mac plugin $HOME/Documents/Rack/plugins/AS (on a Mac) to see it in action, or e.g. ./rackproxy.mac plugins $HOME/Documents/Rack/plugins.

If you can help fix the code 126 plugin loading error on Windows you'll be my hero :-)

## Licenses

Copyright (c) 2018, Lars Bjerregaard.
Source code in this repository is licensed under [BSD-3-Clause](LICENSE).

The code in dep/rackproxy uses code from VCV Rack, Copyright (c) 2016 Andrew Belt.

The "VCV" name is trademarked by Andrew Belt.

Big thanks to Andrew for creating VCV Rack and making it Open Source!