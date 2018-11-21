project(
    'Ray Tracing App',
    'cpp',
    version : '0.0.1',
    default_options: 
    [
        'cpp_std=c++17',
        'b_lundef=false',
        'b_sanitize=none',
        'warning_level=3'
    ],
    meson_version: '>=0.48.0'
)

pkgconfig = import('pkgconfig')

build_number_sh = find_program('build_number.sh', 'tools/build_number.sh')
build_number_increment = run_command(build_number_sh)
build_number = run_command('cat', files('build_number.txt'),).stdout().strip()

message(build_number)

cpp = meson.get_compiler('cpp')

conf_data = configuration_data()
conf_data.set('name', meson.project_name())

conf_data.set('compiler_name', cpp.get_id())
conf_data.set('compiler_version', cpp.version())
conf_data.set('host_machine', host_machine.system())
conf_data.set('cpu_family', build_machine.cpu_family())

conf_data.set('version_number', meson.project_version() + '.' + build_number)

configure_file(input : 'src/config.h.in',
              output : 'config.h',
              configuration : conf_data)

raytrace_src = [
    'src/scene/scene.cpp',
    'src/rendering/raytracer.cpp',
    'src/system/system.cpp',
    'src/system/file.cpp',
    'src/objects/sphere.cpp',
    'src/objects/hitable.cpp',
    'src/main.cpp',
    'src/raytracerapp.cpp'
    ]

raytrace_include_directories = [
    include_directories('src/'),
    include_directories('thirdparty/include/'),
    include_directories('/usr/lib/llvm-8/include/c++/v1/')
    ]

raytrace_cflags = ['-stdlib=libc++']
raytrace_ldflags = [
    '-lm',
    '-lc++',
    '-lc++abi']

threads = dependency('threads')

raytrace_deps = [threads]

if not get_option('with_system_tbb')
    tbb_prefix = 'thirdparty/tbb/'

# tbb build command, replace clang with cpp's name and the stdver and 
# make compiler=clang stdlib=libc++ stdver=c++17 tbb_build_prefix=lib

    if get_option('buildtype') == 'debug'
        libtbb = cpp.find_library(
                'tbb_debug',
                dirs : join_paths(meson.source_root(), tbb_prefix, 'build/lib_debug')
            )
            tbb_link_path = '-L' + meson.source_root() + '/' + tbb_prefix + 'build/lib_debug'
    else
         libtbb = cpp.find_library(
                'tbb',
                dirs : join_paths(meson.source_root(), tbb_prefix, 'build/lib_release')
            )
            tbb_link_path = '-L' + meson.source_root() + '/' + tbb_prefix + 'build/lib_release'
    endif

raytrace_deps += declare_dependency(
    include_directories: include_directories(tbb_prefix + '/include'),
    dependencies: [libtbb],
    link_args: ['-latomic', tbb_link_path]
)

endif

raytrace = executable(
    'raytracer', 
    raytrace_src,
    dependencies: raytrace_deps,
    include_directories: raytrace_include_directories,
    cpp_args: raytrace_cflags,
    link_args: raytrace_ldflags
)
