"""Clean-build production Triangle; fixture flags are deliberately removed."""
from pathlib import Path
import json
import os
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[1]


def main() -> None:
    env = {key: value for key, value in os.environ.items() if not key.startswith('TRI_')}
    for command in [['pebble', 'clean'], ['pebble', 'build']]:
        subprocess.run(command, cwd=ROOT / 'watchface', env=env, check=True)
    version=json.loads((ROOT/'watchface/package.json').read_text())['version']
    target = ROOT / f'dist/Triangle-{version}.pbw'
    target.parent.mkdir(exist_ok=True)
    shutil.copy2(ROOT / 'watchface/build/watchface.pbw', target)
    print(f'Production bundle: {target}')


if __name__ == '__main__':
    main()
