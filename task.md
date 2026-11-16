---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-06T11:10:00-06:00"
---
# Triangle native watchface
## Contract
- Surface: native Emery 200×228.
- Execution: build.py, verify.py, capture_native.py.
- Output: dist/Triangle-0.1.0.pbw.
- Safety: no emulator operation in this implementation session.
- Safety: no publication, push, commit, or .env access.
## Validation contract
- [x] The dial is a right-pointing asymmetric shield.
- [x] The hour hand includes the minute fraction.
- [x] Hand vertices remain inside the shield for 1,440 minute positions.
- [x] Marker vertices remain inside the shield for 60 positions.
- [x] Six palette presets are selectable.
- [x] Custom colors are selectable.
- [x] Seconds Off uses minute subscription.
- [x] Date Off hides date text.
- [x] Word date is selectable.
- [x] ISO date is selectable.
- [x] US date is selectable.
- [x] EU date is selectable.
- [x] Battery uses the battery service.
- [x] Digital footer defaults to Off.
- [x] Digital footer supports watch default.
- [x] Digital footer supports 12-hour mode.
- [x] Digital footer supports 24-hour mode.
- [x] Actual Clay Save sends normalized settings.
- [x] Clay Cancel preserves saved settings.
- [x] Clay reload restores saved settings.
- [x] Host policy tests pass.
- [x] Production bundle matches the clean native build.
- [x] Native fixture capture covers at least 20 cases.
- [x] Parent fresh verifier reviews final output.
## Progress
- Contract written before implementation; parent owns native QA.

## Round compatibility — 2026-11-06
- [x] Emery remains a build target.
- [x] Chalk is a build target.
- [x] Gabbro is a build target.
- [x] The UUID is preserved.
- [x] Existing settings key mappings are preserved.
- [x] Native Chalk layout passes visual review.
- [x] Native Gabbro layout passes visual review.
- [x] Clean production PBW contains three native binaries.
- [x] Existing host/settings checks pass.
- [x] Fresh independent review passes.
No store publication is authorized by this compatibility request.

## Store round update — 2026-11-06
- [x] Publish the verified three-platform production update.
- [x] Preserve the current animated preview.
- [x] Add native screenshots for Chalk.
- [x] Add native screenshots for Gabbro.
- [x] Verify the public download matches the production artifact.
- [x] Verify public round screenshot pixels.
- [x] Fresh independent public-proof review passes.
