#pragma once

#include <SFML\Graphics.hpp>
#include <map>
#include <functional>

extern const double PI;

typedef sf::Vector2f vec2;

namespace ui {

	struct Window;
	struct Text;
	struct TextEntry;

	namespace {
		
		struct Transition {
			Transition(Window* _target, double _duration, const std::function<void(double)>& _transitionFn, const std::function<void()>& _onComplete = {});

			void apply();

			bool complete() const;

			const Window* target;

			private:
			std::function<void(double)> transitionFn;
			std::function<void()> onComplete;
			bool completed;
			double duration;
			long double timestamp;
		};

		struct Context {
			static void init(unsigned width, unsigned height, std::string title, double _render_delay);

			static void addTransition(const Transition& transition);
			static void applyTransitions();
			static void clearTransitions(Window* target);

			static void addKeyboardCommand(sf::Keyboard::Key, void (*)());
			static void addKeyboardCommand(sf::Keyboard::Key, std::vector<sf::Keyboard::Key>, void (*)());
			static void setQuitHandler(bool (*handler)());

			static void handleKeyPress(sf::Keyboard::Key key);
			static void handleMouseDown(sf::Mouse::Button button, vec2 pos);
			static void handleMouseUp(sf::Mouse::Button button, vec2 pos);
			static void handleDrag();
			static void handleHover();
			static void handleQuit(bool force);

			static bool hasQuit();
			static long double getProgramTime();
			static vec2 getMousePosition();
			static sf::RenderWindow& getRenderWindow();
			static double getRenderDelay();
			static void translateView(vec2 offset);
			static vec2 getViewOffset();
			static void resetView();
			static void updateView();
			static const sf::FloatRect& getClipRect();
			static void setClipRect(const sf::FloatRect& rect);
			static void intersectClipRect(const sf::FloatRect& rect);
			static void resize(int w, int h);

			static Window* getDraggingWindow();
			static void setDraggingWindow(Window* window, vec2 offset = vec2(0, 0));
			static void focusTo(Window* window);
			static Window* getCurrentWindow();
			static TextEntry* getTextEntry();
			static void setTextEntry(TextEntry* textentry);

			private:


			static bool quit;
			static double render_delay;

			static vec2 drag_offset;

			static sf::RenderWindow renderwindow;

			static Window* dragging_window;
			static Window* current_window;
			static TextEntry* text_entry;

			static std::vector<Transition> transitions;

			static sf::Clock clock;

			static std::map<std::pair<sf::Keyboard::Key, std::vector<sf::Keyboard::Key>>, void (*)()> commands;
			static bool (*quit_handler)(); // TODO: use std::function

			static const int doubleclicktime;

			static int32_t click_timestamp;
			static sf::Mouse::Button click_button;
			static Window* click_window;
			static sf::FloatRect clip_rect;
			static vec2 view_offset;
			static int width;
			static int height;
		};
	}

	// TODO: automatic sizing based on child windows
	struct Window {
		bool disabled = false;
		bool visible = true;
		bool clipping = false;
		bool bring_to_front = false;
		vec2 pos;
		vec2 size;

		virtual ~Window();

		void close();

		virtual bool hit(vec2 testpos);

		vec2 localMousePos();
		vec2 absPos();

		virtual void onLeftClick(int clicks);
		virtual void onLeftRelease();
		virtual void onRightClick(int clicks);
		virtual void onRightRelease();

		virtual void onScroll(double delta_x, double delta_y);

		void startDrag();
		virtual void onDrag();

		virtual void onHover();
		virtual void onHoverWithWindow(Window *drag_window);

		// shall return false if the parent's method is to be invoked
		virtual bool onDropWindow(Window *window);

		virtual void onFocus();
		bool isFocused();
		virtual void onLoseFocus();
		void grabFocus();
		void focusToNextWindow();
		void focusToPreviousWindow();

		virtual void onKeyDown(sf::Keyboard::Key key);
		virtual void onKeyUp(sf::Keyboard::Key key);
		bool keyDown(sf::Keyboard::Key key);

		struct Alignment {
			enum Type {
				None,
				After,
				Before,
				Center,
				InsideMin,
				InsideMax
			};

			protected:
			float margin;

			Alignment(Type _type, Window* _relative_to = nullptr, float _margin = 0.0f);

			Type type;
			Window* relative_to;

			friend struct Window;
		};

		struct XAlignment : Alignment {
			private:

			XAlignment(Type type, Window* relative_to = nullptr, float margin = 0.0f);

			friend struct Window;
		};
		struct YAlignment : Alignment {
			private:

			YAlignment(Type type, Window* relative_to = nullptr, float margin = 0.0f);

			friend struct Window;
		};

		void setXAlign(XAlignment xalignment = XAlignment(Alignment::None, nullptr));
		void setYAlign(YAlignment yalignment = YAlignment(Alignment::None, nullptr));
		void align();
		void alignChildren();

		static XAlignment noAlignX();
		static XAlignment leftOf(Window* window, float margin = 0.0f);
		static XAlignment rightOf(Window* window, float margin = 0.0f);
		static XAlignment middleOfX(Window* window);
		static XAlignment insideLeft(Window* window, float margin = 0.0f);
		static XAlignment insideRight(Window* window, float margin = 0.0f);
		static YAlignment noAlignY();
		static YAlignment above(Window* window, float margin = 0.0f);
		static YAlignment below(Window* window, float margin = 0.0f);
		static YAlignment insideTop(Window* window, float margin = 0.0f);
		static YAlignment insideBottom(Window* window, float margin = 0.0f);
		static YAlignment middleOfY(Window* window);

		void addChildWindow(Window* window);
		void addChildWindow(Window* window, vec2 pos);
		void addChildWindow(Window* window, XAlignment xalignment);
		void addChildWindow(Window* window, YAlignment yalignment);
		void addChildWindow(Window* window, XAlignment xalignment, YAlignment yalignment);
		void addChildWindow(Window* window, float xpos, YAlignment yalignment);
		void addChildWindow(Window* window, XAlignment xalignment, float ypos);
		void releaseChildWindow(Window* window);
		void bringToFront();
		void clear();

		Window* findWindowAt(vec2 _pos);

		virtual void render(sf::RenderWindow& renderwindow);
		void renderChildWindows(sf::RenderWindow& renderwindow);

		void startTransition(double duration, const std::function<void(double)> transitionFn, const std::function<void()>& onComplete = {});

		const std::vector<Window*>& getChildWindows() const;
		Window* getParent() const;

		private:
		Window* parent = nullptr;
		std::vector<Window*> childwindows;

		XAlignment xalign = XAlignment(Alignment::None, nullptr);
		YAlignment yalign = YAlignment(Alignment::None, nullptr);
		bool children_aligned = false;

		friend struct Context;
	};

	struct Text : Window {
		Text(const std::string& _text, const sf::Font& _font, sf::Color color = sf::Color(0xFF), int charsize = 15);

		void setText(const std::string& _text);
		std::string getText();

		void render(sf::RenderWindow& renderwin) override;

		private:

		void updateSize();

		sf::Text text;
	};

	// TODO: this should derive from Text
	// TODO: pressing escape shouldn't type a blank character
	struct TextEntry : Window {
		TextEntry(const sf::Font& font, int charsize = 15);
		TextEntry(const std::string& str, const sf::Font& font, int charsize = 15, sf::Color _text_color = sf::Color(0xFF), sf::Color _bg_color = sf::Color(0xFFFFFFFF));

		void beginTyping();
		void moveTo(vec2 pos);

		void setText(const std::string& str);
		std::string getText() const;
		void clearText();

		void setTextColor(sf::Color color);
		sf::Color getTextColor() const;

		void setBackGroundColor(sf::Color color);
		sf::Color getBackGroundColor() const;

		void setCharacterSize(unsigned int size);
		unsigned int getCharacterSize() const;

		//to be overridden and used to deal with submission of newly typed text
		virtual void onReturn(const std::string& entered_text);
		virtual void onType(const std::string& full_text);

		void render(sf::RenderWindow& renderwindow) override;

		void onLeftClick(int clicks) override;
		void onFocus() override;

		private:
		void write(char ch);
		void onBackspace();
		void onDelete();
		void onLeft();
		void onRight();
		void onHome();
		void onEnd();

		void updateSize();

		sf::Color background_color;
		sf::Text text;
		unsigned cursor_index = 0;
		float cursor_pos;
		float cursor_width;

		friend void run();
	};

	Window* root();

	void addKeyboardCommand(sf::Keyboard::Key trigger_key, void (*handler)());
	void addKeyboardCommand(sf::Keyboard::Key trigger_key, std::vector<sf::Keyboard::Key> required_keys, void (*handler)());
	void setQuitHandler(bool (*handler)());

	long double getProgramTime();

	vec2 getScreenSize();

	vec2 getMousePos();

	void init(unsigned width = 500, unsigned height = 400, std::string title = "Behold", int target_fps = 30);

	void quit(bool force = false);

	void run();
}