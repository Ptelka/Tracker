CREATE TABLE EventType (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT
);

CREATE TABLE Event (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
    file TEXT,
    timestamp INTEGER,
    event_type INTEGER,
    FOREIGN KEY(event_type) REFERENCES EventType(id)
);

INSERT INTO EventType (id, name) VALUES (0, "File Created");
INSERT INTO EventType (id, name) VALUES (1, "File Moved From");
INSERT INTO EventType (id, name) VALUES (2, "File Moved To");
INSERT INTO EventType (id, name) VALUES (3, "File Deleted");
INSERT INTO EventType (id, name) VALUES (4, "File Modified");
INSERT INTO EventType (id, name) VALUES (5, "Process Started");
INSERT INTO EventType (id, name) VALUES (6, "Process Stopped");
