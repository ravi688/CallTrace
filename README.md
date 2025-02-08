# CallTrace

CallTrace is a small function call hierarchy generation library
## Building and Installing
### Requirements
CallTrace uses build_master has its build system, you can download, build and install from [here](https://github.com/ravi688/BuildMaster). 
### Release build
```
 $ build_master meson setup --wipe <builddir> # wipe the build artifacts (like object files)
 $ build_master meson setup <builddir> --reconfigure --buildtype=release # reconfigure the build directory for release build
 $ build_master meson compile -C <builddir> # compile the project
```
### Debug build
```
 $ build_master meson setup --wipe <buildir> # wipe the build artifacts (like object files)
 $ build_master meson setup <builddir> --reconfigure --buildtype=release # reconfigure the build directory for debug build
 $ build_master meson compile -C <builddir> # compile the project
```

### Static Library
```
 $ build_master meson setup --wipe <buildir> # wipe the build artifacts (like object files)
 # NOTE: --buildtype=release or --buildtype=debug options can be added here  
 $ build_master meson setup -C <builddir> --reconfigure --default-library=static # reconfigure the build directory for static library
 $ build_master meson compile -C <builddir> # compile the project
 $ build_master meson install -C <builddir> # install the static library
```
### Shared Library
```
 $ build_master meson setup --wipe <buildir> # whipe the build artifacts (like object files)
 # NOTE: --buildtype=release or --buildtype=debug options can be added here
 $ build_master meson setup -C <builddir> --reconfigure --default-library=shared # reconfigure the build directory for shared library
 $ build_master meson compile -C <builddir> # compile the project
 $ build_master meson install -C <builddir> # install the shared library
```
### Artifact Installation Directories
- Headers: /include/<ProjectNameInSmallCase>
- Static Libraries: /lib/lib<ProjectNameInSmallCase>.a-
- Shared Libraries: /bin/lib<ProjectNameInSmallCase>.dll
- PkgConfig (.pc) for static library: $PKG_CONFIG_PATH/<ProjectNameInSmallCase>_static.pc
- PkgConfig (.pc) for shared library: $PKG_CONFIG_PATH/<ProjectNameInSmallCase>_shared.pc
