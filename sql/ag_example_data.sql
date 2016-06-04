INSERT INTO network VALUES
  (DEFAULT, 'Home');

INSERT INTO asset VALUES
  (DEFAULT, 'router', (SELECT id FROM network WHERE name = 'Home')),
  (DEFAULT, 'laptop', (SELECT id FROM network WHERE name = 'Home')),
  (DEFAULT, 'attacker', (SELECT id FROM network WHERE name = 'Home'));

INSERT INTO quality VALUES
  (1, 'version', '1.6');

INSERT INTO topology VALUES
  (3, 1, 'connected_web'),
  (2, 1, 'connected');

INSERT INTO exploit VALUES
  (DEFAULT, 'router_vuln', 3);

INSERT INTO exploit_precondition VALUES
  (1, )