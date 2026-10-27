#pragma once
#include "PluginProcessor.h"

class LA2AClassicAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit LA2AClassicAudioProcessorEditor (LA2AClassicAudioProcessor&);
    ~LA2AClassicAudioProcessorEditor() override = default;
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    LA2AClassicAudioProcessor& processor;
    juce::Image background;
    juce::Slider gain, peak, attack, release;
    juce::ComboBox meter;
    juce::ToggleButton compress, power;
    using SliderAttachment = LA2AClassicAudioProcessor::APVTS::SliderAttachment;
    using ButtonAttachment = LA2AClassicAudioProcessor::APVTS::ButtonAttachment;
    using ComboAttachment = LA2AClassicAudioProcessor::APVTS::ComboBoxAttachment;
    std::unique_ptr<SliderAttachment> gainAttachment, peakAttachment, attackAttachment, releaseAttachment;
    std::unique_ptr<ComboAttachment> meterAttachment;
    std::unique_ptr<ButtonAttachment> compressAttachment, powerAttachment;
    void setupInvisibleSlider (juce::Slider&);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LA2AClassicAudioProcessorEditor)
};
