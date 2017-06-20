CREATE TABLE network (
  id SERIAL PRIMARY KEY,
  name TEXT
);

CREATE TABLE factbase (
  id SERIAL PRIMARY KEY,
  node_id INTEGER,
  fact INTEGER,
  UNIQUE (node_id, fact)
);

CREATE TABLE edge (
  id INTEGER,
  from_node INTEGER REFERENCES factbase(id),
  to_node INTEGER REFERENCES factbase(id)
);

CREATE TABLE asset (
  id SERIAL PRIMARY KEY,
  name TEXT,
  network_id INTEGER REFERENCES network(id)
);

CREATE TABLE quality (
  asset_id INTEGER REFERENCES asset(id),
  property TEXT,
  value TEXT,
  PRIMARY KEY (asset_id, property)
);

CREATE TABLE topology (
  asset_from_id INTEGER REFERENCES asset(id),
  asset_to_id INTEGER REFERENCES asset(id),
  options TEXT,
  PRIMARY KEY (asset_from_id, asset_to_id)
);

CREATE TABLE exploit (
  id SERIAL PRIMARY KEY,
  name TEXT,
  params INTEGER
);

CREATE TABLE exploit_precondition (
  id SERIAL PRIMARY KEY,
  exploit_id INTEGER REFERENCES exploit(id),
  type INTEGER, -- 0 for quality, 1 for topology
  param1 INTEGER,
  param2 INTEGER,
  property TEXT,
  value TEXT,
  options TEXT
);

CREATE TABLE exploit_postcondition (
  id SERIAL PRIMARY KEY,
  exploit_id INTEGER REFERENCES exploit(id),
  type INTEGER, -- 0 for quality, 1 for topology
  param1 INTEGER,
  param2 INTEGER,
  property TEXT,
  value TEXT,
  options TEXT
);
