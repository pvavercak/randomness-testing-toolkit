#include "rtt/batteries/dieharder/battery-dh.h"

namespace rtt {
namespace batteries {
namespace dieharder {

std::unique_ptr<Battery> Battery::getInstance(const CliOptions & options) {
    std::unique_ptr<Battery> battery (new Battery());

    TiXmlNode * cfgRoot = NULL;
    loadXMLFile(cfgRoot , options.getInputCfgPath());

    for(int i : options.getTestConsts()) {
        Test test = Test::getInstance(static_cast<TestIndex>(i) ,
                                      cfgRoot , options.getBinFilePath());
        battery->tests.push_back(std::move(test));
    }

    delete cfgRoot;
    return battery;
}

void Battery::runTests() {
    for(auto & i : tests)
        i.execute();

    executed = true;
}

void Battery::processStoredResults() {
    if(!executed)
        throw std::runtime_error("can't process results before execution of battery");

    std::cout << "Momentarily, this does nothing!" << std::endl;
}

} // namespace dieharder
} // namespace batteries
} // namespace rtt

