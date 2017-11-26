#pragma once

#include "musical.h"
#include "FlosionUI.h"
#include "Font.h"

namespace fui {

	struct ConstantObject : ProcessingObject {
		ConstantObject(float value = 1.0f){
			size = {100, 30};
			addChildWindow(new ui::Text(std::to_string(value), getFont()));
			addChildWindow(new NumberOutput(&constant, this, "Value"), rightOf(this));
			constant.setValue(value);
		}

		private:
		musical::Constant constant;
	};
	fuiRegisterObject(ConstantObject, "constant", "value", "number");

	// TODO: add menu similar to spline menu
	struct SliderObject : ProcessingObject {
		SliderObject(float minval, float maxval) : SliderObject() {
			setRange(minval, maxval);
		}
		SliderObject(){
			size = {300, 30};
			addChildWindow(slider = new Slider(this));
			addChildWindow(caption = new ui::Text("", fui::getFont()));

			NumberOutput* output = new NumberOutput(&value, this, "Value", [this](NumberInput* ni){
				const musical::NumberInput* input = ni->getInput();
				if (input->hasRange()){
					setRange(input->getMinimum(), input->getMaximum());
				}
			});

			addChildWindow(output, rightOf(this));

			updateCaption();
		}

		void onLeftClick(int clicks) override {
			slider->jumpToCursor();
		}
		void onRightClick(int clicks) override {
			startDrag();
		}
		
		void setRange(float minimum, float maximum){
			min_value = minimum;
			max_value = maximum;
			value.setRange(minimum, maximum);
			value.setValue(std::min(std::max(value.getValue(), min_value), max_value));
			updateCaption();
		}

		private:

		musical::Constant value;

		float min_value = 0.0f;
		float max_value = 10.0f;

		void updateCaption(){
			caption->setText(std::to_string(value.getValue()));
		}

		struct Slider : ui::Window {
			Slider(SliderObject* _parent){
				parent = _parent;
				size = {30, 30};
			}
			void jumpToCursor(){
				pos.x = parent->localMousePos().x - (size.x / 2);
				if (pos.x < 0){
					pos.x = 0;
				}
				if (pos.x > parent->size.x - size.x){
					pos.x = parent->size.x - size.x;
				}
				double val = parent->min_value + (parent->max_value - parent->min_value) * (pos.x / (parent->size.x - size.x));
				parent->value.setValue(val);
				parent->updateCaption();
				startDrag();
			}
			void onLeftClick(int clicks) override {
				startDrag();
			}
			void onRightClick(int clicks) override {
				parent->startDrag();
			}
			void onDrag() override {
				pos.y = 0;
				if (pos.x < 0){
					pos.x = 0;
				}
				if (pos.x > parent->size.x - size.x){
					pos.x = parent->size.x - size.x;
				}

				double val = parent->min_value + (parent->max_value - parent->min_value) * (pos.x / (parent->size.x - size.x));

				parent->value.setValue(val);
				parent->updateCaption();
			}
			SliderObject* parent;
		} *slider;
		ui::Text* caption;
	};
	fuiRegisterObject(SliderObject, "slider", "variable");
};