#ifndef EVTOL_REPORT_H
#define EVTOL_REPORT_H

#include <list>
#include <map>
#include <memory>
#include <string>

/**
 * Emit events (logfile or sqlite table, putting off the decistion until later)
 * Generates the final report
 *
 * ADAM_DEBUG adding sqlite is what I'd generally do for a real sim, but might be overkill for this.
 */
class Report {
    //       private:
    //         ModelId_t _modelId;
    //         int _count; // number of aircraft of this model.
    //         double _avg_flight_time_per_flight;

    //         // TODO assumption I include the time waiting for a charger. If
    //         // asumption not valid need to adjust final report calculation
    //         double avg_time_charging_per_flight;
    //         Telemetry _telemetry_totals;

public:
    Report(); // ctor inits all to 0.
              //         void addTelemetry(const AircraftInstance &aircraft,
              //                            bool includePartialFlightMiles);
    std::string toString() const;

private:
    // instead of using a db and generating the report from there, all Telemetry data is stored in
    // the aircraft instances. Report will build aggregates based on modelId, calculate averages and
    // export to CSV and/or stdout.
};

#endif