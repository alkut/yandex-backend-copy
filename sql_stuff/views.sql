create view get_all_first_level_parents as
(
select id
from new_items_from_import
union
select id
from import
union
select id
from old_parents
    );


create view old_parents as
(
select connection.parent_id id
from existent_items_from_import
         join connection on existent_items_from_import.id = connection.id
);
-- replace inner query with join
create view get_parents as
(
WITH recursive parent_of (id, parent_id)
                   AS
                   (
-- Anchor query
                       SELECT connection.id, connection.parent_id
                       FROM connection
                       WHERE connection.id in (select id from parents)
                       UNION ALL
-- Recursive query
                       SELECT connection.id, connection.parent_id
                       FROM connection
                                INNER JOIN parent_of
                                           ON connection.id = parent_of.parent_id
                                               and connection.id != 'nullptr')
-- Statement that executes the CTE
SELECT parent_of.id
FROM parent_of
         join item on parent_of.id = item.id
);

create view invalid_parent_count as
(
with all_id as
         (select id as child_id
          from item
          union
          select id
          from import)
select count(*)
from import
         left join all_id
                   on import.parent_id = all_id.child_id
where all_id.child_id is null
   or import.parent_id = import.id
);

create view change_type_count as
(
select count(*)
from import
         join item on import.id = item.id
where import.type != item.type
    );

create view new_items_from_import as
(
select import.id as id, import.parent_id as parent_id
from import
         left join item on import.id = item.id
where item.id is null
    );

create view existent_items_from_import as
(
select import.id as id, import.parent_id as parent_id
from import
         join item on import.id = item.id
    );
