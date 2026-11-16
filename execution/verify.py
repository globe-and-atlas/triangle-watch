"""Verify Triangle identity, target and exact production binary."""
from pathlib import Path
import json
import zipfile

ROOT = Path(__file__).resolve().parents[1]


def main() -> None:
    package = json.loads((ROOT / 'watchface/package.json').read_text())
    assert package['pebble']['displayName'] == 'Triangle'
    assert package['pebble']['targetPlatforms'] == ['emery','chalk','gabbro']
    with zipfile.ZipFile(ROOT / f"dist/Triangle-{package['version']}.pbw") as bundle:
        info = json.loads(bundle.read('appinfo.json'))
        assert info['uuid'] == package['pebble']['uuid']
        assert info['versionLabel'] == package['version']
        assert info['targetPlatforms'] == package['pebble']['targetPlatforms']
        assert 'emery/pebble-app.bin' in bundle.namelist()
        assert 'pebble-js-app.js' in bundle.namelist()
        for platform in ['emery','chalk','gabbro']:
            assert bundle.read(platform+'/pebble-app.bin') == (ROOT / 'watchface/build' / platform / 'pebble-app.bin').read_bytes()
    print('PASS: Triangle production bundle for Emery, Chalk and Gabbro')


if __name__ == '__main__':
    main()
