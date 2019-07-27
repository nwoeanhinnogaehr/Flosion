#pragma once

#include <Immovable.hpp>
#include <SoundChunk.hpp>
#include <SoundNode.hpp>

#include <vector>

namespace flo {

    class SoundSource : public SoundNode {
    public:

        /**
         * Produces the next chunk of sound. The SoundSource's state corresponding
         * to the given context may be updated.
         */
        virtual void getNextChunkFor(SoundChunk& chunk, const SoundState* context) = 0;


        //class Decorator : public SoundNode::Decorator {
            // TODO: ???
        //};
    };

} // namespace flo
