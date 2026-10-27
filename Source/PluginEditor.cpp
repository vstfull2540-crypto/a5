#include "PluginEditor.h"
#include "BinaryData.h"

namespace
{
    class HitAreaLookAndFeel final : public juce::LookAndFeel_V4
    {
    public:
        void drawRotarySlider (juce::Graphics&, int, int, int, int, float,
                               float, float, juce::Slider&) override {}
        void drawToggleButton (juce::Graphics&, juce::ToggleButton& b,
                               bool, bool) override
        {
            // The PNG artwork already contains the switch. Keep a clickable hit area.
            b.setAlpha (0.01f);
        }
        void drawComboBox (juce::Graphics&, int, int, bool,
                           int, int, int, int, juce::ComboBox&) override {}
    };
    HitAreaLookAndFeel hitAreaLookAndFeel;
}

LA2AClassicAudioProcessorEditor::LA2AClassicAudioProcessorEditor (LA2AClassicAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    background = juce::ImageCache::getFromMemory (BinaryData::LA2A_Interface_png,
                                                  BinaryData::LA2A_Interface_pngSize);
    for (auto* s : { &gain, &peak, &attack, &release })
        setupInvisibleSlider (*s);

    meter.addItem ("INPUT", 1);
    meter.addItem ("OUTPUT", 2);
    meter.addItem ("GAIN REDUCTION", 3);
    meter.setLookAndFeel (&hitAreaLookAndFeel);
    meter.setColour (juce::ComboBox::backgroundColourId, juce::Colours::transparentBlack);
    meter.setColour (juce::ComboBox::outlineColourId, juce::Colours::transparentBlack);
    meter.setColour (juce::ComboBox::textColourId, juce::Colours::transparentBlack);
    addAndMakeVisible (meter);

    for (auto* b : { &compress, &power })
    {
        b->setLookAndFeel (&hitAreaLookAndFeel);
        addAndMakeVisible (*b);
    }

    auto& state = processor.parameters;
    gainAttachment = std::make_unique<SliderAttachment> (state, "gain", gain);
    peakAttachment = std::make_unique<SliderAttachment> (state, "peak", peak);
    attackAttachment = std::make_unique<SliderAttachment> (state, "attack", attack);
    releaseAttachment = std::make_unique<SliderAttachment> (state, "release", release);
    meterAttachment = std::make_unique<ComboAttachment> (state, "meter", meter);
    compressAttachment = std::make_unique<ButtonAttachment> (state, "compress", compress);
    powerAttachment = std::make_unique<ButtonAttachment> (state, "power", power);

    setResizable (true, true);
    setResizeLimits (768, 512, 1920, 1280);
    setSize (1152, 768);
}

void LA2AClassicAudioProcessorEditor::setupInvisibleSlider (juce::Slider& s)
{
    s.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    s.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    s.setLookAndFeel (&hitAreaLookAndFeel);
    s.setMouseCursor (juce::MouseCursor::PointingHandCursor);
    addAndMakeVisible (s);
}

void LA2AClassicAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
    if (background.isValid())
        g.drawImage (background, getLocalBounds().toFloat());
}

void LA2AClassicAudioProcessorEditor::resized()
{
    const float sx = getWidth() / 1536.0f;
    const float sy = getHeight() / 1024.0f;
    auto place = [this, sx, sy] (juce::Component& c, float x, float y, float w, float h)
    {
        c.setBounds (juce::roundToInt (x * sx), juce::roundToInt (y * sy),
                     juce::roundToInt (w * sx), juce::roundToInt (h * sy));
    };

    // Transparent interactive hit zones aligned to the artwork in LA2A_Interface.png.
    place (gain, 220, 412, 190, 190);
    place (peak, 1090, 412, 190, 190);
    place (attack, 535, 690, 150, 145);
    place (release, 820, 690, 150, 145);
    place (meter, 145, 735, 110, 90);
    place (compress, 1390, 455, 115, 120);
    place (power, 1345, 755, 90, 100);
}

