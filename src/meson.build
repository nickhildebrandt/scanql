inc = include_directories('.')

# Single-translation-unit build: all code lives in main.c
scanql_exe = executable(
  'scanql',
  'main.c',
  include_directories : inc,
  install : true,
)

# CLI integration test: a valid SQL file must be accepted (exit 0)
test(
  'cli-sql-valid',
  find_program('bash'),
  args: [
    join_paths(meson.project_source_root(), 'scripts', 'arg-test.sh'),
    join_paths(meson.project_source_root(), 'sql', 'valid.sql'),
    scanql_exe,
    'ok',
  ],
)

# CLI integration test: an invalid SQL file must be rejected (exit 1)
test(
  'cli-sql-invalid',
  find_program('bash'),
  args: [
    join_paths(meson.project_source_root(), 'scripts', 'arg-test.sh'),
    join_paths(meson.project_source_root(), 'sql', 'invalid.sql'),
    scanql_exe,
    'fail',
  ],
)
