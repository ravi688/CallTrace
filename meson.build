# ------------------------------ DOCUMENTATION ---------------------------------
# Release build
# -------------------------
# $ meson setup --wipe <builddir> # wipe the build artifacts (like object files)
# $ meson setup <builddir> --reconfigure --buildtype=release # reconfigure the build directory for release build
# $ meson compile -C <builddir> # compile the project
#
# Debug build
# -------------------------
# $ meson setup --wipe <buildir> # wipe the build artifacts (like object files)
# $ meson setup <builddir> --reconfigure --buildtype=release # reconfigure the build directory for debug build
# $ meson compile -C <builddir> # compile the project
#
# Static Library
# -------------------------
# $ meson setup --wipe <buildir> # wipe the build artifacts (like object files)
# # NOTE: --buildtype=release or --buildtype=debug options can be added here  
# $ meson setup -C <builddir> --reconfigure --default-library=static # reconfigure the build directory for static library
# $ meson compile -C <builddir> # compile the project
# $ meson install -C <builddir> # install the static library
#
# Shared Library
# -------------------------
# $ meson setup --wipe <buildir> # whipe the build artifacts (like object files)
# # NOTE: --buildtype=release or --buildtype=debug options can be added here
# $ meson setup -C <builddir> --reconfigure --default-library=shared # reconfigure the build directory for shared library
# $ meson compile -C <builddir> # compile the project
# $ meson install -C <builddir> # install the shared library
#
# Artifact Installation Directories
# ---------------------------------
# Headers: /include/<ProjectNameInSmallCase>
# Static Libraries: /lib/lib<ProjectNameInSmallCase>.a-
# Shared Libraries: /bin/lib<ProjectNameInSmallCase>.dll
# PkgConfig (.pc) for static library: $PKG_CONFIG_PATH/<ProjectNameInSmallCase>_static.pc
# PkgConfig (.pc) for shared library: $PKG_CONFIG_PATH/<ProjectNameInSmallCase>_shared.pc
#
# -------------------------------- PROJECT CONFIGS -----------------------------

project('CallTrace', 'c', 'cpp',
  version : '1.0.0',
  default_options : [
    'warning_level=3',
    'buildtype=debug',
    'c_std=c17',
    'cpp_std=c++20'
  ]
)

# Release Build Defines
release_defines = [
  '-DCALLTRACE_RELEASE'
]

# Debug Build Defines
debug_defines = [
  '-DCALLTRACE_DEBUG'
]

# Shared Library Build Defines
shared_library_defines = [
  '-DCALLTRACE_BUILD_DYNAMIC_LIBRARY'
]

# Static Library Build Defines
static_library_defines = [
  '-DCALLTRACE_BUILD_STATIC_LIBRARY'
]

# Source files (excluding main.c)
lib_sources = files(
  'source/buffer.c',
  'source/calltrace.c'
)

# Include directories
inc = include_directories('include', 'include/calltrace')

# Main executable source
main_source = files('source/main.c')

# Whether to install executable?
is_install_executable = false

# ------------------------------ INTERNALS ---------------------------------------

# Compiler configuration
add_project_arguments('-m64', language : 'c')
add_project_arguments('-m64', language : 'cpp')
# Linker configuration
add_project_link_arguments('-m64', language : 'c')
add_project_link_arguments('-m64', language : 'cpp')

# Build type specific defines
build_mode_defines = []
if get_option('buildtype') == 'release'
  add_project_arguments(release_defines, language : 'c')
  add_project_arguments(release_defines, language : 'cpp')
  build_mode_defines += release_defines
else
  add_project_arguments(debug_defines, language : 'c')
  add_project_arguments(debug_defines, language : 'cpp')
  build_mode_defines += debug_defines
endif

# Static Library
calltrace_static_lib = static_library('calltrace_static',
  lib_sources,
  include_directories : inc,
  install : true,
  install_dir : get_option('libdir')/'calltrace',
  c_args : static_library_defines,
  cpp_args : static_library_defines,
  gnu_symbol_visibility: 'hidden'
)

# Shared Library
calltrace_shared_lib = shared_library('calltrace_shared',
  lib_sources,
  include_directories : inc,
  install : true,
  install_dir : get_option('libdir')/'calltrace',
  c_args : shared_library_defines,
  cpp_args : shared_library_defines,
  gnu_symbol_visibility: 'hidden'
)

# Main executable
calltrace_exe = executable('calltrace',
  main_source,
  link_with : calltrace_static_lib,
  include_directories : inc,
  install : is_install_executable
)

# Header installation
install_subdir('include/calltrace', install_dir : get_option('includedir'))

# pkg-config package installation
python = find_program('python')

result = run_command(python, '-c', 'import os; print(os.environ["PKG_CONFIG_PATH"])', check : false)
if result.returncode() == 0
  pkgconfig_install_path = result.stdout().split(';')[0]
else 
  pkgconfig_install_path = get_option('libdir')
endif

pkgmod = import('pkgconfig')
# Generate .pc file for static library 
pkgmod.generate(calltrace_static_lib,
    name: 'CallTrace',
    description: 'A function call trace library',
    filebase: 'calltrace_static',
    install_dir: pkgconfig_install_path
)
# Generate .pc file for shared library
pkgmod.generate(calltrace_shared_lib,
    name: 'CallTrace',
    description: 'A function call trace library',
    filebase: 'calltrace_shared',
    install_dir: pkgconfig_install_path
)

calltrace_dep = declare_dependency(
    link_with: calltrace_static_lib,
    include_directories: inc,
    compile_args: build_mode_defines
)
