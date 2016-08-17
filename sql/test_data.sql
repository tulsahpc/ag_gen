INSERT INTO network VALUES
  (DEFAULT, 'home');

INSERT INTO asset VALUES
  (DEFAULT, 'router', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'laptop', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'attacker', (SELECT id FROM network WHERE name = 'home'));

INSERT INTO quality VALUES
  (1, 'version', '1.6');

INSERT INTO topology VALUES
  (3, 1, 'connected_web'),
  (2, 1, 'connected');

INSERT INTO exploit VALUES
  (DEFAULT, 'router_vuln', 3);

INSERT INTO exploit_precondition VALUES
  (1, 1, 'type', 'router'),
  (1, 1, 'version', '1.6');

INSERT INTO exploit_postcondition VALUES
  (1, 1, 'access', 'root'),
  (1, 1, 'light', 'on');
