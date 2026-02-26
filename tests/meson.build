# Unit tests compiled from the single translation unit with TEST_MODE defined.
# In TEST_MODE main.c compiles its own main() that runs all assertions.

test_exe = executable(
  'test_scanql',
  join_paths(meson.project_source_root(), 'src', 'main.c'),
  c_args: ['-DTEST_MODE'],
  include_directories: include_directories('../src'),
)

test('unit-tests', test_exe)
