cmake_minimum_required(VERSION 3.22)
project(LA2AClassicPlugin VERSION 1.0.0 LANGUAGES CXX)

# JUCE is fetched from its official repository during configure/build.
include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.6
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(LA2AClassic
    COMPANY_NAME "Muepim Studio"
    BUNDLE_ID "com.muepimstudio.la2aclassic"
    PLUGIN_MANUFACTURER_CODE Muep
    PLUGIN_CODE La2A
    FORMATS VST3 Standalone
    PRODUCT_NAME "LA-2A Classic"
    DESCRIPTION "PNG-skinned optical-style leveling amplifier"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

target_sources(LA2AClassic PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
)

target_compile_features(LA2AClassic PRIVATE cxx_std_17)
target_compile_definitions(LA2AClassic PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

juce_add_binary_data(LA2AAssets
    SOURCES
        Assets/LA2A_Interface.png
        Assets/01_full_plugin_interface.png
        Assets/02_top_toolbar.png
        Assets/03_arm_nongmuang_logo.png
        Assets/04_la2a_title.png
        Assets/05_muepim_studio_logo.png
        Assets/06_gain_knob.png
        Assets/07_vu_meter.png
        Assets/08_peak_reduction_knob.png
        Assets/09_meter_selector.png
        Assets/10_attack_knob.png
        Assets/11_release_knob.png
        Assets/12_limit_compress_switch.png
        Assets/13_power_indicator.png
        Assets/14_power_switch.png
        Assets/15_top_power_button.png
        Assets/16_preset_selector.png
        Assets/17_nam_model_selector.png
        Assets/18_ab_compare_controls.png
        Assets/19_settings_button.png
)
target_link_libraries(LA2AClassic PRIVATE
    LA2AAssets
    juce::juce_audio_utils
    juce::juce_dsp
)
