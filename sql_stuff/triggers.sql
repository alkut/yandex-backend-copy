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

	    -- existent items and parents update
	    -- insert new items
	    -- delete outdated items



	    delete from import;
	END IF;
	return new;
END;
$$;

CREATE TRIGGER on_import_changed
after insert
ON import
execute procedure validate_and_import();
