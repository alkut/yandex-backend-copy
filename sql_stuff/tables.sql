CREATE TYPE type AS ENUM ('file', 'folder');

CREATE TABLE item (
    id varchar(50) unique not null,
    url VARCHAR(255)  NOT NULL,
    size INT NOT NULL,
    type type NOT NULL,
    update DATE NOT NULL
);

CREATE INDEX item_id_index ON item USING hash (id);

CREATE TABLE connection (
    id VARCHAR(50) CONSTRAINT foreign_key_id REFERENCES item(id),
    parent_id VARCHAR(50) CONSTRAINT foreign_key_parent_id REFERENCES item(id),
    PRIMARY KEY (id, parent_id)
);

CREATE INDEX id_index ON connection USING hash (id);
CREATE INDEX parent_id_index ON connection USING hash (parent_id);

CREATE TABLE import (
    id VARCHAR(50) UNIQUE NOT NULL,
    url VARCHAR(255) NOT NULL,
    size INT NOT NULL,
    type type NOT NULL,
    update_date DATE NOT NULL,
    parent_id VARCHAR(50)
);

CREATE INDEX string_id_index ON import using hash (id);

CREATE table parents (
    id varchar(50) unique not null constraint paforeign_key_id references item(id)
);

CREATE INDEX parents_id_index ON parents using hash (id);
