dep_scanner = dependency('wayland-scanner', native: true)
prog_scanner = find_program(dep_scanner.get_variable(pkgconfig: 'wayland_scanner'))

dep_wp = dependency('wayland-protocols')
wp_dir = dep_wp.get_variable(pkgconfig: 'pkgdatadir', internal: 'pkgdatadir')

generated_protocols = [
	[ 'xdg-shell', 'stable' ],
]

# yoinked from Weston
foreach proto: generated_protocols
	proto_name = proto[0]
	if proto[1] == 'internal'
		base_file = proto_name
		xml_path = proto_name + '.xml'
	elif proto[1] == 'stable'
		base_file = proto_name
		xml_path = wp_dir / 'stable' / base_file / (base_file + '.xml')
	elif proto[1] == 'unstable'
		base_file = '@0@-unstable-@1@'.format(proto_name, proto[2])
		xml_path = wp_dir / 'unstable' / proto_name / (base_file + '.xml')
	elif proto[1] == 'staging'
		base_file = '@0@-@1@'.format(proto_name, proto[2])
		xml_path = wp_dir / 'staging' / proto_name / (base_file + '.xml')
	endif

	foreach output_type: [ 'client-header', 'server-header', 'private-code' ]
		if output_type == 'client-header'
			output_file = '@0@-client-protocol.h'.format(base_file)
		elif output_type == 'server-header'
			output_file = '@0@-server-protocol.h'.format(base_file)
		else
			output_file = '@0@-protocol.c'.format(base_file)
			if dep_scanner.version().version_compare('< 1.14.91')
				output_type = 'code'
			endif
		endif

		var_name = output_file.underscorify()
		target = custom_target(
			'@0@ @1@'.format(base_file, output_type),
			command: [ prog_scanner, output_type, '@INPUT@', '@OUTPUT@' ],
			input: xml_path,
			output: output_file,
		)
		set_variable(var_name, target)
	endforeach
endforeach


vixen_sources = [
  xdg_shell_client_protocol_h,
  xdg_shell_protocol_c,
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
