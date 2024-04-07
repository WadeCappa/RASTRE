#include "representative_subset_calculator/representative_subset.h"
#include "data_tools/data_row_visitor.h"
#include "data_tools/dot_product_visitor.h"
#include "data_tools/data_row.h"
#include "data_tools/data_row_factory.h"
#include "data_tools/base_data.h"
#include "representative_subset_calculator/timers/timers.h"
#include "representative_subset_calculator/naive_representative_subset_calculator.h"
#include "representative_subset_calculator/lazy_representative_subset_calculator.h"
#include "representative_subset_calculator/fast_representative_subset_calculator.h"
#include "representative_subset_calculator/lazy_fast_representative_subset_calculator.h"
#include "representative_subset_calculator/orchestrator/orchestrator.h"

#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>

int main(int argc, char** argv) {
    CLI::App app{"Approximates the best possible approximation set for the input dataset."};
    AppData appData;
    Orchestrator::addCmdOptions(app, appData);
    CLI11_PARSE(app, argc, argv);

    Timers timers;
    timers.loadingDatasetTime.startTimer();
    std::ifstream inputFile;
    inputFile.open(appData.inputFile);
    std::unique_ptr<FullyLoadedData> data(Orchestrator::buildData(appData, inputFile));
    inputFile.close();
    timers.loadingDatasetTime.stopTimer();

    timers.totalCalculationTime.startTimer();
    std::unique_ptr<SubsetCalculator> calculator(Orchestrator::getCalculator(appData));
    std::unique_ptr<Subset> solution = calculator->getApproximationSet(*data.get(), appData.outputSetSize);
    timers.totalCalculationTime.stopTimer();

    nlohmann::json result = Orchestrator::buildOutput(appData, *solution.get(), *data.get(), timers);
    std::ofstream outputFile;
    outputFile.open(appData.outputFile);
    outputFile << result.dump(2);
    outputFile.close();

    return 0;
}