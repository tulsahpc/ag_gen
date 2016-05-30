INSERT INTO network VALUES
  (DEFAULT, 'Example1');

INSERT INTO asset VALUES
  (DEFAULT, 'host1', (SELECT id FROM network WHERE name = 'Example1')),
  (DEFAULT, 'host2', (SELECT id FROM network WHERE name = 'Example1')),
  (DEFAULT, 'host3', (SELECT id FROM network WHERE name = 'Example1')),
  (DEFAULT, 'host4', (SELECT id FROM network WHERE name = 'Example1')),
  (DEFAULT, 'host5', (SELECT id FROM network WHERE name = 'Example1')),
  (DEFAULT, 'host6', (SELECT id FROM network WHERE name = 'Example1'));

INSERT INTO quality VALUES
  (1, 'OS', 'WinXP'),
  (2, 'OS', 'MacOSX'),
  (3, 'OS', 'Unix'),
  (4, 'OS', 'OS2'),
  (5, 'OS', 'OS2'),
  (6, 'OS', 'OS2');

INSERT INTO topology VALUES
  (1, 2, TRUE, TRUE),
  (1, 3, TRUE, TRUE),
  (1, 4, TRUE, TRUE),
  (1, 6, TRUE, TRUE),
  (2, 3, TRUE, FALSE),
  (3, 4, TRUE, FALSE),
  (6, 4, TRUE, FALSE);
