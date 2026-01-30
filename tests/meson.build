inc = include_directories('../src')

test_sqlstring_exe = executable(
  'test_sqlstring',
  files('test_sqlstring.c', '../src/sql-string.c'),
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
    '../src/sql-string.c',
    '../src/sql-string-list.c',
  ),
  include_directories : inc,
)

test(
  'sqlstringlist',
  test_sqlstringlist_exe,
)

test_sqlvalidate_exe = executable(
  'test_sqlvalidate',
  files(
    'test_sqlvalidate.c',
    '../src/sql-validate.c',
  ),
  include_directories : inc,
)

test(
  'sqlvalidate',
  test_sqlvalidate_exe,
)

test_sqlvalidate_report_exe = executable(
  'test_sqlvalidate_report',
  files(
    'test_sqlvalidate_report.c',
    '../src/sql-validate.c',
    '../src/sql-validate-report.c',
  ),
  include_directories : inc,
)

test(
  'sqlvalidate_report',
  test_sqlvalidate_report_exe,
)

test_tokenizer_exe = executable(
  'test_tokenizer',
  files(
    'test_tokenizer.c',
    '../src/tokenizer.c',
    '../src/sql-token.c',
    '../src/sql-token-stack.c',
    '../src/sql-validate.c',
  ),
  include_directories : inc,
)

test(
  'tokenizer',
  test_tokenizer_exe,
)
