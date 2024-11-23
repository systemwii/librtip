# libruntimeiospatch

A library providing necessary functions for patching IOS at runtime using HW_AHBPROT.

Distributed under the GNU GPL v2 license.

### Including

Include the library in your git repository as follows:

1. add submodule with bash: `git submodule add https://gitlab.com/shoutplenty/libruntimeiospatch.git librip`
2. set the version you want: `cd librip && git checkout vs2.0 && cd ..`
3. edit makefile to add: `INCLUDES := librip/source`
4. edit makefile to add: `LIBS := ../librip/*.a` (you can set this to wherever you build this `.a` file)
5. edit relevant source files to add: `#include "runtimeiospatch.h"`

Available versions:
- `master`: the latest commit on this repository's `master` branch
- `vs2.0`: v2.0 as produced by [@shoutplenty](https://gitlab.com/shoutplenty)
- `v1.5.4`: the latest release of the original as maintained by [@Nanolx](https://gitlab.com/Nanolx)

Sync your copy with this repo the usual way: `cd librip && git pull && cd ..`.

### API

See the "API" files.

### Thanks
- libOGC devs
- Team Twiizers
- damysterman
- anyone I forgot to mention here
