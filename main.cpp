#include <algorithm>

#include <per/sai.h>

#include <util/Terrarium.h>
#include <util/PLL.h>

namespace
{
    Terrarium terrarium;
    PLL pll;
    bool effect_enabled = true;
    float output_level = 0.8f;

    void processAudioBlock(
        daisy::AudioHandle::InputBuffer in,
        daisy::AudioHandle::OutputBuffer out,
        size_t size)
    {
        for (size_t i = 0; i < size; ++i)
        {
            const float dry_signal = in[0][i];

            const float wet_signal = pll.Process(dry_signal);
            const float output_signal = effect_enabled ? (wet_signal * output_level) : dry_signal;
            
            out[0][i] = std::clamp(output_signal, -1.0f, 1.0f);
            out[1][i] = out[0][i];
        }
    }   
}

int main()
{
    terrarium.Init(true);

    auto& knob_input_threshold = terrarium.knobs[2];
    auto& knob_vco_threshold = terrarium.knobs[3];
    auto& knob_response_speed = terrarium.knobs[4];
    auto& knob_output_level = terrarium.knobs[5];

    auto& toggle_effect_enable = terrarium.toggles[0];

    auto& led_effect = terrarium.leds[0];

    terrarium.seed.SetAudioBlockSize(2);
    terrarium.seed.SetAudioSampleRate(daisy::SaiHandle::Config::SampleRate::SAI_48KHZ);
    terrarium.seed.StartAudio(processAudioBlock);


    terrarium.Loop(100, [&]() {
        const float input_threshold = 0.0001f + (0.1f * knob_input_threshold.Process());
        const float vco_threshold = 0.1f * knob_vco_threshold.Process();
        const float response_speed = 0.01f + (5.0f * knob_response_speed.Process());
        output_level = knob_output_level.Process();

        pll.dry_signal_threshold = std::clamp(input_threshold, 0.0001f, 1.0f);
        pll.vco_threshold = std::clamp(vco_threshold, 0.0f, 1.0f);
        pll.response_speed = std::clamp(response_speed, 0.0f, 5000.0f);

        effect_enabled = toggle_effect_enable.Pressed();
        led_effect.Set(effect_enabled ? 1.0f : 0.0f);
    });
}
