project('mus', 'cpp')
incdir = include_directories('src')
src = ['src/main.cc', 'src/modules.cc']
executable('synth', src, include_directories : incdir)

gtest_dep = dependency('gtest')
module_test_src = ['test/module_test.cc', 'src/modules.cc']
module_test_exe = executable('synth_test_modules', module_test_src, dependencies : gtest_dep)
test('module test', module_test_exe)
