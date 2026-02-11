#pragma once

#include <JuceHeader.h>

namespace Parameters
{

    const int VERSION_HINT = 1;

    const String ID_MODULATION = "mod";
    const String ID_DECAY = "decay";
    const String ID_DAMP = "damp";
    const String ID_MIX = "mix";
    const String ID_PREDELAY = "predelay";
    const String ID_SCALE = "scale";

    const String ID_INPUT_DIFFUSION = "indiff";

    const String ID_TAIL_LOW_F = "taillowf";
    const String ID_TAIL_LOW_G = "taillowg";
    const String ID_TAIL_HIGH_F = "tailhif";
    const String ID_TAIL_HIGH_G = "tailhig";


    using ParamLayout = AudioProcessorValueTreeState::ParameterLayout;
    static ParamLayout createLayout()
    {
        ParamLayout layout;
        layout.add(
            std::make_unique<AudioParameterFloat>(ParameterID(ID_PREDELAY, VERSION_HINT), "Predelay", 0.f, 250.f, 2.0f),
            std::make_unique<AudioParameterBool>(ParameterID(ID_INPUT_DIFFUSION, VERSION_HINT), "Input Diffusion", true),
            std::make_unique<AudioParameterFloat>(ParameterID(ID_DECAY, VERSION_HINT), "Decay", 0.f, 1.f, 0.2f),
            std::make_unique<AudioParameterFloat>(ParameterID(ID_DAMP, VERSION_HINT), "Dampening", 0.f, 1.f, 0.45f),
            std::make_unique<AudioParameterFloat>(ParameterID(ID_MODULATION, VERSION_HINT), "Modulation", 0.f, 1.f, 0.3f),
            std::make_unique<AudioParameterFloat>(ParameterID(ID_SCALE, VERSION_HINT), "Size", 0.f, 1.f, 1.f),

            std::make_unique<AudioParameterFloat>(ParameterID(ID_TAIL_LOW_F, VERSION_HINT), "Tail Low F", 1.f, 250.f, 80.0f),
            std::make_unique<AudioParameterFloat>(ParameterID(ID_TAIL_LOW_G, VERSION_HINT), "Tail Low G", -24.f, 0.f, 0.0f),
            std::make_unique<AudioParameterFloat>(ParameterID(ID_TAIL_HIGH_F, VERSION_HINT), "Tail High F", 5000.f, 15000.f, 10000.0f),
            std::make_unique<AudioParameterFloat>(ParameterID(ID_TAIL_HIGH_G, VERSION_HINT), "Tail High G", -24.f, 0.f, 0.0f),

            std::make_unique<AudioParameterFloat>(ParameterID(ID_MIX, VERSION_HINT), "Mix", 0.f, 1.f, 0.5f)
        );
        return layout;
    }
}