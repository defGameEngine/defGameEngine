import subprocess


class Command:
    def __init__(self, tool: str):
        self.tool = tool
        self.arguments = []

    def add_argument(self, argument: str):
        self.arguments.append(argument)

    def add_flag(self, flag: str, value: str | None = None):
        arg = '-' + flag

        if value:
            arg += '=' + value
        
        self.arguments.append(arg)


class Builder:
    def __init__(self):
        self.commands = []

    def add_command(self, command: Command):
        self.commands.append(command)

    def execute(self, show_stdout=False, show_stderr=False, show_prompt=False):
        for command in self.commands:
            prompt = f'{command.tool} {' '.join(command.arguments)}'

            if show_prompt:
                print(prompt)

            run = subprocess.run(prompt, capture_output=True)

            if show_stdout and len(run.stdout) > 0:
                print(run.stdout.decode("utf-8"))

            if show_stderr and len(run.stderr) > 0:
                print(run.stderr.decode("utf-8"))
