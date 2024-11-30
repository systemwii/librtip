# librtip : Runtime IOS Patching

A library for Wii homebrew to patch and reload IOSes at runtime using HW_AHBPROT.

### Setup

This library is designed to be included in a Git repository and compiled alongside the rest of the program. It works out-of-the-box with the [systemwii Make setup](https://github.com/systemwii/make) but can also be adapted to other build setups.

1. Add it as a submodule:
```bash
git submodule add https://github.com/systemwii/librtip.git lib/rtip
```
2. Set the version you want:
```bash
cd lib/rtip && git checkout vs2.1 && cd ../..
```
3. Ensure your Makefile can find it with:
```makefile
    LIBDIRSBNDLE =	$(wildcard lib/*/_)
```
4. Use it in relevant source files with:
```c
#include "rtip.h"
```
5. Build it or clear built files with these commands, in either its own folder or those of its containing projects:
```bash
make
make clean
```

Available versions:
- `master`: the latest commit on this repository's `master` branch
- `vs2.1`: the latest version as produced by [@shoutplenty](https://github.com/pyorot) for [@systemwii](https://github.com/systemwii)
- `v1.5.4`: the latest release of the original as maintained by [@Nanolx](https://gitlab.com/Nanolx/homebrewfilter)
- you can use any other tag or commit by its ID

Update your copy of this repo the usual way:
```bash
cd lib/rtip && git pull && cd ../..
```

### Usage

See the "API" files.

### Building

This library is included by source and compiled alongside applications. To clone it independently, use the command:
```bash
git clone --recurse-submodules https://github.com/systemwii/librtip.git librtip
```
Update any of its submodules, e.g. "make" here, with:
```bash
cd lib/make && git pull && cd ../..
```
Or update them all with:
```bash
git submodule update --remote
```

### Credits

This library originated as a fork of [HomebrewFilter](https://gitlab.com/Nanolx/homebrewfilter), but this repository is incompatible with that one because it was passed thru [git filter-repo](https://github.com/newren/git-filter-repo) to extract the relevant part. Thanks to Nanolx for maintaining the original.

Nanolx also thanks:
- libogc devs
- Team Twiizers
- damysteryman
- anyone I forgot to mention here
