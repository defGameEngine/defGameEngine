import subprocess
import os
import shutil


class Command:
    def __init__(self, tool: str):
        self.tool = tool
        self.arguments = []

    def add_argument(self, argument: str):
        self.arguments.append(argument)

    def add_flag(self, flag: str, value: str | None = None, omit_equals=False):
        arg = '-' + flag

        if value:
            if omit_equals:
                arg += value
            else:
                arg += '=' + value
        
        self.arguments.append(arg)

    def combine(self, cmd):
        new_cmd = Command(self.tool)

        new_cmd.arguments = self.arguments.copy()
        new_cmd.arguments.extend(['&', cmd.tool])
        new_cmd.arguments.extend(cmd.arguments.copy())

        return new_cmd


class Builder:
    def __init__(self):
        self.commands = []

    def add_command(self, command: Command):
        self.commands.append(command)

    def execute(self, show_stdout=False, show_stderr=False, show_prompt=False):
        for command in self.commands:
            prompt = f'{command.tool} {" ".join(command.arguments)}'

            if show_prompt:
                print(prompt)

            run = subprocess.run(prompt, capture_output=True, shell=True)

            if show_stdout and len(run.stdout) > 0:
                print(run.stdout.decode(errors='ignore'))

            if show_stderr and len(run.stderr) > 0:
                print(run.stderr.decode(errors='ignore'))


def is_dir(path: str) -> bool:
    return os.path.isdir(path)


def get_cwd() -> str:
    return os.getcwd()


def make_dir(path: str):
    os.mkdir(path)


def fix_path(path: str, is_local=True):
    return f'{get_cwd()}/{path}' if is_local else path


def create_dir_if_not_exists(path: str, is_local=True):
    full_path = fix_path(path, is_local)

    if not is_dir(full_path):
        make_dir(full_path)


def is_file(path: str, is_local=True):
    return os.path.isfile(fix_path(path, is_local))


def get_files(path, is_local=True) -> list[str] | None:
    full_path = fix_path(path, is_local)

    if not is_dir(full_path):
        return None
    
    files = []

    for entry in os.listdir(full_path):
        if is_file(f'{full_path}/{entry}', False):
            files.append(entry)

    return files


def get_path_base(path: str):
    return os.path.basename(path)


def _copy(source: str, dest: str, is_source_local=True, is_dest_local=True, func=None):
    func(fix_path(source, is_source_local), fix_path(dest, is_dest_local))


def copy_file(source: str, dest: str, is_source_local=True, is_dest_local=True):
    _copy(source, dest, is_source_local, is_dest_local, shutil.copy2)


def copy_dirs(source: str, dest: str, is_source_local=True, is_dest_local=True):
    _copy(source, dest, is_source_local, is_dest_local, os.rename)
