#pragma once

#include <SFML\Audio.hpp>
#include "SoundResult.h"
#include "musical.h"
#include <thread>

namespace musical {

	struct DAC : private sf::SoundStream {
		DAC();
		~DAC();

		void play();
		void pause();
		void reset();

		bool isPlaying() const;

		// returns the average amplitude of the current chunk, intended for aesthetic use
		Sample getCurrentAmp() const;

		SoundResult input;

		private:

		SoundChunk inbuffer;
		std::array<int16_t, CHUNK_SIZE * 2> outbuffer;

		// sf::SoundStream overrides

		bool onGetData(Chunk& data) override;

		void onSeek(sf::Time timeOffset) override;
	};
}