#include "Report.h"
#include <iostream>

std::string Report::toString() const { return "ADAM_DEBUG placeholder"; }

const std::string DBFILENAME = "evtol.db";

bool openDatabase(const std::string & filename, sqlite3 **db) {
    int rc = sqlite3_open(filename.c_str(), db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(*db)
                  << std::endl;
        sqlite3_close(*db);
        *db = nullptr;
        return false;
    } else {
        std::cout << "Opened database successfully" << std::endl;
        return true;
    }
}

// TODO pull all the db stuff into a separate file so it makes it easier to not use it.
Report::Report() : db{nullptr} {
    // _modelId = NONE;
    // _count = 0;
    // _avg_flight_time_per_flight = 0;
    // avg_time_charging_per_flight = 0;
    // _telemetry_totals = {0, 0, 0, 0};
    if (!openDatabase(DBFILENAME, &db)) {
        std::cerr << "Unable to open datase " << DBFILENAME
                  << ". Some analytics will be missing from report"
                  << std::endl;
    }
    int rc = sqlite3_open(DBFILENAME.c_str(), &db);
    if (rc) {
        std::cerr << "Unable to open datase " << DBFILENAME << ". Some analytics will be missing from report" << std::endl;
        // Handle error: can't open database
        sqlite3_close(db); // Make sure to close if opening partially
                                 // succeeded but returned an error
        db = nullptr;
    } else {
        // TODO if theis is a new database, crrate the tables from the sql files:
        // ChargingEvents.sql
        // AircraftInstance.sql
        // AircraftModels.sql
    }
}
