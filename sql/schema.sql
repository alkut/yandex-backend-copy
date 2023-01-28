CREATE TABLE id (
    id  SERIAL PRIMARY KEY,
    string_id VARCHAR(50) UNIQUE NOT NULL
);

CREATE INDEX id_index ON id using hash (string_id);

CREATE TYPE type AS ENUM ('file', 'folder');

CREATE TABLE item (
    id SERIAL PRIMARY KEY CONSTRAINT foreign_key REFERENCES id(id),
    url VARCHAR(255)  NOT NULL,
    size INT NOT NULL,
    type type NOT NULL,
    update DATE NOT NULL
);

CREATE INDEX item_id_index ON item using hash (id);

CREATE TABLE parent (
    id SERIAL CONSTRAINT foreign_key_id REFERENCES id(id),
    parent_id SERIAL CONSTRAINT foreign_key_parent_id REFERENCES id(id),
    PRIMARY KEY (id, parent_id)
);

CREATE INDEX parent_id_index ON parent using hash (id);


CREATE TABLE children_folders (
    id SERIAL CONSTRAINT foreign_key_folder REFERENCES id(id),
    folder_id SERIAL CONSTRAINT foreign_key_sub_folder REFERENCES id(id),
    PRIMARY KEY (id, folder_id)
);

CREATE INDEX children_folders_index ON children_folders using hash (id);

CREATE TABLE children_files (
    id SERIAL CONSTRAINT foreign_key_file REFERENCES id(id),
    file_id SERIAL CONSTRAINT foreign_key_sub_file REFERENCES id(id),
    PRIMARY KEY (id, file_id)
);

CREATE INDEX children_files_index ON children_files using hash (id);

CREATE TABLE update_history (
    id SERIAL CONSTRAINT foreign_key_history REFERENCES id(id),
    update DATE NOT NULL
);

CREATE INDEX update_history_index ON update_history using hash (id);