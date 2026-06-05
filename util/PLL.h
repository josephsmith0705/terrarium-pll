class PLL
{
    public:
        float dry_signal_threshold = 0.001f;
        float vco_threshold = 0.0f;
        float response_speed = 5.0f;

        float Process(float dry_signal)
        {
            const bool input_edge = DetectRisingEdge(dry_signal, dry_signal_high, dry_signal_threshold);
            const bool vco_edge = DetectRisingEdge(vco_output, vco_high, vco_threshold);

            const float phase_error = PhaseComparator(input_edge, vco_edge);
            UpdateVCOFrequency(phase_error);
            return vco_output;
        }

    private:
        bool dry_signal_high = false;
        bool vco_high = false;
        float vco_output = 0.0f;
        float vco_frequency = 0.0f;
        float vco_phase = 0.0f;
        float sample_rate = 48000.0f;
        float min_vco_frequency = 0.0f;
        float max_vco_frequency = 5400.0f;

        void UpdateVCOFrequency(float phase_error)
        {
            vco_frequency += phase_error * response_speed;

            if (vco_frequency < min_vco_frequency)
            {
                vco_frequency = min_vco_frequency;
            }
            else if (vco_frequency > max_vco_frequency)
            {
                vco_frequency = max_vco_frequency;
            }

            vco_output = GenerateVcoSample(vco_frequency);
        }

        float PhaseComparator(bool input_edge, bool vco_edge)
        {
            return (input_edge ? 1.0f : 0.0f) - (vco_edge ? 1.0f : 0.0f);
        }

        float GenerateVcoSample(float target_vco_frequency)
        {
            vco_phase += target_vco_frequency / sample_rate;
            if (vco_phase >= 1.0f)
            {
                vco_phase -= 1.0f;
            }

            return (vco_phase < 0.5f) ? 1.0f : -1.0f;
        }

        bool DetectRisingEdge(float input, bool &is_high, float threshold)
        {
            bool rising_edge = false;

            if (!is_high && input > threshold)
            {
                rising_edge = true;
                is_high = true;
            }
            else if (is_high && input <= -threshold)
            {
                is_high = false;
            }

            return rising_edge;
        }
};