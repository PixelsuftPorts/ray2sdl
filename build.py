# Pixelsuft Build System
import os
import sys
import json
import subprocess
import threading


class App:
    def __init__(self, argv: list) -> None:
        self.exit_code = 0
        self.encoding = sys.getdefaultencoding()
        self.argv = argv
        self.cwd = (os.path.dirname(self.argv[0]) if len(self.argv) > 0 else None) or \
            os.path.dirname(__file__) or os.getcwd()
        os.chdir(self.cwd)
        self.src_dir = os.path.join(self.cwd, 'src')
        self.is_win = sys.platform == 'win32'
        self.cache_dir = self.p('.cache')
        if not os.path.isdir(self.cache_dir):
            os.mkdir(self.cache_dir)
        self.config_path = self.pc('config.json')
        self.config = json.loads(self.read(self.config_path)) if os.path.isfile(self.config_path) else {
            'compiler': os.getenv('cc') or os.getenv('compiler') or os.getenv('ccompiler') or 'cc',
            'output': 'test.exe' if self.is_win else 'test',
            'flags': self.get_flags(),
            'libs': self.get_libs(),
            'cache': {}
        }
        self.shell = True
        self.opt = '-O0'
        self.run = False
        self.rebuild = False
        self.threaded = False
        self.check_args()
        self.c_fns = [fn for fn in os.listdir(self.src_dir) if fn.endswith('.c')]
        self.to_build = []
        for fn in self.c_fns:
            fp = self.p('src', fn)
            f_hash = self.hash_file(fp)
            new_fn = os.path.join('src', fn)
            if self.rebuild or new_fn not in self.config['cache'] or not f_hash == self.config['cache'][new_fn]:
                self.to_build.append({
                    'fn': new_fn,
                    'fp': fp,
                    'out': self.pc(fn + '.o'),
                    'hash': f_hash
                })
        # TODO: don't hardcode fn
        fn = 'test1.c'
        self.to_build.append({
            'fn': fn,
            'fp': fn,
            'out': self.pc(fn + '.o'),
            'hash': self.hash_file(self.p(fn))
        })
        if self.threaded:
            self.build_threaded()
        else:
            self.build()
        self.write_config()

    def build_threaded(self) -> None:
        raise NotImplementedError('TODO: threaded build')

    def build(self) -> None:
        if self.to_build:
            print('[Building]----------------------------------------')
            for file in self.to_build:
                cmd = [self.config['compiler'], file['fp'], self.opt] + self.config['flags'] + ['-c', '-o', file['out']]
                self.print_cmd(cmd)
                try:
                    output = subprocess.check_output(cmd, shell=self.shell, encoding=self.encoding)
                    self.config['cache'][file['fn']] = file['hash']
                    if output:
                        print(output)
                except subprocess.CalledProcessError as err:
                    self.exit_code = 1
                    return
        print('[Linking]-----------------------------------------')
        out_path = self.p(self.config['output'])
        try:
            cmd = [
                self.config['compiler'], '-o', out_path, os.path.join('.cache/', '*.o'), self.opt
            ] + self.config['flags'] + self.config['libs']
            self.print_cmd(cmd)
            output = subprocess.check_output(cmd, shell=self.shell, encoding=self.encoding)
            if output:
                print(output)
        except subprocess.CalledProcessError as err:
            if err.stdout:
                print(err.stdout)
            if err.stderr:
                print(err.stderr)
            self.exit_code = 1
            return
        if self.run:
            print('[Running]-----------------------------------------')
            cmd = [out_path] + self.argv[self.argv.index('--run') + 1:]
            self.print_cmd(cmd)
            subprocess.Popen(cmd)

    def write_config(self) -> None:
        self.write(self.config_path, json.dumps(self.config, indent=4))

    @staticmethod
    def print_cmd(cmd: list) -> None:
        print(' '.join(cmd))
        '''result = ''
        for cmd_part in cmd:
            result += ' '
            if ' ' in cmd_part:
                result += '"' + cmd_part + '"'
            else:
                result += cmd_part
        print(result and result[1:])'''

    @staticmethod
    def hash_file(path: str) -> str:
        stat = os.stat(path)
        return f'{stat.st_size}_{stat.st_ctime_ns}'

    def get_flags(self) -> list:
        if self.is_win:
            return [
                r'-ISDL2\include',
                r'-Isrc\include',
                r'-Wall'
            ]
        result = self.get_output(['sdl2-config', '--cflags']).split('\n')[0].strip().split(' ') + [
            '-lm',
            '-I/usr/include',
            '-Iinclude',
            '-Wall'
        ]
        if sys.platform.endswith('bsd'):
            result.append('-I/usr/pkg/include' if sys.platform.lower().startswith('net') else '-I/usr/local/include')
        return result

    def get_libs(self) -> list:
        if self.is_win:
            return [
                r'-LSDL2\lib',
                r'-LSDL2_ttf\lib',
                r'-LSDL2_mixer\lib',
                r'-LSDL2_image\lib',
                r'-lSDL2main',
                r'-lSDL2',
                # r'-lSDL2_ttf',
                # r'-lSDL2_mixer',
                # r'-lSDL2_image'
            ]
        result = self.get_output(['sdl2-config', '--libs']).split('\n')[0].strip().split(' ') + [
            r'-lSDL2main',
            r'-lSDL2',
            r'-lSDL2_ttf',
            r'-lSDL2_mixer',
            r'-lSDL2_image'
        ]
        if sys.platform.endswith('bsd'):
            result.append('-L/usr/pkg/lib' if sys.platform.lower().startswith('net') else '-L/usr/local/lib')
        return result
    
    def get_output(self, *cmd: any) -> str:
        try:
            return subprocess.check_output(*cmd, shell=self.shell, encoding=self.encoding)
        except subprocess.CalledProcessError as err:
            self.exit_code = 1
            raise RuntimeError(f'Failed to get output of the command [{err}]')

    def check_args(self) -> None:
        if '--run' in self.argv:
            self.run = True
        if '--rebuild' in self.argv:
            self.rebuild = True
        if '--threaded' in self.argv:
            self.threaded = True
        if '--o1' in self.argv:
            self.opt = '-O1'
        if '--o2' in self.argv:
            self.opt = '-O2'
        if '--o3' in self.argv:
            self.opt = '-O3'
        if '--os' in self.argv:
            self.opt = '-Os'
        if '--og' in self.argv:
            self.opt = '-Og'
        if '--of' in self.argv:
            self.opt = '-Ofast'
        if '--cc' in self.argv:
            self.config['compiler'] = self.argv[self.argv.index('--cc') + 1]

    def p(self, *path: str) -> str:
        return os.path.join(self.cwd, *path)

    def pc(self, *path: str) -> str:
        return os.path.join(self.cache_dir, *path)

    def read(self, path: str) -> str:
        file_handle = open(path, 'rb')
        result = file_handle.read()
        file_handle.close()
        return result.decode(self.encoding, errors='replace')

    def write(self, path: str, content: str) -> int:
        file_handle = open(path, 'wb')
        result = file_handle.write(content.encode(self.encoding, errors='replace'))
        file_handle.close()
        return result


if __name__ == '__main__':
    sys.exit(App(sys.argv).exit_code)
