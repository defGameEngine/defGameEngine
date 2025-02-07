"""
    It's a build script that you can use for the Emscripten (works only on Windows)
"""

from helpers import *


EMSDK_PATH = 'C:/SDK/emsdk'
STB_PATH = 'C:/SDK/stb'
ASSETS_FOLDER = '../../Examples/Assets'
ENGINE_INCLUDE_PATH = '../../Include'
ENGINE_SOURCES_PATH = '../../Sources'

TARGETS = []


def construct_engine_sources() -> list[str]:
    return [file_name for file_name in get_files('../../Sources/')]


def main():
    builder = Builder()

    create_dir_if_not_exists('../Target')

    command_initialise = Command(f'{EMSDK_PATH}/emsdk')

    command_initialise.add_argument('activate')
    command_initialise.add_argument('latest')

    builder.add_command(command_initialise)

    for target in TARGETS:
        create_dir_if_not_exists(f'../Target/{target}')

        c = Command('em++')

        c.add_flag('O2')
        c.add_flag('Wall')
        c.add_flag('pedantic')
        c.add_flag('std', 'c++20')

        c.add_flag('sALLOW_MEMORY_GROWTH', '1')
        c.add_flag('sMIN_WEBGL_VERSION', '2')
        c.add_flag('sMAX_WEBGL_VERSION', '2')

        c.add_flag(f'I"{STB_PATH}"')
        c.add_flag(f'I"{ENGINE_INCLUDE_PATH}"')

        for source in construct_engine_sources():
            if source != 'PlatformGLFW3.cpp':
                c.add_argument(f'{ENGINE_SOURCES_PATH}/{source}')

        c.add_argument(f'../../Examples/{target}.cpp')

        c.add_flag('o')
        c.add_argument(f'../Target/{target}/index.js')

        if is_dir(ASSETS_FOLDER):
            c.add_argument('--preload-file')
            c.add_argument(ASSETS_FOLDER)

        builder.add_command(command_initialise.combine(c))

    builder.execute(True, True, True)
    copy('Emscripten/template.html', f'../Target/{target}/index.html')


if __name__ == '__main__':
    main()
