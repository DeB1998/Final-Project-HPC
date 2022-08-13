#ifndef FINAL_PROJECT_HPC_DATATYPESTESTER_H
#define FINAL_PROJECT_HPC_DATATYPESTESTER_H

#include "TypesPrinter.h"
#include "../ResultsChecker.h"
#include "ParallelClustering.h"
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <vector>

namespace cluster::test::types {

/**
 * Description.
 *
 * @author DeB
 * @version 1.0 2022-08-10
 * @since version date
 */
class DataTypesTester {
    using DataIteratorType = cluster::utils::DataIteratorType;
    using DataIteratorTypeUtils = cluster::utils::DataIteratorTypeUtils;
    using DataIteratorUtils = cluster::utils::DataIteratorUtils;

public:
    DataTypesTester(const size_t dataElementsCount,
                    const size_t dimension,
                    const std::vector<std::size_t> &expectedPi,
                    const std::vector<double> &expectedLambda,
                    const int maxTypeNameLength,
                    const int maxResultLength) :
        dataElementsCount(dataElementsCount),
        dimension(dimension),
        expectedPi(expectedPi),
        expectedLambda(expectedLambda),
        maxTypeNameLength(maxTypeNameLength),
        maxResultLength(maxResultLength) {
    }

    template <typename... CDs, typename... NCDs>
    void testParallelTypes(
            const std::tuple<std::pair<CDs &, DataIteratorType>...> &compilableDataStructures,
            const std::tuple<std::pair<NCDs &, DataIteratorType>...> &notCompilableDataStructures) {

        std::vector<std::size_t> pi{};
        pi.resize(this->dataElementsCount);
        std::vector<double> lambda{};
        lambda.resize(this->dataElementsCount);

        if constexpr (sizeof...(CDs) > 0) {
            testCompilableTypes<false, 0>(compilableDataStructures, pi, lambda);
        }
        if constexpr (sizeof...(NCDs) > 0) {
            testNotCompilableTypes<0>(notCompilableDataStructures);
        }
    }

private:
    template <bool S, std::size_t DI, typename... CDs>
    void testCompilableTypes(
            const std::tuple<std::pair<CDs &, DataIteratorType>...> &compilableDataStructures,
            std::vector<std::size_t> &pi,
            std::vector<double> &lambda) {

        const auto &dataPair = std::get<DI>(compilableDataStructures);
        const auto &data = dataPair.first;
        const auto iteratorType = dataPair.second;

        std::cout << std::setfill(' ') << std::setw(this->maxTypeNameLength)
                  << TypesPrinter::getTypeName<decltype(data)>();
        std::cout << " | ";
        std::cout << std::setfill(' ') << std::setw(this->maxResultLength) << ' ';
        std::cout << " | ";
        std::cout.flush();

        if constexpr (!utils::ParallelDataIterator<decltype(data)>) {
            std::cout << "\033[" << this->maxResultLength + 3 << "D"
                      << "\033[31m";
            std::cout << std::setfill(' ') << std::setw(this->maxResultLength)
                      << "Error (should compile)";
            std::cout << "\033[0m";
        } else {
            if constexpr (!S) {
                parallel::ParallelClustering<true, true, true>::cluster<
                        parallel::DistanceComputers::CLASSICAL>(
                        data, this->dataElementsCount, this->dimension, pi, lambda, 6, 6, 6);
            } else {
                // TODO: Sequential
            }
            bool result = ResultsChecker::checkResults(pi.cbegin(),
                                                       pi.cend(),
                                                       lambda.cbegin(),
                                                       lambda.cend(),
                                                       this->expectedPi.cbegin(),
                                                       this->expectedPi.cend(),
                                                       this->expectedLambda.cbegin(),
                                                       this->expectedLambda.cend());
            std::cout << "\033[" << this->maxResultLength + 3 << "D"
                      << ((result) ? "\033[32mOK" : "\033[31mError") << "\033[0m";
            if (DataIteratorUtils::lastIteratorType != iteratorType) {
                std::cout << " \033[31m(" << DataIteratorTypeUtils::getDescription(iteratorType)
                          << ")\033[0m";
            } else {
                std::cout << " \033[32m(OK)\033[0m";
            }
        }
        std::cout << std::endl;
        if constexpr (DI + 1 < sizeof...(CDs)) {
            testCompilableTypes<S, DI + 1>(compilableDataStructures, pi, lambda);
        }
    }

    template <std::size_t NDI, typename... NCDs>
    void testNotCompilableTypes(
            const std::tuple<std::pair<NCDs &, DataIteratorType>...> &notCompilableDataStructures) {

        const auto &dataPair = std::get<NDI>(notCompilableDataStructures);
        const auto &data = dataPair.first;

        std::cout << std::setfill(' ') << std::setw(this->maxTypeNameLength)
                  << TypesPrinter::getTypeName<decltype(data)>();
        std::cout << " | ";

        if constexpr (utils::ParallelDataIterator<decltype(data)>) {
            std::cout << "\033[31m";
            std::cout << std::setfill(' ') << std::setw(this->maxResultLength)
                      << "Error (should not compile)";
            std::cout << "\033[0m |";
        } else {
            std::cout << "\033[34m";
            std::cout << std::setfill(' ') << std::setw(this->maxResultLength) << "OK";
            std::cout << "\033[0m |";
        }
        std::cout << std::endl;
        if constexpr (NDI + 1 < sizeof...(NCDs)) {
            testNotCompilableTypes<NDI + 1>(notCompilableDataStructures);
        }
    }

    const std::size_t dataElementsCount;
    const std::size_t dimension;
    const std::vector<std::size_t> &expectedPi;
    const std::vector<double> &expectedLambda;
    const int maxTypeNameLength;
    const int maxResultLength;
};

}  // namespace cluster::test::types

#endif  // FINAL_PROJECT_HPC_DATATYPESTESTER_H
