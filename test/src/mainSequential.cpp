#include <fstream>
#include <vector>
#include <iostream>
#include <cmath>
#include <filesystem>
#include <algorithm>
#include <unordered_map>
#include "DataReader.h"
#include "SequentialClustering.h"
#include "DataWriter.h"

/*
 * Description.
 *
 * @author DeB
 * @version 1.0 2022-07-09
 * @since version date
 */
int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cerr << "Too few arguments" << std::endl;
        return 1;
    }
    std::string fileName{argv[1]};
    unsigned long startColumnIndex = -1;
    unsigned long endColumnIndex = -1;
    
    if (fileName == "pb") {
        fileName = "Parking Birmingham.data";
        startColumnIndex = 2;
        endColumnIndex = 3;
    } else if (fileName == "slide") {
        fileName = "slide.data";
        startColumnIndex = 1;
        endColumnIndex = 2;
    } else if (fileName == "acc") {
        fileName = "accelerometer.csv";
        startColumnIndex = 3;
        endColumnIndex = 4;
    } else {
        if (argc < 4) {
            std::cerr << "Too few arguments" << std::endl;
            return 1;
        }
        char *nextNonParsedIndex = &(argv[2][0]);
        startColumnIndex = std::strtoul(argv[2], &nextNonParsedIndex, 10);
        if (nextNonParsedIndex == &(argv[2][0])) {
            std::cerr << "Wrong start column index" << std::endl;
            return 1;
        }
        nextNonParsedIndex = &(argv[3][0]);
        endColumnIndex = std::strtoul(argv[3], &nextNonParsedIndex, 10);
        if (nextNonParsedIndex == &(argv[3][0])) {
            std::cerr << "Wrong end column index" << std::endl;
            return 1;
        }

        if (endColumnIndex < startColumnIndex) {
            std::cerr << "Wrong start and end column index" << std::endl;
            return 1;
        }
    }

    const unsigned long dimension = endColumnIndex - startColumnIndex + 1;

    std::vector<double *> data =
            DataReader::readData(startColumnIndex - 1, endColumnIndex - 1, fileName);

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::size_t> pi{};
    std::vector<double> lambda{};

    SequentialClustering::cluster(data, dimension, pi, lambda);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = end - start;

    using namespace std::literals::chrono_literals;

    auto nanoseconds = duration % 1us;
    auto microseconds = (duration % 1ms) / 1us;
    auto milliseconds = (duration % 1s) / 1ms;
    auto seconds = (duration % 1min) / 1s;
    auto minute = duration / 1min;

    std::cout << minute << " minutes " << seconds << " seconds " << milliseconds << " ms ("
              << milliseconds << "." << microseconds << "." << nanoseconds.count() << " ns)"
              << std::endl;

    std::filesystem::path outputDirectory{".."};
    outputDirectory = outputDirectory / ".." / "visualizer" / "DeB_Cluster_Visualizer" / "out";

    DataWriter::createOutputFile(outputDirectory / "out.txt", data, dimension, pi, lambda);
    DataWriter::createMathematicaOutputFile(outputDirectory / "mat.txt", pi, lambda);

    return 0;
}
