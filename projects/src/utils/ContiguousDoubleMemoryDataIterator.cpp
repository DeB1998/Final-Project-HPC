/*
 * Description.
 *
 * @author DeB
 * @version 1.0 2022-08-01
 * @since version date
 */
#include "../../include/utils/ContiguousDoubleMemoryDataIterator.h"
#include "SimdUtils.h"

ContiguousDoubleMemoryDataIterator::ContiguousDoubleMemoryDataIterator() :
    startElement(nullptr),
    currentElement{nullptr},
    stride(0) {
}

ContiguousDoubleMemoryDataIterator::ContiguousDoubleMemoryDataIterator(const double* startElement,
                                                                       size_t stride) :
    startElement(startElement),
    currentElement{startElement},
    stride(stride) {
}

ContiguousDoubleMemoryDataIterator& ContiguousDoubleMemoryDataIterator::operator= (
        const ContiguousDoubleMemoryDataIterator& other) {

    this->startElement = other.startElement;
    this->currentElement = other.currentElement;
    this->stride = other.stride;

    return *this;
}

ContiguousDoubleMemoryDataIterator& ContiguousDoubleMemoryDataIterator::operator= (
        ContiguousDoubleMemoryDataIterator&& other) noexcept {
    if (&other != this) {
        this->startElement = other.startElement;
        this->currentElement = other.currentElement;
        this->stride = other.stride;
    }

    return *this;
}

ContiguousDoubleMemoryDataIterator::value_type& ContiguousDoubleMemoryDataIterator::operator* ()
        const {
    return currentElement;
}

ContiguousDoubleMemoryDataIterator::pointer ContiguousDoubleMemoryDataIterator::operator->() const {
    return &currentElement;
}

const double* ContiguousDoubleMemoryDataIterator::operator[] (
        ContiguousDoubleMemoryDataIterator::difference_type index) const {
    return startElement + (stride * index);
}

bool ContiguousDoubleMemoryDataIterator::operator== (
        const ContiguousDoubleMemoryDataIterator& other) const {
    return currentElement == other.currentElement;
}

bool ContiguousDoubleMemoryDataIterator::operator!= (
        const ContiguousDoubleMemoryDataIterator& other) const {
    return currentElement != other.currentElement;
}

ContiguousDoubleMemoryDataIterator& ContiguousDoubleMemoryDataIterator::operator++ () {

    currentElement += stride;
    return *this;
}

ContiguousDoubleMemoryDataIterator ContiguousDoubleMemoryDataIterator::operator++ (int) {

    ContiguousDoubleMemoryDataIterator copy = *this;
    currentElement += stride;
    return copy;
}

std::size_t ContiguousDoubleMemoryDataIterator::computeSseStride(std::size_t dimension) {
    
    return SimdUtils::SSE_PACK_SIZE * SimdUtils::computeSseBlocksCount(dimension);
}

std::size_t ContiguousDoubleMemoryDataIterator::computeAvxStride(std::size_t dimension) {
    
    return SimdUtils::AVX_PACK_SIZE * SimdUtils::computeAvxBlocksCount(dimension);
}