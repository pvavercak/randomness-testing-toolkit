#ifndef RTT_STORAGE_MYSQLSTORAGE_H
#define RTT_STORAGE_MYSQLSTORAGE_H

#include <cstdint>
#include <memory>
#include <mysql-cppconn-8/jdbc/cppconn/driver.h>
#include <mysql-cppconn-8/jdbc/cppconn/exception.h>
#include <mysql-cppconn-8/jdbc/cppconn/prepared_statement.h>

#include "clinterface/rttclioptions.h"
#include "globalcontainer.h"
#include "storage/istorage.h"

namespace rtt {
namespace storage {

using namespace clinterface;

class MySQLStorage : public IStorage {
public:
    static std::unique_ptr<MySQLStorage> getInstance(const GlobalContainer & container);

    void init();

    void writeResults(const std::vector<batteries::ITestResult *> & testResults);

    void close();

    void addBatteryError(const std::string & error);

    void addBatteryErrors(const std::vector<std::string> & errors);

    void addBatteryWarning(const std::string & warning);

    void addBatteryWarnings(const std::vector<std::string> & warnings);

private:
    /*
    =================
    *** Variables ***
    =================
    */
    static const std::string objectInfo;
    /* Pointers to global objects */
    RTTCliOptions * rttCliOptions;
    ToolkitSettings * toolkitSettings;

    time_t creationTime;
    BatteryArg battery;

    sql::Driver * driver;
    std::unique_ptr<sql::Connection> conn;
    std::uint64_t dbBatteryId = 0;
    std::uint64_t currDbTestId = 0;
    std::uint64_t currDbVariantId = 0;
    std::uint64_t currDbSubtestId = 0;
    int currTestIdx = 0;
    int currSubtestIdx = 0;
    int currVariantIdx = 0;
    int totalTestCount = 0;
    int passedTestCount = 0;

    /*
    ===============
    *** Methods ***
    ===============
    */
    MySQLStorage() {}

    void addNewTest(const std::string & testName);
    void finalizeTest();

    void addVariant();
    void finalizeVariant();

    void addSubTest();
    void finalizeSubTest();

    void setTestResult(bool passed);

    void setTestPartialAlpha(double alpha);

    void setUserSettings(
            const std::vector<std::pair<std::string, std::string> > & options);

    void setTestParameters(
            const std::vector<std::pair<std::string, std::string> > & options);

    void setVariantWarnings(const std::vector<std::string> & warnings);

    void setVariantErrors(const std::vector<std::string> & errors);

    void setVariantStdErr(const std::vector<std::string> & stderr);

    void addStatisticResult(
            const std::string & statName ,
            double value, bool passed);

    void addPValues(const std::vector<double> & pvals);

    void finalizeReport();

    std::uint64_t getLastInsertedId();
};

} // namespace storage
} // namespace rtt

#endif // RTT_STORAGE_MYSQLSTORAGE_H
