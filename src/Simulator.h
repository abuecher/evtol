#ifndef EVTOL_SIMULATOR_H
#define EVTOL_SIMULATOR_H

#include <queue>
#include <string>
#include <vector>

#include "common_defs.h"
#include "Aircraft.h"
#include "Report.h"

/**
 * Comparison operator for the waiting queue
 */
struct WaitingChargingEventComparator {
    bool operator()(const ChargingEvent& a, const ChargingEvent& b) const {
        return a.startWaitingTime > b.startWaitingTime;
    }
};

/**
 * Comparison operator for the active queue (the ones on the chargers)
 */
struct ActiveChargingEventComparator {
    bool operator()(const ChargingEvent& a, const ChargingEvent& b) const {
        return a.chargingCompletionTime > b.chargingCompletionTime;
    }
};

using WaitingQueue_T =
    std::priority_queue<ChargingEvent, std::deque<ChargingEvent>, WaitingChargingEventComparator>;

using ActiveChagingQueue_t =
    std::priority_queue<ChargingEvent, std::deque<ChargingEvent>, ActiveChargingEventComparator>;


class Simulator {
private:
    std::vector<AircraftInstance> _aircrafts;

    ModelId_t pickRandomModel() const;

public:
    std::vector<AircraftInstance> generateTestAircraft(int numAircraft) const;
    void initChargerWaitingQueue(WaitingQueue_T& waitingQueue, Stamp_t simDuration);
    void simulateUntil(int numAircraft, Stamp_t endTime);
    Report generateSimulationReport() const; ///< generate report for all aircraft
    void writeReport(const Report& report) const;
};

#endif // EVTOL_SIMULATOR_H
