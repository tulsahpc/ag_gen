INSERT INTO network VALUES
  (DEFAULT, 'home'),
  (DEFAULT, 'test');

INSERT INTO asset VALUES
  (DEFAULT, 'routerA', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'routerB', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'routerC', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'laptopA', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'laptopB', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'laptopC', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'tabletA', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'tabletB', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'tabletC', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'smartphoneA', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'smartphoneB', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'smartphoneC', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'tvA', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'tvB', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'tvC', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'desktopA', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'desktopB', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'desktopC', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'printerA', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'printerB', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'printerC', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'nas', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'emailserver', (SELECT id FROM network WHERE name = 'home')),
  (DEFAULT, 'internet', (SELECT id FROM network WHERE name = 'home')),

  (DEFAULT, 'test_router', (SELECT id FROM network WHERE name = 'test')),
  (DEFAULT, 'test_workstation', (SELECT id FROM network WHERE name = 'test'));


INSERT INTO quality VALUES
  (1, 'type', '=', 'router'),
  (1, 'version', '=', '1.6'),
  (2, 'type', '=', 'router'),
  (2, 'version', '=', '1.5'),
  (3, 'type', '=', 'router'),
  (3, 'version', '=', '1.6'),

  (4, 'type', '=', 'laptop'),
  (4, 'os', '=', 'winxp'),
  (4, 'adobereader', '=', '4'),
  (5, 'type', '=', 'laptop'),
  (5, 'os', '=', 'win7'),
  (5, 'adobereader', '=', '5'),
  (6, 'type', '=', 'laptop'),
  (6, 'os', '=', 'win10'),
  (6, 'adobereader', '=', '6'),

  (7, 'type', '=', 'tablet'),
  (7, 'os', '=', 'ios'),
  (8, 'type', '=', 'tablet'),
  (8, 'os', '=', 'android'),
  (9, 'type', '=', 'tablet'),
  (9, 'os', '=', 'kindle'),

  (10, 'type', '=', 'smartphone'),
  (10, 'os', '=', 'ios'),
  (10, 'light', '=', 'off'),
  (11, 'type', '=', 'smartphone'),
  (11, 'os', '=', 'android'),
  (11, 'light', '=', 'off'),
  (12, 'type', '=', 'smartphone'),
  (12, 'os', '=', 'blackberry'),
  (12, 'light', '=', 'off'),

  (13, 'type', '=', 'samsung'),
  (14, 'type', '=', 'visio'),
  (15, 'type', '=', 'sony'),

  (16, 'type', '=', 'desktop'),
  (16, 'os', '=', 'win8'),
  (16, 'photoshop', '=', '5'),
  (17, 'type', '=', 'desktop'),
  (17, 'os', '=', 'macosx'),
  (17, 'photoshop', '=', '5'),
  (18, 'type', '=', 'desktop'),
  (18, 'os', '=', 'win10'),
  (18, 'photoshop', '=', '5'),

  (19, 'type', '=', 'printer'),
  (19, 'model', '=', 'hp123'),
  (20, 'type', '=', 'printer'),
  (20, 'model', '=', 'hp456'),
  (21, 'type', '=', 'printer'),
  (21, 'model', '=', 'hp789'),

  (22, 'type', '=', 'nas'),
  (22, 'os', '=', 'freebsd'),
  (23, 'type', '=', 'emailserver'),
  (23, 'owner', '=', 'hillaryclinton'),
  (23, 'contents', '=', 'topsecrets'),
  (23, 'status', '=', 'compromised'),

  (24, 'type', '=', 'internet');

INSERT INTO topology VALUES
  (1, 2, 'connected'),
  (2, 3, 'connected'),
  (1, 3, 'connected'),
  (1, 2, 'trusted'),
  (2, 3, 'trusted'),
  (1, 3, 'trusted'),

  (1, 24, 'connected'),
  (2, 24, 'connected'),
  (3, 24, 'connected'),
  (1, 24, 'trusted'),
  (2, 24, 'trusted'),
  (3, 24, 'trusted'),

  (1, 4, 'connected'),
  (1, 4, 'trusted'),

  (1, 7, 'connected'),
  (1, 10, 'connected'),
  (1, 13, 'connected'),
  (1, 16, 'connected'),
  (1, 19, 'connected'),

  (1, 22, 'connected'),
  (1, 23, 'connected'),
  (1, 22, 'trusted'),
  (1, 23, 'trusted'),

  (2, 5, 'connected'),
  (2, 8, 'connected'),
  (2, 11, 'connected'),
  (2, 14, 'connected'),
  (2, 17, 'connected'),
  (2, 20, 'connected'),

  (2, 22, 'connected'),
  (2, 23, 'connected'),
  (2, 22, 'trusted'),
  (2, 23, 'trusted'),

  (3, 6, 'connected'),
  (3, 9, 'connected'),
  (3, 12, 'connected'),
  (3, 15, 'connected'),
  (3, 18, 'connected'),
  (3, 21, 'connected'),

  (3, 22, 'connected'),
  (3, 23, 'connected'),
  (3, 22, 'trusted'),
  (3, 23, 'trusted'),

  (10, 24, 'connected'),
  (11, 24, 'connected'),
  (12, 24, 'connected')
  (10, 24, 'trusted'),
  (11, 24, 'trusted'),
  (12, 24, 'trusted');

INSERT INTO exploit VALUES
  (DEFAULT, 'router_vuln', 1),
  (DEFAULT, 'printer_vuln', 1),
  (DEFAULT, 'adobereader_vuln', 1),
  (DEFAULT, 'photoshop_vuln', 1),
  (DEFAULT, 'kindle_vuln', 1),
  (DEFAULT, 'light_vuln', 1);

INSERT INTO exploit_precondition VALUES
  (DEFAULT, 1, 0, 1, NULL, 'type', 'router', '=', NULL),
  (DEFAULT, 1, 0, 1, NULL, 'version', '1.6', '=', NULL),

  (DEFAULT, 2, 0, 1, NULL, 'type', 'printer', '=', NULL),
  (DEFAULT, 2, 0, 1, NULL, 'model', '456', '=', NULL),

  (DEFAULT, 3, 0, 1, NULL, 'type', 'laptop', '=', NULL),
  (DEFAULT, 3, 0, 1, NULL, 'os', 'winxp', '=', NULL),
  (DEFAULT, 3, 0, 1, NULL, 'adobereader', '4', '=', NULL),

  (DEFAULT, 4, 0, 1, NULL, 'type', 'desktop', '=', NULL),
  (DEFAULT, 4, 0, 1, NULL, 'os', 'osx', '=', NULL),
  (DEFAULT, 4, 0, 1, NULL, 'photoshop', '5', '=', NULL),

  (DEFAULT, 5, 0, 1, NULL, 'type', 'tablet', '=', NULL),
  (DEFAULT, 5, 0, 1, NULL, 'os', 'kindle', '=', NULL),

  (DEFAULT, 6, 0, 1, NULL, 'type',' smartphone', '=', NULL),
  (DEFAULT, 6, 0, 1, NULL, 'os', 'ios', '=', NULL),
  (DEFAULT, 6, 0, 1, NULL, 'light', 'off', '=', NULL);

INSERT INTO exploit_postcondition VALUES
  (DEFAULT, 1, 0, 1, NULL, 'access', 'root', '=', NULL),
  (DEFAULT, 2, 0, 1, NULL, 'access', 'root', '=', NULL),
  (DEFAULT, 3, 0, 1, NULL, 'access', 'root', '=', NULL),
  (DEFAULT, 4, 0, 1, NULL, 'access', 'root', '=', NULL),
  (DEFAULT, 5, 0, 1, NULL, 'access', 'root', '=', NULL),
  (DEFAULT, 6, 0, 1, NULL, 'light', 'on', '=', NULL);
