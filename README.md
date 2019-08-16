# README.md

# Building raytracer

 # Dependicies
 * cmake >= 3.15 or meson >=0.48.0 with ninja
 * clang/clang++ >= 8
 * libc++ >= 8
 * libc++abi >= 8

(as of Ubuntu 18.04 you will have to use kitware's apt repo for cmake >= 3.15)
https://apt.kitware.com/

```
sudo apt-get install clang-8 cmake libc++-8-dev libc++abi-8-dev
```


# Building documentation
Building the documentation depends on graphviz and doxygen
# On Ubuntu
```
sudo apt-get install doxygen graphviz
```

Build the project with cmake or meson