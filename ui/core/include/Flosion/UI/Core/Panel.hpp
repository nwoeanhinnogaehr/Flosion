#pragma once

#include <Flosion/Core/SoundSource.hpp>
#include <Flosion/Core/NumberSource.hpp>
#include <GUI/GUI.hpp>

#include <map>

namespace flui {

    class Object;
    class NumberInputPeg;
    class NumberOutputPeg;
    class NumberWire;
    class SoundInputPeg;
    class SoundOutputPeg;
    class SoundWire;
	
    // TODO: ctrl-A to select everything

	// Panel is what holds all objects
	class Panel : public ui::Boxed<ui::FreeContainer>, public ui::Control {
    public:
		Panel();
        ~Panel();

		void addObject(std::unique_ptr<Object>);
        void removeObject(const Object*);
        
        // Adds a NumberWire to the panel.
        // At most one of src and dst may be null.
        NumberWire* addWire(flo::NumberSource* src, flo::NumberInput* dst);
        void removeWire(NumberWire*);

        // Adds a SoundWire to the panel.
        // At most one of src and dst may be null.
        SoundWire* addWire(flo::SoundSource* src, flo::SoundInput* dst);
        void removeWire(SoundWire*);

		/*void releaseObject(ui::Ref<Object> object);

		void addSoundWire(ui::Ref<SoundWire> wire);

		void removeSoundWire(SoundWire& wire);

		void addNumberWire(ui::Ref<NumberWire> wire);

		void removeNumberWire(NumberWire& wire);*/

	private:

		std::vector<Object*> m_objects;
		std::vector<NumberWire*> m_numberwires;
		std::vector<SoundWire*> m_soundwires;

    private:

        NumberInputPeg* findPegFor(const flo::NumberInput*);
        NumberOutputPeg* findPegFor(const flo::NumberSource*);
        SoundInputPeg* findPegFor(const flo::SoundInput*);
        SoundOutputPeg* findPegFor(const flo::SoundSource*);

        template<typename Traits>
        friend class Wire;

		// calculate the bounding box of all contents
		// returns { top-left, bottom-right } as positions
		// in local space
		//std::pair<ui::vec2, ui::vec2> getContentBounds();

		// move all objects and wires by delta
		//void shiftContents(vec2 delta);

	private:

        void makeSelection(ui::vec2 topLeft, ui::vec2 size);

        class SelectedObjects : public ui::FreeContainer, public ui::Control, public ui::Draggable {
        public:
            SelectedObjects(Panel& parentPanel, const std::vector<Object*>&);

        private:
            bool onLeftClick(int) override;

            void onLeftRelease() override;

            bool onKeyDown(ui::Key) override;

            void onLoseFocus() override;

            void onDrag() override;

            void render(sf::RenderWindow&) override;

            class FrontCover : public ui::Control, public ui::BoxElement {
            public:
                FrontCover(SelectedObjects& parent);

            private:
                bool onLeftClick(int) override;

                SelectedObjects& m_parent;
            };

            Panel& m_parentPanel;

            // vector of (selected object, base position) pairs
            std::vector<std::pair<Object*, ui::vec2>> m_objects;
            ui::vec2 m_lastPos;
        };

		bool onLeftClick(int) override;

        void onLeftRelease() override;

        void render(sf::RenderWindow&) override;

        std::optional<ui::vec2> m_selectionStart;
        ui::Draggable* m_selectionDragger;
	};

    // TODO: collapsible panel

	class MainPanel : public Panel {
    public:
		MainPanel();
	};

} // namespace flui