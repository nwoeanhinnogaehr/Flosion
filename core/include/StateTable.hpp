#pragma once

#include <SoundState.hpp>
#include <StateAllocator.hpp>

#include <memory>
#include <vector>

namespace flo {

    class StateBorrower;
    class SoundNode;

    /*
     * StateTable is an array-like container of custom SoundState objects.
     * Additional states may be stored alongside the SoundStates to be used by
     * state borrowers such as stateful number sources.
     * Sound states are stored inside slots right beside their associated borrowed
     * states. Slots are stored contiguously.
     * StateTable is the basis for an associative mapping from dependent sound nodes,
     * dependent states, etc, to the states needed by a sound node.
     */

    // TODO: is size_t a waste of space? something like uint16 is probably fine for indices
    // Just beware of really big states

    // The gist of it:
    // - Every row is mapped to by a dependent state
    // - Every column is mapped to by a key (there is 1 key by default)
    // - There is one slot for every pair of row and column
    // - There is one main state in each slot
    // - There is one borrowed state in each slot for each borrower
    // - All slots are stored contiguously in row-major order
    class StateTable {
    public:
        StateTable(SoundNode* owner);
        StateTable(StateTable&&) noexcept;
        StateTable& operator=(StateTable&&) noexcept;
        ~StateTable();

        StateTable(const StateTable&) = delete;
        StateTable& operator=(const StateTable&) = delete;

        virtual std::unique_ptr<StateAllocator> makeAllocator() const = 0;

        SoundState* getState(const SoundNode* dependent, const SoundState* dependentState, size_t keyIndex) noexcept;
        const SoundState* getState(const SoundNode* dependent, const SoundState* dependentState, size_t keyIndex) const noexcept;

        SoundState* getState(size_t index) noexcept;
        const SoundState* getState(size_t index) const noexcept;

        State* getBorrowedState(const SoundState* mainState, const StateBorrower* borrower) noexcept;
        const State* getBorrowedState(const SoundState* mainState, const StateBorrower* borrower) const noexcept;

        void resetStateFor(const SoundNode* dependent, const SoundState* dependentState) noexcept;

        size_t getStateIndex(const SoundState* ownState) const noexcept;

        bool hasState(const SoundState* ownState) const noexcept;

        // the number of slots in the table. Equal to numDependentStates() * numKeys().
        size_t numSlots() const noexcept;

        // the number of rows in the table
        size_t numDependentStates() const noexcept;

        // the number of columns in the table
        size_t numKeys() const noexcept;

        // the number of borrowed states per slot
        size_t numBorrowers() const noexcept;

        // the total size of a slot, in bytes
        size_t slotSize() const noexcept;

        void insertDependentStates(const SoundNode* dependent, size_t beginIndex, size_t endIndex);
        void eraseDependentStates(const SoundNode* dependent, size_t beginIndex, size_t endIndex);
        
        void insertKeys(size_t beginIndex, size_t endIndex);
        void eraseKeys(size_t beginIndex, size_t endIndex);

        void addBorrower(StateBorrower*);
        void removeBorrower(StateBorrower*);

    private:
        SoundNode* m_owner;

        // the allocator for states owned by the state table's owner
        // This will be null until the first state is allocated
        // Use getStateAllocator() instead of this member.
        std::unique_ptr<StateAllocator> m_mainAllocator;

        // Per slot item data
        struct SlotItem {

            // The allocator used to manage the lifetime of the slot item
            std::unique_ptr<StateAllocator> allocator;

            // The offset (in bytes) from the slot's begin to the item
            size_t offset;
        };

        std::vector<SlotItem> m_slotItems;

        struct DependentOffset {
            // the node whose states are parents to the states in this array
            const SoundNode* dependent;

            // the index at which the dependent's states begin
            size_t offset;
        };
        
        std::vector<DependentOffset> m_dependentOffsets;

        // number of dependent states (rows)
        size_t m_numDependentStates;
        
        // number of keys (columns)
        size_t m_numKeys;

        // size in bytes per slot
        size_t m_slotSize;

        // the array
        unsigned char* m_data;

        // See also: https://en.cppreference.com/w/cpp/memory/align
        // for computing successive addresses

        size_t getDependentOffset(const SoundNode* dependent) const noexcept;

        size_t nextAlignedOffset(const unsigned char* addr, size_t align) const;

        StateAllocator* getMainAllocator();

        unsigned char* allocateData(size_t slotSize, size_t numSlots);
        void deallocateData(unsigned char*);

        // constructs a slot in place from uninitialized storage
        void constructSlot(unsigned char* where, const flo::SoundState* dependentState);

        // destroys a slot in place
        void destroySlot(unsigned char* where);

        // moves a slot from one location to another
        // the old slot is destroyed
        void moveSlot(unsigned char* from, unsigned char* to, const SoundState* newDependentState);

        void resetSlot(unsigned char* where);

        // moves a slot from one location to another while adding a slot item
        // the slot item must be present in m_slotItems
        // the old slot is destroyed
        void moveSlotAndAddItem(unsigned char* from, unsigned char* to, const StateAllocator* whichItem);
        
        // moves a slot from one location to another while removing a slot item
        // the old slot is destroyed
        void moveSlotAndRemoveItem(unsigned char* from, unsigned char* to, const StateAllocator* whichItem);

        void repointStatesFor(const SoundNode* dependent) noexcept;

        friend class SoundNode;
    };

} // namespace flo
