CREATE TYPE type AS ENUM ('file', 'folder');

CREATE TABLE item (
                      id VARCHAR(50) UNIQUE NOT NULL,
                      url VARCHAR(255)  NOT NULL,
                      size INT NOT NULL,
                      type type NOT NULL,
                      update DATE NOT NULL
);

CREATE INDEX item_id_index ON item USING hash (id);

CREATE TABLE connection (
                            id VARCHAR(50) CONSTRAINT foreign_key_connection_id REFERENCES item(id),
                            parent_id VARCHAR(50) CONSTRAINT foreign_key_connection_parent_id REFERENCES item(id),
                            PRIMARY KEY (id, parent_id)
);

CREATE INDEX connection_id_index ON connection USING hash (id);
CREATE INDEX connection_parent_id_index ON connection USING hash (parent_id);

CREATE TABLE import (
                        id VARCHAR(50) UNIQUE NOT NULL,
                        url VARCHAR(255) NOT NULL,
                        size INT NOT NULL,
                        type type NOT NULL,
                        update_date DATE NOT NULL,
                        parent_id VARCHAR(50)
);

CREATE INDEX import_id_index ON import USING hash (id);

CREATE TABLE parents (
    id VARCHAR(50) UNIQUE NOT NULL CONSTRAINT foreign_key_parents_id REFERENCES item(id)
);

CREATE INDEX parents_id_index ON parents USING hash (id);

CREATE TABLE children (
    id VARCHAR(50) UNIQUE NOT NULL CONSTRAINT foreign_key_children_id REFERENCES item(id)
);
CREATE INDEX children_id_index ON children USING hash (id);

CREATE TABLE delete (
                        id VARCHAR(50) CONSTRAINT foreign_key_delete_id REFERENCES item(id),
                        update DATE NOT NULL
);

CREATE VIEW new_items_from_import AS (
                                     SELECT
                                         import.id AS id, import.parent_id AS parent_id
                                     FROM
                                         import
                                             LEFT JOIN
                                         item
                                         ON
                                                 import.id = item.id
                                     WHERE
                                         item.id IS NULL
                                         );

CREATE VIEW existent_items_from_import AS (
                                          SELECT
                                              import.id AS id, import.parent_id AS parent_id
                                          FROM
                                              import
                                                  JOIN
                                              item
                                              ON
                                                      import.id = item.id
                                              );

CREATE VIEW old_parents AS (
                           SELECT
                               connection.parent_id AS id
                           FROM
                               existent_items_from_import
                                   JOIN
                               connection
                               ON
                                       existent_items_from_import.id = connection.id
                               );

CREATE VIEW get_all_first_level_parents AS (
                                           SELECT
                                               id
                                           FROM
                                               new_items_from_import
                                           UNION
                                           SELECT
                                               id
                                           FROM
                                               import
                                           UNION
                                           SELECT
                                               id
                                           FROM
                                               old_parents
                                               );

CREATE VIEW get_parents AS (
                           WITH RECURSIVE parent_of (id, parent_id) AS (
                               SELECT
                                   connection.id AS id, connection.parent_id AS parent_id
                               FROM
                                   connection
                               WHERE
                                       connection.id IN (SELECT id FROM parents)
                               UNION
                               SELECT
                                   connection.id AS id, connection.parent_id AS parent_id
                               FROM
                                   connection
                                       INNER JOIN
                                   parent_of
                                   ON
                                               connection.id = parent_of.parent_id
                                           AND
                                               connection.id != 'nullptr'
                               )
SELECT
    parent_of.id AS id
FROM
    parent_of
    );

CREATE VIEW invalid_parent_count AS (
                                    WITH all_id AS (
                                        SELECT
                                            id AS child_id
                                        FROM
                                            item
                                        UNION
                                        SELECT
                                            id
                                        FROM
                                            import
                                    )
                                    SELECT
                                        COUNT(*)
                                    FROM
                                        import
                                            LEFT JOIN
                                        all_id
                                        ON
                                                import.parent_id = all_id.child_id
                                    WHERE
                                        all_id.child_id IS NULL
                                       OR
                                            import.parent_id = import.id
                                        );

CREATE VIEW change_type_count AS (
                                 SELECT
                                     COUNT(*)
                                 FROM
                                     import
                                         JOIN
                                     item
                                     ON
                                             import.id = item.id
                                 WHERE
                                     import.type != item.type
);

CREATE VIEW get_children AS (
                            WITH RECURSIVE children_of (id, parent_id) AS (
                                SELECT
                                    connection.id AS id, connection.parent_id AS parent_id
                                FROM
                                    connection
                                WHERE
                                        connection.id IN (SELECT id FROM children)
                                UNION
                                SELECT
                                    connection.id AS id, connection.parent_id AS parent_id
                                FROM
                                    connection
                                        INNER JOIN
                                    children_of
                                    ON
                                            connection.parent_id = children_of.id
                            )
                            SELECT
                                children_of.id AS id
                            FROM
                                children_of
                                );

CREATE OR REPLACE FUNCTION validate_and_import()
RETURNS TRIGGER
LANGUAGE PLPGSQL AS $$
BEGIN
	IF
(SELECT * FROM invalid_parent_count) + (SELECT * FROM change_type_count) > 0
	THEN
DELETE FROM
    import;
RAISE EXCEPTION 'validation failed';
ELSE
        INSERT INTO
            connection
SELECT
    *
FROM
    new_items_from_import;

UPDATE
    connection
SET
    parent_id = existent_items_from_import.parent_id
    FROM
            existent_items_from_import
WHERE
    connection.id = existent_items_from_import.id;

DELETE FROM
    parents;

INSERT INTO
    parents
SELECT
    id
FROM
    get_all_first_level_parents;

-- update date
UPDATE
    item
SET
    update = (SELECT update FROM import LIMIT 1)
FROM
    get_parents
WHERE
    item.id = get_parents.id;

-- update url
-- update size
UPDATE
    item
SET
    url = import.url, size = import.size
    FROM
            existent_items_from_import
        JOIN
            import
ON
    existent_items_from_import.id = import.id
WHERE
    item.id = existent_items_from_import.id;

-- insert new items
INSERT INTO
    item(id, url, size, type, update)
SELECT
    import.id AS id,
    import.url AS url,
    import.size AS size,
            import.type AS type,
            import.update_date AS update
                                                      FROM
                                                          new_items_from_import
                                                      JOIN
                                                      import
                                  ON
                                                      new_items_from_import.id = import.id;

DELETE FROM
    import;
END IF;
RETURN NEW;
END;
$$;

CREATE TRIGGER
    on_import_changed
    AFTER INSERT
    ON
        import
    EXECUTE PROCEDURE
        validate_and_import();

CREATE OR REPLACE FUNCTION validate_and_delete()
RETURNS TRIGGER
LANGUAGE PLPGSQL AS $$
BEGIN
DELETE FROM
    parents;

INSERT INTO
    parents
SELECT
    id
FROM
    delete;

-- update date of parents
UPDATE
    item
SET
    update = (SELECT update FROM import LIMIT 1)
FROM
    get_parents
WHERE
    item.id = get_parents.id;

-- delete children
DELETE FROM
    connection
WHERE
        connection.id IN (SELECT * FROM get_children);

DELETE FROM
    item
WHERE
        item.id IN (SELECT * FROM get_children);

DELETE FROM
    delete;
RETURN NEW;
END;
$$;

CREATE TRIGGER
    on_delete_changed
    AFTER INSERT
    ON
        delete
    EXECUTE PROCEDURE
        validate_and_delete();