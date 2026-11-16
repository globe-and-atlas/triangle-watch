---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-06T11:24:00-06:00"
---
# Dependencies
- Pebble SDK 4.33.1; pebble-tool 5.0.40; Emery target only.
- @rebble/clay 1.1.0 pinned in package-lock.json. Settings tests load the actual installed Clay package.
- Pillow 12.3.0 for native capture/contact sheets.
- pytest for host policy verification; system C compiler executes shared native geometry policy.

## Round native QA — 2026-11-06
Pillow12.3.0 is pinned for deterministic screenshot decoding, palette normalization, and contact sheets. This is development tooling only; Pebble runtime dependencies are unchanged. It provides `Image.get_flattened_data()` used by the capture audit. No SDK upgrade was installed.

## 2026-11-06 — Round SDK targets
Installed SDK4.33.1/tool5.0.40 built Emery, Chalk, and Gabbro without an SDK upgrade. Clay1.1.0 recognizes both round platforms. Pillow12.3.0 is native QA tooling only.
