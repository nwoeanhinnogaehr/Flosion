#include <Flosion/UI/Objects/WaveGenerator.hpp>
#include <Flosion/UI/Core/ObjectFactory.hpp>
#include <Flosion/UI/Core/Font.hpp>
#include <Flosion/UI/Core/NumberWire.hpp>
#include <Flosion/UI/Core/SoundWire.hpp>

#include <GUI/Helpers/ToggleButton.hpp>
#include <GUI/Helpers/CallbackButton.hpp>

namespace flui {

    WaveGenerator::WaveGenerator(){
        addToTop(makePeg(&m_waveGen.phase, "Phase"));
        addToLeft(makePeg(&m_waveGen.waveFunction, "Wave Function"));
        addToLeft(makePeg(&m_waveGen.frequency, "Frequency"));
        addToTop(makePeg(&m_waveGen.currentTime, "Current Time"));
        addToRight(makePeg(&m_waveGen, "Output"));

        auto bod = std::make_unique<ui::Boxed<ui::VerticalList>>();
        bod->push_back<ui::Text>("Wave Generator", getFont());
        bod->push_back<ui::ToggleButton>(
            false,
            getFont(),
            [&](bool v){
                m_waveGen.setPhaseSync(v);
            },
            std::pair<ui::String, ui::String>{"Sync Off", "Sync On"}
        );
        bod->setPadding(5.0f);
        bod->setBackgroundColor(0x22BB22FF);
        setBody(std::move(bod));
    }
    RegisterFactoryObject(WaveGenerator, "WaveGenerator");

} // namespace flui
