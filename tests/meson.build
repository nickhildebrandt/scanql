inc = include_directories('../src')

test_sqlstring_exe = executable(
  'test_sqlstring',
  files('test_sqlstring.c', '../src/sqlstring.c'),
  include_directories : inc,
)

test(
  'sqlstring',
  test_sqlstring_exe,
)

test_sqlstringlist_exe = executable(
  'test_sqlstringlist',
  files(
    'test_sqlstringlist.c',
    '../src/sqlstring.c',
    '../src/sqlstringlist.c',
  ),
  include_directories : inc,
)

test(
  'sqlstringlist',
  test_sqlstringlist_exe,
)
