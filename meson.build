project(
    'raytracer',
    'cpp',
    version : '0.0.1',
    default_options: 
    [
        'cpp_std=c++17',
        'b_lundef=false',
        'warning_level=3'
    ],
    meson_version: '>=0.48.0'
)

pkgconfig = import('pkgconfig')

cpp = meson.get_compiler('cpp')

raytrace_src = [
    'src/raytracer.cpp',
    'src/main.cpp'
    ]

raytrace_include_directories = [
    include_directories('src/'),
    include_directories('thirdparty/json/include/')
    ]

raytrace_cflags = ['-stdlib=libc++']
raytrace_ldflags = [
    '-lm',
    '-lc++',
    '-lc++abi']
raytrace_deps = []

raytrace = executable(
    'raytracer', 
    raytrace_src,
    dependencies: raytrace_deps,
    include_directories: raytrace_include_directories,
    cpp_args: raytrace_cflags,
    link_args: raytrace_ldflags
)
