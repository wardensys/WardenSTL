from SCons.Environment import Environment
from SCons.Script import AddOption, GetOption, COMMAND_LINE_TARGETS
from SCons.Builder import Node
import webbrowser
import os


# Command line options and compiler flags

AddOption(
    "--compiler",
    dest="compiler",
    type="string",
    default=None,
    help="Compiler name to use: g++, clang++, cl, icpx (default is what SCons determines)"
)

AddOption(
    "--cppstd",
    dest="cppstd",
    type="string",
    default="11",
    help="C++ standard to use: 98, 03, 11, 14, 17, 20 (default is C++11)"
)

compiler = GetOption('compiler')
cppstd = GetOption('cppstd')

print(f"Using {compiler if compiler is not None else 'default'} compiler.")
print(f"Using C++{cppstd} standard.")

GNU_COMPILER_FLAGS = [
    f'-std=c++{cppstd}', 
    '-Wall', 
    '-Wextra', 
    '-Wpedantic', 
    '-Wconversion', 
    '-Wshadow', 
    '-Werror'
]

COMPILER_FLAGS = {
    'g++': GNU_COMPILER_FLAGS,
    'clang++': GNU_COMPILER_FLAGS,
    'icpx': GNU_COMPILER_FLAGS,
    "cl": [f"/std:c++{cppstd}", "/W4", "/WX", "/permissive-", "/Zc:__cplusplus"]
}

SYNTAX_CHECKS_FLAG = {
    'g++': '-fsyntax-only',
    'clang++': '-fsyntax-only',
    'icpx': '-fsyntax-only',
    'cl': '/Zs',
}

# Environment

env = Environment(
    CXXCOMSTR = 'Compiling [$SOURCE]',
    LINKCOMSTR = 'Linking [$TARGET]'
)

if compiler is not None: 
    env['CXX'] = compiler

env.Append(
    CXXFLAGS = COMPILER_FLAGS[env['CXX']],
    CPPDEFINES = [
        'DOCTEST_CONFIG_SUPER_FAST_ASSERTS',
        '__WSTL_NO_INITIALIZERLIST__',
        '__WSTL_STD_ITERATORTRAITS_SUPPORT__',
        '__WSTL_CHECK_PUSHPOP__',
        '__WSTL_EXCEPTIONS__',
        '__WSTL_EXCEPTION_LOCATION__',
        '__WSTL_CHECK_ITERATOR__',
        '__WSTL_CHECK_INDEXOPERATOR__'
    ],
    CPPPATH = [
        env.Dir('doctest/doctest').srcnode(),
        env.Dir('include').srcnode()
    ]
)

# Doxygen

def OpenDoxygen(target: list[Node], source: list[Node], env: Environment) -> None:
    '''
    Opens the Doxygen documentation in the default browser
    '''
    webbrowser.open('file://' + source[0].get_abspath())

doxygen = env.Command(
    'build/docs/html/index.html', 
    'Doxyfile', 
    env.Action('doxygen ${SOURCE}', cmdstr='Generating Doxygen documentation...')
)

env.Alias('docs', doxygen)
env.Clean(doxygen, 'build/docs')

env.AlwaysBuild(env.Alias(
    'open', 
    doxygen, 
    env.Action(OpenDoxygen, cmdstr="Opening documentation in default browser...")
))

# Unit tests

env.VariantDir('build/tests', 'tests', duplicate=0)
tests = env.Program('build/tests/test', env.Glob('build/tests/*.cpp'))

env.AlwaysBuild(env.Alias('test', tests, './${SOURCE}'))

# Syntax checks

if 'syntax-checks' in COMMAND_LINE_TARGETS:
    syntaxEnv = env.Clone(
        CXXCOMSTR = 'Syntax checking [$SOURCE]'
    )

    syntaxEnv.Append(CXXFLAGS = SYNTAX_CHECKS_FLAG[syntaxEnv['CXX']])

    syntaxChecks = syntaxEnv.Object(syntaxEnv.Glob('build/tests/syntax_checks/*.t.cpp'))

    env.Alias('syntax-checks', syntaxChecks)