CREATE FUNCTION new_factbase(myhash TEXT) RETURNS INTEGER AS $$
DECLARE
  myid INTEGER;
BEGIN
  IF NOT EXISTS (SELECT 1 FROM factbase WHERE hash = myhash) THEN
    INSERT INTO factbase VALUES (DEFAULT, myhash) RETURNING id INTO myid;
  ELSE
    SELECT id INTO myid FROM factbase WHERE hash = myhash;
  END IF;

  RETURN myid;
END;
$$ LANGUAGE plpgsql;
