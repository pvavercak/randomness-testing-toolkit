#ifndef RTT_BATTERIES_RESULT_PVALUESET_H
#define RTT_BATTERIES_RESULT_PVALUESET_H

#include <vector>
#include <string>

#include "rtt/constants.h"
#include "rtt/rttexception.h"

namespace rtt {
namespace batteries {
namespace result {

class PValueSet {
public:
    static PValueSet getInstance(std::string statName, double statRes,
                                 const std::vector<double> & pValues);

    std::string getStatName() const;

    double getStatRes() const;

    std::vector<double> getPValues() const;

private:
    PValueSet(std::string statName, double statRes,
              const std::vector<double> & pValues)
        : statName(statName), statRes(statRes), pValues(pValues)
    {}

    std::string statName;
    double statRes;
    std::vector<double> pValues;
};


} // namespace result
} // namespace batteries
} // namespace rtt

#endif // RTT_BATTERIES_RESULT_PVALUESET_H
