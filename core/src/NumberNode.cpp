#include <NumberNode.hpp>

#include <SoundNode.hpp>
#include <SoundResult.hpp>

#include <algorithm>
#include <cassert>
#include <functional>

namespace flo {

    NumberNode::NumberNode() noexcept
        : m_network(nullptr)
        , m_stateOwner(nullptr) {

    }

    bool NumberNode::canAddDependency(const NumberNode* node) const noexcept {
        if (hasDependency(node) || node->hasDependency(this)){
            return false;
        }

        // make sure every stateful dependent's state owner is a depencency of every stateful dependency's state owner.
        // That might sound confusing but trust me.
        for (const auto& dependency : getAllDependencies()){
            const auto dependencyStateOwner = dependency->getStateOwner();
            if (!dependencyStateOwner){
                continue;
            }
            for (const auto& dependent : getAllDependents()){
                const auto dependentStateOwner = dependent->getStateOwner();
                if (!dependentStateOwner){
                    continue;
                }
                if (!dependencyStateOwner->hasDependency(dependentStateOwner)){
                    return false;
                }
            }
        }

        return true;
    }

    void NumberNode::addDependency(NumberNode* node) {
        assert(std::count(m_dependencies.begin(), m_dependencies.end(), node) == 0);
        assert(std::count(node->m_dependents.begin(), node->m_dependents.end(), this) == 0);
        if (!canAddDependency(node)){
            throw std::runtime_error("Don't do that.");
        }
        m_dependencies.push_back(node);
        node->m_dependents.push_back(this);
    }

    void NumberNode::removeDependency(NumberNode* node){
        assert(std::count(m_dependencies.begin(), m_dependencies.end(), node) == 1);
        assert(std::count(node->m_dependents.begin(), node->m_dependents.end(), this) == 1);

        m_dependencies.erase(
            std::remove(m_dependencies.begin(), m_dependencies.end(), node),
            m_dependencies.end()
        );
        node->m_dependents.erase(
            std::remove(node->m_dependents.begin(), node->m_dependents.end(), node),
            node->m_dependents.end()
        );
    }

    const std::vector<NumberNode*>& NumberNode::getDirectDependencies() const noexcept {
        return m_dependencies;
    }

    const std::vector<NumberNode*>& NumberNode::getDirectDependents() const noexcept {
        return m_dependents;
    }

    std::set<const NumberNode*> NumberNode::getAllDependencies() const noexcept {
        std::set<const NumberNode*> nodes;
        std::function<void(const NumberNode*)> search = [&](const NumberNode* n){
            if (!n){
                return;
            }
            nodes.insert(n);
            for (const auto& d : n->getDirectDependencies()){
                search(d);
            }
        };
        search(this);
        return nodes;
    }

    std::set<const NumberNode*> NumberNode::getAllDependents() const noexcept {
        std::set<const NumberNode*> nodes;
        std::function<void(const NumberNode*)> search = [&](const NumberNode* n){
            if (!n){
                return;
            }
            nodes.insert(n);
            for (const auto& d : n->getDirectDependents()){
                search(d);
            }
        };
        search(this);
        return nodes;
    }

    bool NumberNode::hasDependency(const NumberNode* node) const noexcept {
        if (this == node){
            return true;
        }
        for (const auto& d : m_dependencies){
            if (d->hasDependency(node)){
                return true;
            }
        }
        return false;
    }

    SoundNode* NumberNode::getStateOwner() noexcept {
        return m_stateOwner;
    }

    const SoundNode* NumberNode::getStateOwner() const noexcept {
        return m_stateOwner;
    }

    void NumberNode::setStateOwner(SoundNode* sn){
        if (m_stateOwner){
            m_stateOwner->removeNumberNode(this);
        }
        m_stateOwner = sn;
        if (m_stateOwner){
            m_stateOwner->addNumberNode(this);
        }
    }

} // namespace flo
