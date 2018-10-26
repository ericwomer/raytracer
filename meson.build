project(
    'raytracer',
    'cpp',
    version : '0.0.1',
    default_options: 
    [
        'cpp_std=c++11',
        'b_lundef=false',
        'b_sanitize=none',
        'warning_level=3'
    ],
    meson_version: '>=0.48.0'
)

pkgconfig = import('pkgconfig')

cpp = meson.get_compiler('cpp')

raytrace_src = [
    'src/system/file.cpp',
    'src/system/system.cpp',
    'src/rendering/raytracer.cpp',
    'src/objects/sphere.cpp',
    'src/rendering/hitable.cpp',
    'src/rendering/raytracer.cpp',
     'src/raytracerapp.cpp',
     'src/main.cpp',
    ]

raytrace_include_directories = [
    include_directories('src/'),
    include_directories('thirdparty/include/')
    ]

raytrace_cflags = ['-stdlib=libc++']
raytrace_ldflags = [
    '-lm',
    '-lc++',
    '-lc++abi']

assimp = dependency('assimp')

raytrace_deps = [assimp]

raytrace = executable(
    'raytracer', 
    raytrace_src,
    dependencies: raytrace_deps,
    include_directories: raytrace_include_directories,
    cpp_args: raytrace_cflags,
    link_args: raytrace_ldflags
)
