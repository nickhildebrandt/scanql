project('scanql', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'c_std=c23', 'werror=true'],)

clang_format = find_program('clang-format', required: false)

format_script = join_paths(meson.project_source_root(), 'scripts', 'format-code.sh')

run_target(
  'format',
  command: [find_program('bash'), files(format_script)]
)

subdir('src')

subdir('tests')
