CREATE TABLE AircraftInstances (
    aircraftId INTEGER PRIMARY KEY,
    modelId INTEGER NOT NULL,
    flightTimePerCharge REAL,
    startSegmentTime TEXT, -- Represented as a text, assuming you'll store a timestamp here.
    totalFlightHours REAL NOT NULL,
    totalMilesTraveled REAL NOT NULL,
    totalChargingTime REAL NOT NULL,
    totalFaults INTEGER NOT NULL,
    FOREIGN KEY (modelId) REFERENCES AircraftModels (modelId)
);
