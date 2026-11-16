---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-06T11:24:00-06:00"
---
# Build and verify Triangle
1. Run `npm install --ignore-scripts` inside watchface.
2. Run `node watchface/settings.test.js` from repository root.
3. Run `python3 -m pytest -q`.
4. Run `python3 execution/build.py`.
5. Run `python3 execution/verify.py`.

Production archive is dist/Triangle-0.1.0.pbw. Fixture flags are TRI_FIXTURE, TRI_HOUR, TRI_MINUTE, TRI_SECOND, TRI_BATTERY, TRI_YEAR, TRI_MONTH, TRI_DAY. The production script strips TRI_* and starts with pebble clean.

Serial emulator owner may install `.tmp/triangle-fixture.pbw`, then run `python3 execution/capture_native.py --monitor PORT`. Fixture messages 11000/11001/11002 set hour/minute/second;11004 sets battery;11005/11006/11007 set year/month/day. The helper captures 31 cases and restores defaults. Message keys 10000..10008 are Palette, Background, Foreground, Accent, TimeFormat, DateFormat, ShowSeconds, DigitalTime, ShowBattery.

Capture output is native 200×228; QEMU backlight values are normalized to the palette peak. .tmp/native-checks.json records checks; media/triangle-palettes.png is the six-palette sheet. Capture helper is prepared but not executed by the implementation agent.

## Native polygon rotation
Use the explicit fixed-point vertex transformation in main.c for hand polygons. SDK GPath rotation produced collapsed near-360° native hands at 23:59 in this SDK/emulator combination. The replacement passed the original native pixel checks without relaxed tolerance. Exact SDK internals remain undiagnosed; preserve the boundary case when refactoring.
