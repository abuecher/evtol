#ifndef EVTOL_REPORT_H
#define EVTOL_REPORT_H

#include <list>
#include <map>
#include <memory>
#include <string>

#include "common_defs.h"
#include "Aircraft.h"

/**
 * Emit events (logfile or sqlite table, putting off the decistion until later)
 * Generates the final report
 *
 * Note - adding sql (swlite or a locla sql server) seems like a better solution to track event metadata, but I don't have time.
 */
class Report {
public:
    Report();
    void addAircraftTelemetry(int aircraftId, const std::string& modelName, const Telemetry& telemetry);

    /**
     * for CSV ser delim to ,  for stdout set delim to \t.
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