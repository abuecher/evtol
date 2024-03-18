CREATE TABLE TimedEvents (
    eventId INTEGER PRIMARY KEY AUTOINCREMENT,
    eventType INTEGER NOT NULL,
    startTime TEXT NOT NULL,
    endTime TEXT NOT NULL,
    chargingCompletionTime TEXT NOT NULL,
    aircraftId INTEGER NOT NULL,
    FOREIGN KEY (eventType) REFERENCES EventTypes(eventType),
    FOREIGN KEY (aircraftId) REFERENCES AircraftInstances(aircraftId)
);
