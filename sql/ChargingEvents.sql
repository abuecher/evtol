CREATE TABLE ChargingEvents (
    eventId INTEGER PRIMARY KEY AUTOINCREMENT,
    startWaitingTime TEXT NOT NULL,
    startChargingTime TEXT NOT NULL,
    chargingCompletionTime TEXT NOT NULL,
    aircraftId INTEGER NOT NULL,
    FOREIGN KEY (aircraftId) REFERENCES AircraftInstances(aircraftId)
);
