#ifndef EVTOL_REPORT_H
#define EVTOL_REPORT_H

#include <list>
#include <map>
#include <memory>
#include <string>

#include "Aircraft.h"
#include "common_defs.h"

/**
 * Generates the final report
 *
 * TODO - adding sql (sqlite or a local sql server) seems like a better solution to track event
 * metadata, but I don't have time.
 */
class Report {
public:
    Report() = default;
    void addAircraftTelemetry(int aircraftId, const std::string& modelName,
                              const Telemetry& telemetry);

    /**
     * for CSV use delim to ',''  for stdout set delim to '\t'.
     */
    void writeReport(std::ostream& out, char delim = '\t') const;


    // convenience methods for testing / validation.
    std::list<std::string> getListOfModels() const;
    std::map<std::string, Telemetry> getCopyOfTelemetry() const;

private:
    // stored using modelName for report output.
    std::map<std::string, Telemetry> totalModelTelemetry;
    std::map<std::string, int> modelCount;
};

#endif