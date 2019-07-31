#include <Sample.hpp>

namespace flo {

    // Sample

    Sample::Sample() noexcept
        : m_data{} {

    }

    Sample::Sample(float left, float right) noexcept {
        l() = left;
        r() = right;
    }

    Sample::Sample(const ConstSampleProxy& other) noexcept {
        l() = other.l();
        r() = other.r();
    }

    float& Sample::l() noexcept {
        return m_data[0];
    }

    const float& Sample::l() const noexcept {
        return m_data[0];
    }

    float& Sample::r() noexcept {
        return m_data[1];
    }

    const float& Sample::r() const noexcept {
        return m_data[1];
    }

    Sample& Sample::operator=(const Sample& other) noexcept {
        l() = other.l();
        r() = other.r();
        return *this;
    }

    Sample& Sample::operator=(const ConstSampleProxy& other) noexcept {
        l() = other.l();
        r() = other.r();
        return *this;
    }

    Sample& Sample::operator+=(const Sample& other) noexcept {
        l() += other.l();
        r() += other.r();
        return *this;
    }

    Sample& Sample::operator+=(const ConstSampleProxy& other) noexcept {
        l() += other.l();
        r() += other.r();
        return *this;
    }

    Sample& Sample::operator-=(const Sample& other) noexcept {
        l() -= other.l();
        r() -= other.r();
        return *this;
    }

    Sample& Sample::operator-=(const SampleProxy& other) noexcept {
        l() -= other.l();
        r() -= other.r();
        return *this;
    }

    Sample& Sample::operator*=(float value) noexcept {
        l() *= value;
        r() *= value;
        return *this;
    }

    Sample Sample::operator-() const noexcept {
        return {-l(), -r()};
    }

    Sample Sample::operator+(const Sample& other) const noexcept {
        return {l() + other.l(), r() + other.r()};
    }

    Sample Sample::operator+(const ConstSampleProxy& other) const noexcept {
        return {l() + other.l(), r() + other.r()};
    }

    Sample Sample::operator-(const Sample& other) const noexcept {
        return {l() - other.l(), r() - other.r()};
    }

    Sample Sample::operator-(const ConstSampleProxy& other) const noexcept {
        return {l() - other.l(), r() - other.r()};
    }

    Sample Sample::operator*(float value) const noexcept {
        return {l() - value, r() - value};
    }

    Sample::operator SampleProxy() & noexcept {
        return SampleProxy(static_cast<float*>(m_data));
    }

    Sample::operator ConstSampleProxy() const & noexcept {
        return ConstSampleProxy(static_cast<const float*>(m_data));
    }

    // SampleProxy

    SampleProxy::SampleProxy(float* ptr) noexcept
        : m_ptr(ptr) {
        
    }

    float& SampleProxy::l() noexcept {
        return m_ptr[0];
    }

    const float& SampleProxy::l() const noexcept {
        return m_ptr[0];
    }

    float& SampleProxy::r() noexcept {
        return m_ptr[1];
    }

    const float& SampleProxy::r() const noexcept {
        return m_ptr[1];
    }

    // ConstSampleProxy

    ConstSampleProxy::ConstSampleProxy(const float* ptr) noexcept
        : m_ptr(ptr) {
    
    }

    const float& ConstSampleProxy::l() const noexcept {
        return m_ptr[0];
    }

    const float& ConstSampleProxy::r() const noexcept {
        return m_ptr[1];
    }

} // namespace flo
