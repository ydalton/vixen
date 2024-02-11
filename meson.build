project('vixen', 'c',
  version: '1.0',
  default_options: [
    'warning_level=3',
    'optimization=2',
  ]
)

join_paths(meson.source_root(), 'src')
join_paths(meson.source_root(), 'examples')

compiler_flags = [
  '-Wno-unused-parameter',
  '-Werror=incompatible-pointer-types',
  '-Werror=implicit-function-declaration',
  '-Werror=return-type',
]

cc = meson.get_compiler('c')
add_project_arguments(
  cc.get_supported_arguments(compiler_flags),
  language: 'c'
)

incdir = include_directories('include')

subdir('src')
if get_option('examples')
  subdir('examples')
endif
