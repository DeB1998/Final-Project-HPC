#ifndef FINAL_PROJECT_HPC_ONLYITERABLEVECTOR_H
#define FINAL_PROJECT_HPC_ONLYITERABLEVECTOR_H

#include <vector>

namespace cluster::test::types::collections {

/**
 * Description.
 *
 * @author DeB
 * @version 1.0 2022-08-12
 * @since version date
 */
template <typename T>
class OnlyIterableVector {

public:
    using value_type = T;
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    template <class... Args>
    T& emplace_back(Args&&... args) {
        return this->data.template emplace_back(std::forward<Args...>(args...));
    }

    void push_back(const T& value) {
        this->data.push_back(value);
    }

    void resize(std::size_t newSize) {
        this->data.resize(newSize);
    }

    iterator begin() {
        return this->data.begin();
    }

    iterator end() {
        return this->data.end();
    }

    const_iterator begin() const {
        return this->data.cbegin();
    }

    const_iterator end() const {
        return this->data.cend();
    }

private:
    std::vector<T> data;
};

}  // namespace cluster::test::types::collections

#endif  // FINAL_PROJECT_HPC_ONLYITERABLEVECTOR_H
