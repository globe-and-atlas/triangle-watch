---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-06T11:24:00-06:00"
---
# Triangle
Native Pebble Time 2 watchface: a right-pointing sculpted shield, thin light batons, slim hands and a red seconds hand. Original identity inspired by triangular mid-century watches.

Six palettes plus custom colors; optional seconds; optional battery; date Off, word, ISO, US or EU; optional digital footer with watch-default, 12-hour or 24-hour time. Digital footer defaults Off.

Build: `python3 execution/build.py`. Verify: `python3 execution/verify.py`. Tests: `python3 -m pytest -q` and `node watchface/settings.test.js`.

Native fixture capture is intentionally owned by the root session: build with `TRI_FIXTURE=1`, install serially, then `python3 execution/capture_native.py --monitor PORT`. Production builds remove TRI_* variables and clean first.

No store publication is part of this implementation.


## Round-compatible release — 0.1.1

Triangle includes native layouts for Pebble Time2 (Emery200×228), original Time Round (Chalk180×180), and Round2 (Gabbro260×260). The rectangular layout and existing settings identities are preserved.

Build: `python3 execution/build.py`, then `python3 execution/verify.py`. Local bundle: `dist/Triangle-0.1.1.pbw`. Native round previews: `media/round/`. Repeatable fixture/capture instructions: [round compatibility QA](knowledge/procedural/round_compatibility.md).

This update is [publicly available](https://apps.repebble.com/a78ddf0105ec4f4598c3fedc) with round screenshots. Public PBW bytes and screenshot pixels verify; physical-watch validation remains separate.
