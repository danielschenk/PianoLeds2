#!python

# MIT License
#
# Copyright (C) 2017 Daniel Schenk <danielschenk@users.noreply.github.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import os

Import('env')

# I did not find a way yet to make SCons' AddOption / GetOption work with PlatformIO.
# However, PlatformIO allows passing custom targets.
# Therefore I use this little hack to add the ability to run valgrind optionally.
def _build_nothing(target, source, env):
    pass
dummy = env.Command('foo', 'lib/readme.txt', _build_nothing)
Alias('memcheck', dummy)

prefix = ''
suffix = ''

if 'memcheck' in COMMAND_LINE_TARGETS:
    prefix += 'valgrind --leak-check=yes '

# Propagate the 'TERM' environment variable from the OS, so tools can decide if they should colorize output
env['ENV']['TERM'] = os.getenv('TERM', 'unknown')

# Build separate program for every test case.
# PIOBUILDFILES contains the SCons node for every built object file as determined by platformio.ini
output_dir = 'build/test/'
test_results = []
for node in env['PIOBUILDFILES']:
    test_name = os.path.basename(str(node[0])).rsplit('.')[0]
    test_program = env.Program(output_dir + test_name, node)

    test_result = env.Command(output_dir + test_name + 'Result.xml', test_program, '{}./$SOURCE --gtest_output=xml:$TARGET{}'.format(prefix, suffix))
    env.AlwaysBuild(test_result)
    Alias(test_name, test_result)
    test_results += test_result

# Make sure that when only 'memcheck' is specified, all tests are still run
if 'memcheck' in COMMAND_LINE_TARGETS and (len(COMMAND_LINE_TARGETS) == 1):
    Alias('memcheck', test_results)

Alias('all', test_results)
Default('all')
