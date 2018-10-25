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
    'src/raytracerapp.cpp',
    'src/main.cpp',
    'src/system/system.cpp',
    'src/system/file.cpp',
    'src/rendering/raytracer.cpp',
    'src/objects/sphere.cpp',
    'src/rendering/hitable.cpp',
    'src/rendering/raytracer.cpp'
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
