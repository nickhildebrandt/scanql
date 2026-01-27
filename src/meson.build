srcs = files(
  'main.c',
  'tokenizer.c',
  'sqlstring.c',
  'sql-validate.c',
  'sql-validate-report.c',
  'sql-token.c',
  'sql-token-stack.c',
)

inc = include_directories('.')

scanql_exe = executable(
  'scanql',
  srcs,
  include_directories : inc,
  install : true,
)

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

# Todo: needs to be set from 'ok' to 'fail'
test(
  'cli-sql-invalid',
  find_program('bash'),
  args: [
    join_paths(meson.project_source_root(), 'scripts', 'arg-test.sh'),
    join_paths(meson.project_source_root(), 'sql', 'invalid.sql'),
    scanql_exe,
    'ok',
  ],
)
