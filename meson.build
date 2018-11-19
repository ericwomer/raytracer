project(
    'raytracer',
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

cpp = meson.get_compiler('cpp')

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

tbb_prefix = get_option('with_tbb')
    
libtbb = cpp.find_library(
        'tbb',
        dirs:join_paths(tbb_prefix, 'lib')
)

tbb = declare_dependency(
    include_directories: include_directories(tbb_prefix + '/include'),
    dependencies: [libtbb],
    link_args: ['-latomic']
)

raytrace_deps = [tbb, threads]

raytrace = executable(
    'raytracer', 
    raytrace_src,
    dependencies: raytrace_deps,
    include_directories: raytrace_include_directories,
    cpp_args: raytrace_cflags,
    link_args: raytrace_ldflags
)
