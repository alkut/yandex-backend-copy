INSERT INTO id (string_id) VALUES
                                    ('nullptr'),
                                    ('folder1'),
                                    ('file1'),
                                    ('folder2');

INSERT INTO parent (id, parent_id) VALUES
                                       (1, 1),
                                       (2, 1),
                                       (3, 2),
                                       (4, 2);

INSERT INTO item (id, url, size, type, update) VALUES
                                                   (2, 'url/folder1', 5, 'folder'::type, '2017-03-17 18:37:29.229'::DATE),
                                                   (3, 'url/file1', 5, 'file'::type, '2017-03-17 18:37:29.229'::DATE),
                                                   (4, 'url/folder2', 0, 'folder'::type, '2017-03-17 18:37:29.229'::DATE);

INSERT INTO children_folders (id, folder_id) VALUES
                                                 (2, 4);

INSERT INTO children_files (id, file_id) VALUES
                                             (2, 3);

INSERT INTO update_history (id, update) VALUES
                                            (2, '2017-03-17 18:37:29.229'::DATE),
                                            (3, '2017-03-17 18:37:29.229'::DATE),
                                            (4, '2017-03-17 18:37:29.229'::DATE);