WITH RECURSIVE get_nodes AS (
    SELECT id, file_id as next_level FROM children_files
                       WHERE children_files.id = 2
    UNION
    SELECT id, folder_id as next_level FROM children_folders
                       WHERE children_folders.id = 2
    UNION ALL
    SELECT get_nodes.id AS id, get_nodes.next_level AS next_level FROM get_nodes
                          INNER JOIN children_folders ON get_nodes.id = children_folders.id
) SELECT * FROM children_folders;