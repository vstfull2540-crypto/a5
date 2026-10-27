#include "PluginProcessor.h"
#include "PluginEditor.h"

LA2AClassicAudioProcessor::LA2AClassicAudioProcessor()
    : AudioProcessor (BusesProperties()
        .withInput ("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      parameters (*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

LA2AClassicAudioProcessor::APVTS::ParameterLayout
LA2AClassicAudioProcessor::createParameterLayout()
{
    APVTS::ParameterLayout layout;
    layout.add (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "gain", 1 }, "Gain",
        juce::NormalisableRange<float> (0.0f, 100.0f, 0.1f), 50.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "peak", 1 }, "Peak Reduction",
        juce::NormalisableRange<float> (0.0f, 100.0f, 0.1f), 35.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "attack", 1 }, "Attack",
        juce::NormalisableRange<float> (0.1f, 10.0f, 0.01f), 1.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "release", 1 }, "Release",
        juce::NormalisableRange<float> (50.0f, 5000.0f, 1.0f), 500.0f));
    layout.add (std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID { "meter", 1 }, "Meter",
        juce::StringArray { "Input", "Output", "Gain Reduction" }, 2));
    layout.add (std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID { "compress", 1 }, "Compress", true));
    layout.add (std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID { "power", 1 }, "Power", true));
    return layout;
}

void LA2AClassicAudioProcessor::prepareToPlay (double sampleRate, int)
{
    currentSampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
    envelope = 0.0f;
}

void LA2AClassicAudioProcessor::releaseResources() {}

bool LA2AClassicAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto in = layouts.getMainInputChannelSet();
    const auto out = layouts.getMainOutputChannelSet();
    return in == out && (out == juce::AudioChannelSet::mono()
                      || out == juce::AudioChannelSet::stereo());
}

void LA2AClassicAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                               juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const int channels = buffer.getNumChannels();
    const int samples = buffer.getNumSamples();
    const bool enabled = parameters.getRawParameterValue("power")->load() > 0.5f;
    const bool compress = parameters.getRawParameterValue("compress")->load() > 0.5f;
    const float gainDb = (parameters.getRawParameterValue("gain")->load() - 50.0f) * 0.24f;
    const float reduction = parameters.getRawParameterValue("peak")->load();
    const float attackMs = parameters.getRawParameterValue("attack")->load();
    const float releaseMs = parameters.getRawParameterValue("release")->load();
    const float makeup = juce::Decibels::decibelsToGain(gainDb);
    const float threshold = juce::jmap(reduction, 0.0f, 100.0f, 1.0f, 0.12f);
    const float attackCoeff = std::exp(-1.0f / (float(currentSampleRate) * attackMs * 0.001f));
    const float releaseCoeff = std::exp(-1.0f / (float(currentSampleRate) * releaseMs * 0.001f));

    for (int i = 0; i < samples; ++i)
    {
        float peak = 0.0f;
        for (int ch = 0; ch < channels; ++ch)
            peak = juce::jmax (peak, std::abs (buffer.getSample (ch, i)));

        const float coeff = peak > envelope ? attackCoeff : releaseCoeff;
        envelope = coeff * envelope + (1.0f - coeff) * peak;

        float compGain = 1.0f;
        if (enabled && compress && envelope > threshold)
        {
            // Smooth, program-dependent optical-style leveling approximation.
            const float over = envelope / juce::jmax (threshold, 0.0001f);
            compGain = std::pow (over, -0.55f);
        }

        const float g = enabled ? makeup * compGain : 1.0f;
        for (int ch = 0; ch < channels; ++ch)
            buffer.setSample (ch, i, buffer.getSample (ch, i) * g);
    }
}

juce::AudioProcessorEditor* LA2AClassicAudioProcessor::createEditor()
{
    return new LA2AClassicAudioProcessorEditor (*this);
}

void LA2AClassicAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void LA2AClassicAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary (data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName (parameters.state.getType()))
        parameters.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LA2AClassicAudioProcessor();
}
