SELECT file, timestamp, name FROM Event
LEFT JOIN EventType ON Event.event_type = EventType.id;