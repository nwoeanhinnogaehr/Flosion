#pragma once

#include "NumberResult.h"

namespace musical {

	NumberResult::NumberResult(Stateful* owner) : input(nullptr, owner) {

	}
	float NumberResult::getValue(State* state, float default_value){
		return input.getValue(state, default_value);
	}
	bool NumberResult::isSafeSource(NumberSource* source) const {
		return input.isSafeSource(source);
	}
	void NumberResult::setSource(NumberSource* source){
		input.setSource(source);
	}

}