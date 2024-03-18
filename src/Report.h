#ifndef EVTOL_REPORT_H
#define EVTOL_REPORT_H

#include <string>
#include <memory>

#define USE_SQLITE

#ifdef USE_SQLITE
#include <sqlite3.h>

// ADAM_DEBUG do I want to uise a custom deleter to auto-close the db?
// struct SqliteDeleter {
//     void operator()(sqlite3 *db) const {
//         sqlite3_close(db); // Close the database connection properly
//     }
// };
#endif


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
              //         void add_telemetry(const AircraftInstance &aircraft,
              //                            bool includePartialFlightMiles);
    std::string toString() const;

#ifdef USE_SQLITE
    // std::unique_ptr<sqlite3, SqliteDeleter> dbPtr; // ADAM_DEBUG playing w/a custom deleter. Not sold on it.
    sqlite3* db;
#endif
};

#endif