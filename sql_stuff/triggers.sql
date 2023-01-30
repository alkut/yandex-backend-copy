CREATE OR REPLACE FUNCTION validate_and_import()
    RETURNS TRIGGER
    LANGUAGE PLPGSQL
    AS
$$
BEGIN
	IF ((select * from invalid_parent_count) + (select * from change_type_count) > 0)
	THEN
	     delete from import;
	     raise exception 'validation failed';
	ELSE
	    insert into connection
        select * from
        new_items_from_import;

        update connection
        set parent_id = existent_items_from_import.parent_id
        from existent_items_from_import
        where connection.id = existent_items_from_import.id;

	    delete from parents;
	    insert into parents
	    select id from get_all_first_level_parents;

	    -- update date
	    update item
        set update = (select update from import limit 1)
        from get_parents
        where item.id = get_parents.id;

	    -- update url
	    update item
	    set url = import.url
	    from existent_items_from_import join import on existent_items_from_import.id = import.id
	    where item.id = existent_items_from_import.id;



	    delete from import;
	END IF;
	return new;
END;
$$;

CREATE TRIGGER on_import_changed
    after insert
    ON import
    execute procedure validate_and_import();
