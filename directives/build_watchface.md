---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-06T11:24:00-06:00"
---
# Build Triangle
Goal: produce the clean native Emery bundle plus verifiable local test evidence.
Inputs: watchface C/JS source, package-lock.json.
Execution: build.py, verify.py, tests/test_geometry.py, watchface/settings.test.js.
Output: dist/Triangle-0.1.0.pbw.
Safety: no .env reads; no SDK lockfile commit; serialize emulator operations through root owner.
Native review: capture_native.py --monitor PORT covers palettes, boundary times, formats, seconds and battery visibility. Independent verifier reviews the contract and final native images.
