/*
  network table
  id: integer
  name: string

  primary key: id

  Used to group assets.
*/
CREATE TABLE network (
  id SERIAL PRIMARY KEY,
  name TEXT
);

/*
  asset table
  id: integer
  name: string
  network_id: foreign key -> network

  primary key: id

  Represents node on a network.
  Apart of a network, has many
  qualities and topologies.
 */
CREATE TABLE asset (
  id SERIAL PRIMARY KEY,
  name TEXT,
  network_id INTEGER REFERENCES network(id)
);

/*
  quality table
  asset_id: foreign key -> asset
  property: string
  value: string

  primary key: (asset_id, property)

  Defines which properties and values and
  attached them to an asset.
 */
CREATE TABLE quality (
  asset_id INTEGER REFERENCES asset(id),
  property TEXT,
  value TEXT,
  PRIMARY KEY (asset_id, property)
);

/*
  topology table
  asset_from_id: foreign key -> asset
  asset_to_id: foreign key -> asset
  relationship: string

  primary key: (asset_from_id, asset_to_id)

  constraints: asset_from_id and asset_to_id
    must be in the same network.

  Represents which assets are connected and the
  trust relationship between those assets.
 */
CREATE TABLE topology (
  asset_from_id INTEGER REFERENCES asset(id),
  asset_to_id INTEGER REFERENCES asset(id),
  connected BOOL,
  trusted BOOL,
  PRIMARY KEY (asset_from_id, asset_to_id)
  -- Need constraint to ensure both assets are in the same network
);
