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
  (1, 'type', 'router'),
  (1, 'version', '1.6'),
  (2, 'type', 'router'),
  (2, 'version', '1.5'),
  (3, 'type', 'router'),
  (3, 'version', '1.4'),

  (4, 'type', 'laptop'),
  (4, 'os', 'winxp'),
  (4, 'adobereader', '4'),
  (5, 'type', 'laptop'),
  (5, 'os', 'win7'),
  (5, 'adobereader', '5'),
  (6, 'type', 'laptop'),
  (6, 'os', 'win10'),
  (6, 'adobereader', '6'),

  (7, 'type', 'tablet'),
  (7, 'os', 'ios'),
  (8, 'type', 'tablet'),
  (8, 'os', 'android'),
  (9, 'type', 'tablet'),
  (9, 'os', 'kindle'),

  (10, 'type', 'smartphone'),
  (10, 'os', 'ios'),
  (10, 'light', 'off'),
  (11, 'type', 'smartphone'),
  (11, 'os', 'android'),
  (11, 'light', 'off'),
  (12, 'type', 'smartphone'),
  (12, 'os', 'blackberry'),
  (12, 'light', 'off'),

  (13, 'type', 'samsung'),
  (14, 'type', 'visio'),
  (15, 'type', 'sony'),

  (16, 'type', 'desktop'),
  (16, 'os', 'win8'),
  (16, 'photoshop', '5'),
  (17, 'type', 'desktop'),
  (17, 'os', 'macosx'),
  (17, 'photoshop', '5'),
  (18, 'type', 'desktop'),
  (18, 'os', 'win10'),
  (18, 'photoshop', '5'),

  (19, 'type', 'printer'),
  (19, 'model', 'hp123'),
  (20, 'type', 'printer'),
  (20, 'model', 'hp456'),
  (21, 'type', 'printer'),
  (21, 'model', 'hp789'),

  (22, 'type', 'nas'),
  (22, 'os', 'freebsd'),
  (23, 'type', 'emailserver'),
  (23, 'owner', 'hillaryclinton'),
  (23, 'contents', 'topsecrets'),
  (23, 'status', 'compromised'),

  (24, 'type', 'internet');

INSERT INTO topology VALUES
  (1, 2, 'connected,trusted'),
  (2, 3, 'connected,trusted'),
  (1, 3, 'connected,trusted'),

  (1, 24, 'connected,trusted'),
  (2, 24, 'connected,trusted'),
  (3, 24, 'connected,trusted'),

  (1, 4, 'connected,trusted'),
  (1, 7, 'connected'),
  (1, 10, 'connected'),
  (1, 13, 'connected'),
  (1, 16, 'connected'),
  (1, 19, 'connected'),
  (1, 22, 'connected,trusted'),
  (1, 23, 'connected,trusted'),

  (2, 5, 'connected'),
  (2, 8, 'connected'),
  (2, 11, 'connected'),
  (2, 14, 'connected'),
  (2, 17, 'connected'),
  (2, 20, 'connected'),
  (2, 22, 'connected,trusted'),
  (2, 23, 'connected,trusted'),

  (3, 6, 'connected'),
  (3, 9, 'connected'),
  (3, 12, 'connected'),
  (3, 15, 'connected'),
  (3, 18, 'connected'),
  (3, 21, 'connected'),
  (3, 22, 'connected,trusted'),
  (3, 23, 'connected,trusted'),

  (10, 24, 'connected,trusted'),
  (11, 24, 'connected,trusted'),
  (12, 24, 'connected,trusted');

INSERT INTO exploit VALUES
  (DEFAULT, 'router_vuln', 3),
  (DEFAULT, 'printer_vuln', 3),
  (DEFAULT, 'adobereader_vuln', 3),
  (DEFAULT, 'photoshop_vuln', 3),
  (DEFAULT, 'kindle_vuln', 3),
  (DEFAULT, 'light_vuln', 3);

INSERT INTO exploit_precondition VALUES
  (1, 'type', 'router'),
  (1, 'version', '1.6'),

  (2, 'type', 'printer'),
  (2, 'model', '456'),

  (3, 'type', 'laptop'),
  (3, 'os', 'winxp'),
  (3, 'adobereader', '4'),

  (4, 'type', 'desktop'),
  (4, 'os', 'osx'),
  (4, 'photoshop', '5'),

  (5, 'type', 'tablet'),
  (5, 'os', 'kindle'),

  (6, 'type',' smartphone'),
  (6, 'os', 'ios'),
  (6, 'light', 'off');

INSERT INTO exploit_postcondition VALUES
  (1, 'access', 'root'),
  (2, 'access', 'root'),
  (3, 'access', 'root'),
  (4, 'access', 'root'),
  (5, 'access', 'root'),
  (6, 'light', 'on');
