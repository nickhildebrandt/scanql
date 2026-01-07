project('scanql', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'c_std=c23', 'werror=true'],)

clang_format = find_program('clang-format', required: false)

run_target(
  'format',
  command: ['bash', files('scripts/format-code.sh')]
)

subdir('src')

subdir('tests')
