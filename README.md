# CallTrace

CallTrace is a small function call hierarchy generation library
## Building
### Release build
```
 $ meson setup --wipe <builddir> # wipe the build artifacts (like object files)
 $ meson setup <builddir> --reconfigure --buildtype=release # reconfigure the build directory for release build
 $ meson compile -C <builddir> # compile the project
```
### Debug build
```
 $ meson setup --wipe <buildir> # wipe the build artifacts (like object files)
 $ meson setup <builddir> --reconfigure --buildtype=release # reconfigure the build directory for debug build
 $ meson compile -C <builddir> # compile the project
```

### Static Library
```
 $ meson setup --wipe <buildir> # wipe the build artifacts (like object files)
 # NOTE: --buildtype=release or --buildtype=debug options can be added here  
 $ meson setup -C <builddir> --reconfigure --default-library=static # reconfigure the build directory for static library
 $ meson compile -C <builddir> # compile the project
 $ meson install -C <builddir> # install the static library
```
### Shared Library
```
 $ meson setup --wipe <buildir> # whipe the build artifacts (like object files)
 # NOTE: --buildtype=release or --buildtype=debug options can be added here
 $ meson setup -C <builddir> --reconfigure --default-library=shared # reconfigure the build directory for shared library
 $ meson compile -C <builddir> # compile the project
 $ meson install -C <builddir> # install the shared library
```
### Artifact Installation Directories
- Headers: /include/<ProjectNameInSmallCase>
- Static Libraries: /lib/lib<ProjectNameInSmallCase>.a-
- Shared Libraries: /bin/lib<ProjectNameInSmallCase>.dll
- PkgConfig (.pc) for static library: $PKG_CONFIG_PATH/<ProjectNameInSmallCase>_static.pc
- PkgConfig (.pc) for shared library: $PKG_CONFIG_PATH/<ProjectNameInSmallCase>_shared.pc
