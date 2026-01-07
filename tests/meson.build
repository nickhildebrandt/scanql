inc = include_directories('../src')

test_exe = executable(
  'test_sqlstring',
  files('test_sqlstring.c', '../src/sqlstring.c'),
  include_directories : inc,
)

test(
  'sqlstring',
  test_exe,
)
