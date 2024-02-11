vixen_sources = [
  'vixen.c',
  'vixen-shm.c',
  'vixen-window.c',
]

vixen_dependencies = [
  dependency('wayland-client')
]

libvixen = library(
  'vixen', 
  vixen_sources,
  dependencies: vixen_dependencies,
  include_directories: incdir
)
