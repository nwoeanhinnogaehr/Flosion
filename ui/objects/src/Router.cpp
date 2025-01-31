#include <Flosion/UI/Objects/Router.hpp>
#include <Flosion/UI/Core/ObjectFactory.hpp>
#include <Flosion/UI/Core/NumberWire.hpp>
#include <Flosion/UI/Core/SoundWire.hpp>
#include <Flosion/UI/Core/Font.hpp>

#include <GUI/Helpers/CallbackButton.hpp>

namespace flui {

    Router::Router(){
        addToLeft(makePeg(m_router.getSoundInput()));

        auto fc = std::make_unique<ui::Boxed<ui::FreeContainer>>();
        fc->setBackgroundColor(0x4f192aff);
        fc->setBorderColor(0xFFFFFFFF);
        fc->setBorderThickness(2.0f);
        auto& b = fc->add<ui::VerticalList>(ui::FreeContainer::Center, ui::FreeContainer::Center);
        b.setPadding(5.0f);
        b.push_back<ui::Text>("Router", getFont());
        
        b.push_back<ui::CallbackButton>(
            "Add Sound Output",
            getFont(),
            [&](){
                addSoundSource();
            }
        );
        b.push_back<ui::CallbackButton>(
            "Add Number Input",
            getFont(),
            [&](){
                addNumberInput();
            }
        );

        setBody(std::move(fc));
    }

    void Router::addSoundSource(){
        auto ss = m_router.addSoundSource();
        
        auto b = std::make_unique<OutputBlock>(this, ss);

        b->addItem(makePeg(ss));

        for (std::size_t j = 0, jEnd = m_router.numNumberSources(); j != jEnd; ++j){
            auto ni = m_router.getNumberInput(m_router.getNumberSource(j) , ss);
            b->addItem(makePeg(ni, "Number Input " + std::to_string(j + 1)));
        }

        m_outputBlocks.push_back(b.get());
        addToRight(std::move(b));
    }

    void Router::addNumberInput(){
        auto ns = m_router.addNumberSource();

        auto fc = std::make_unique<ui::Boxed<ui::FreeContainer>>();
        fc->setWidth(75.0f, true);
        fc->setBackgroundColor(0x8a2444ff);
        fc->setBorderRadius(10.0f);
        fc->setBorderThickness(1.0f);
        fc->setBorderColor(0xFFFFFFFF);
        fc->adopt(
            ui::FreeContainer::InsideLeft,
            ui::FreeContainer::Center,
            makePeg(ns, "Number Output " + std::to_string(m_router.numNumberSources()))
        );
        fc->add<ui::CallbackButton>(
            ui::FreeContainer::InsideRight,
            ui::FreeContainer::Center,
            "x",
            getFont(),
            [&,ns,fc = fc.get()](){
                m_router.removeNumberSource(ns);
                fc->close();
            }
        );

        addToLeft(std::move(fc));

        for (std::size_t j = 0, jEnd = m_outputBlocks.size(); j != jEnd; ++j){
            auto ni = m_router.getNumberInput(ns, m_router.getSoundSource(j));
            auto& ob = m_outputBlocks[j];
            ob->addItem(makePeg(ni, "Number Input " + std::to_string(m_router.numNumberSources())));
        }
    }


    Router::OutputBlock::OutputBlock(Router* parentRouter, const flo::SoundSource* ss)
        : m_parentRouter(parentRouter)
        , m_soundSource(ss)
        , m_list(add<ui::VerticalList>(ui::FreeContainer::InsideRight, ui::FreeContainer::Center)) {

        setWidth(75.0f, true);

        setBackgroundColor(0x8a2444ff);
        setBorderRadius(10.0f);
        setBorderThickness(1.0f);
        setBorderColor(0xFFFFFFFF);

        add<ui::CallbackButton>(
            ui::FreeContainer::InsideLeft,
            ui::FreeContainer::Center,
            "X",
            getFont(),
            [&](){
                m_parentRouter->m_router.removeSoundSource(m_soundSource);
                auto& ob = m_parentRouter->m_outputBlocks;
                auto it = std::find(ob.begin(), ob.end(), this);
                assert(it != ob.end());
                ob.erase(it);
                close();
            }
        );
    }

    void Router::OutputBlock::addItem(std::unique_ptr<ui::Element> e){
        m_list.push_back(std::move(e));
    }

    RegisterFactoryObject(Router, "Router");

} // namespace flui
