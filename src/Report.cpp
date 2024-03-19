#include "Report.h"
#include <iostream>
#include <string>
#include <list>
#include <set>

// TODO pull all the db stuff into a separate file so it makes it easier to not use it.
Report::Report() {}

void Report::addAircraftTelemetry(int aircraftId, const std::string& modelName, const Telemetry &telemetry) {
    std::set<int> aircraftReported;
    if (aircraftReported.find(aircraftId) != aircraftReported.end()) {
        std::cout << "dedupe - aircraft " << aircraftId << " telemetry already reported" << std::endl;
        return;
    }
    aircraftReported.insert(aircraftId);
    auto telemIter = this->totalModelTelemetry.find(modelName);
    if (telemIter == totalModelTelemetry.end()) {
        totalModelTelemetry[modelName] = telemetry;
        modelCount[modelName] = 1;
    } else {
        modelCount[modelName]++;
        telemIter->second.totalMilesTraveled += telemetry.totalMilesTraveled;
        telemIter->second.totalFlightHours += telemetry.totalFlightHours;
        telemIter->second.totalFaults += telemetry.totalFaults;
        telemIter->second.totalChargingTime += telemetry.totalChargingTime;
        telemIter->second.chargingStops += telemetry.chargingStops;
        telemIter->second.passengers += telemetry.passengers;
    }
}

// for CSV ser delim to ,  for stdout set delim to \t.
void Report::writeReport(std::ostream &out, char delim) const {
    // table header
    out << "Model" << delim << "aircraft-count" << delim << "avg-flight-time" << delim << "avg-distance-per-flight"
        << delim << "avg-time-charging-per-charge-session" << delim << "total-faults" << delim
        << "total-passenger-miles\n";
    for (const auto item : this->totalModelTelemetry) {
        auto instanceCount = modelCount.at(item.first);
        double avg_flight_time_min= item.second.totalFlightHours / instanceCount;
        double avg_distance_per_flight = item.second.totalMilesTraveled / instanceCount;
        double avg_charging_per_session = item.second.totalChargingTime / item.second.chargingStops;
        int faults = item.second.totalFaults;
        int totalPassMiles = item.second.totalMilesTraveled * item.second.passengers;

        out << item.first << delim << instanceCount << delim << avg_flight_time_min / 60 << delim << avg_distance_per_flight << delim
            << avg_charging_per_session / 60 << delim << faults << delim << totalPassMiles << "\n";
    }
}

std::map<std::string, Telemetry> Report::getCopyOfTelemetry() const {
    return std::map<std::string, Telemetry>(this->totalModelTelemetry.begin(), this->totalModelTelemetry.end());
}

// for testing / validation
std::list<std::string> Report::getListOfModels() const {
    std::list<std::string> modelNames;
    for (auto item = totalModelTelemetry.begin(); item != totalModelTelemetry.end(); ++item) {
        modelNames.push_back(item->first);
    }
    return modelNames;
}
