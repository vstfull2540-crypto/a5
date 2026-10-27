# LA-2A Classic — PNG-skinned JUCE plugin

A starter JUCE/C++ audio plugin project for Windows 10/11, Visual Studio 2022 and GitHub Actions. It builds VST3 and Standalone targets. The GUI uses **PNG files only** for its artwork; there are no SVG assets.

> Note: This is an original optical-style compressor approximation and branded interface demo, not an official Teletronix/Universal Audio product and not a component-accurate LA-2A emulation. The uploaded PNG artwork is used as the GUI background.

## Build on GitHub

1. Create a new GitHub repository.
2. Upload all files and folders from this project, preserving `.github/workflows/build-windows.yml`.
3. Commit to `main` (or `master`).
4. Open the repository's **Actions** tab and select **Build LA-2A Classic (Windows)**.
5. Wait for the workflow to finish. Open the successful run and download the artifact named `LA-2A-Classic-VST3-Windows-x64`.
6. Extract the artifact ZIP. Install/copy `LA-2A Classic.vst3` to:
   - `C:\\Program Files\\Common Files\\VST3`
7. Rescan plugins in your DAW.

GitHub Actions downloads JUCE 8.0.6 during CMake configuration, so you do not need to commit the JUCE source tree.

## Build locally (Developer PowerShell for VS 2022)

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release --target LA2AClassic_VST3 LA2AClassic_Standalone --parallel
```

## PNG assets

`Assets/LA2A_Interface.png` is the full interface background. The remaining PNGs are separated components from the provided asset ZIP and are embedded into the plugin binary through JUCE BinaryData. All controls are transparent interactive hit zones over the PNG artwork; move/drag over the pictured knobs to adjust them.

## Parameters

- Gain: output makeup level
- Peak Reduction: compression amount
- Attack: envelope attack time (ms)
- Release: envelope release time (ms)
- Meter: input / output / gain reduction display selector (UI state)
- Compress: bypasses/enables compression
- Power: bypasses the processing

Parameter values are saved/restored by the host.

## Build status and troubleshooting

The GUI LookAndFeel method signature has been corrected for JUCE 8. The workflow now builds VST3 and Standalone separately so the failing target is easy to identify, and uploads CMake diagnostics even when a build fails.

## Troubleshooting

- If CMake cannot download JUCE, check GitHub Actions network access and rerun the workflow.
- If the workflow fails at the VST3 verification step, inspect the configure/build logs before downloading artifacts.
- The generated plugin is x64. Use a 64-bit DAW.
