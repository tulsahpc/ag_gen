INSERT INTO network VALUES
  (DEFAULT, 'home');

INSERT INTO asset VALUES
  (DEFAULT, 'router', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'laptop', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'printer', (SELECT id FROM network WHERE name = 'home'));

INSERT INTO quality VALUES
  (1, 'type', 'router'),
  (1, 'version', '1.6'),
  (2, 'os', 'winxp'),
  (3, 'model', 'hp450');

INSERT INTO topology VALUES
  (1, 2, 'connected'),
  (1, 3, 'connected,trusted');

INSERT INTO exploit VALUES
  (DEFAULT, 'router_vuln', 3);

INSERT INTO exploit_precondition VALUES
  (1, 'type', 'router'),
  (1, 'version', '1.6');

INSERT INTO exploit_postcondition VALUES
  (1, 'access', 'root');
