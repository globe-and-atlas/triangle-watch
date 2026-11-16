---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-06T12:00:00-06:00"
---
# Errors
## 2026-11-06 — Fixture weekday
Error: word-date fixture kept Friday when date messages changed the calendar date.
Cause: tm_wday was assigned 5 independently of injected year/month/day.
Fix: shared Gregorian calendar_weekday computes Sunday-zero weekday from the validated fixture calendar. Host checks include leap day, Sunday, year boundary and default Friday.
Graduated-to: policy.h calendar_weekday; tests/test_geometry.py.

## 2026-11-06 — Near-midnight path rotation collapse
Error: native 23:59 fixture showed hour/minute hands nearly vertical and too narrow; expected minute sample (78,76) was missing.
Cause boundary: source fractional angles and shared ray geometry were correct; rendering through SDK GPath rotation produced the incorrect pixels. SDK internal cause was not isolated.
Fix: explicitly transform each local polygon vertex with Pebble sin_lookup/cos_lookup fixed-point values, then draw the resulting absolute-coordinate polygon without gpath_rotate_to.
Verification: parent reinstalled rebuilt fixture and confirmed 23:59 native checker passes without changing pixel tolerance; other native cases continued passing.
Graduated-to: main.c rotated helper; knowledge/procedural/build_watchface.md.

## Native confirmation
Root recaptured 31 cases after explicit vertex rotation. Late 23:59 hand samples pass without loosening assertions. Production seconds on/off and persisted settings pass. The exact SDK internals remain unproven; direct transformed vertices are the verified workaround.

## 2026-11-06 — Round digital/date footer collision
Error: digital time and ISO date were only8logicalpixels apart and their glyph ink merged.
Fix: place the date13logicalpixels below the digital label; include12-hour+digital+ISO+100%battery in the native fixture matrix. Text remains below the sculpted shield and inside the circle.
Graduated-to: round_analog.h and execution/round_cases.json.
