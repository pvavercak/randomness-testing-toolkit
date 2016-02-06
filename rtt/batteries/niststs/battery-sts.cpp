#include "rtt/batteries/niststs/battery-sts.h"

namespace rtt {
namespace batteries {
namespace niststs {

const std::string Battery::XPATH_LOG_DIRECTORY = "TOOLKIT_SETTINGS/LOGGER/NIST_STS_DIR";

std::unique_ptr<Battery> Battery::getInstance(const CliOptions & options) {
    std::unique_ptr<Battery> battery (new Battery());
    TiXmlNode * cfgRoot = NULL;
    loadXMLFile(cfgRoot , options.getInputCfgPath());

    std::string logFileName;
    logFileName = getXMLElementValue(cfgRoot , XPATH_LOG_DIRECTORY);
    if(logFileName.empty())
        throw std::runtime_error("empty tag: " + XPATH_LOG_DIRECTORY);

    std::string binFileName =
            Utils::getLastItemInPath(options.getBinFilePath());
    std::string datetime = Utils::getDateTime();
    if(logFileName.back() != '/')
        logFileName.append("/");
    logFileName.append(datetime + "-" + binFileName + ".log");
    battery->logFileName = std::move(logFileName);

    battery->storage = output::InterfaceFactory::createOutput(cfgRoot ,
                                                              options ,
                                                              datetime);

    for(int i : options.getTestConsts()) {
        Test test = Test::getInstance(static_cast<TestIndex>(i) ,
                                      cfgRoot , options.getBinFilePath());
        battery->tests.push_back(std::move(test));
    }

    delete cfgRoot;
    return battery;
}

void Battery::runTests() {
    /* Executing all tests in sequence here */
    /* In time, it's possible to add some multithreading */
    for(auto & i : tests)
        i.execute();

    /* Setting executed to true, allowing postprocessing */
    executed = true;
}

void Battery::processStoredResults() {
    if(!executed)
        throw std::runtime_error("can't process results before execution of battery");

    std::cout << "Storing log and results." << std::endl;

    /* Log storage */
    std::string batteryLog;
    for(auto & i : tests)
        i.appendTestLog(batteryLog);

    Utils::createDirectory(Utils::getPathWithoutLastItem(logFileName));
    Utils::saveStringToFile(logFileName , batteryLog);

    /* Result storage */
    for(const Test & test : tests) {
        storage->addNewTest(test.getLogicName());
        storage->setTestOptions(test.getSettings());

        std::vector<tTestPvals> results = test.getResults();

        if(results.size() == 1) { /* Single test */
            storage->addStatisticResult("Chi-square" , chi2_stat(results.at(0)));
            storage->addStatisticResult("Proportion" , proportionStat(results.at(0)));
            storage->addPValues(results.at(0));
        } else { /* Multiple subtests */
            for(const auto & result : results) {
                storage->addSubTest();
                storage->addStatisticResult("Chi-square" , chi2_stat(result));
                storage->addStatisticResult("Proportion" , proportionStat(result));
                storage->addPValues(result);
                storage->finalizeSubTest();
            }
        }
        storage->finalizeTest();
    }
    storage->finalizeReport();
}

std::string Battery::proportionStat(tTestPvals pvals) {
    int passCount = 0;
    double ALPHA = 0.01;
    for(const auto & val : pvals) {
        if(val >= ALPHA)
            ++passCount;
    }
    return {Utils::itostr(passCount) + "/" + Utils::itostr(pvals.size())};
}

/* Following code is taken from NIST STS source code */
/* Used for calculation of Chi2 statistic */
double Battery::chi2_stat(tTestPvals pvals) {
    int streamCount = pvals.size();
    int j, pos , expCount;
    double chi2 , uniformity;

    std::vector<int> freqPerBin(10 , 0);

    std::sort(pvals.begin() , pvals.end());
    for(j = 0 ; j <  streamCount ; ++j) {
        pos = (int)floor(pvals.at(j)*10);
        if(pos == 10)
            pos--;
        ++freqPerBin[pos];
    }
    chi2 = 0.0;
    expCount =  streamCount/10;
    if(expCount == 0) {
        uniformity = 0.0;
    } else {
        for (j = 0 ; j < 10 ; ++j)
            chi2 += pow(freqPerBin[j]-expCount, 2)/expCount;

        uniformity = Cephes::cephes_igamc(9.0/2.0, chi2/2.0);
    }
    return uniformity;
}



} // namespace niststs
} // namespace batteries
} // namespace rtt
