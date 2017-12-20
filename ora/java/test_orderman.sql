
-- :!sqlplus db/myoradb\$db @order

--################################################################### calling
/*
call dbms_java.grant_permission('DB', 'SYS:java.util.logging.LoggingPermission', 'control', '' );
call dbms_java.grant_permission('DB', 'SYS:java.io.FilePermission', '<<ALL FILES>>', 'read,write,delete');
COMMIT;
*/


DELETE FROM lineitems;
DELETE FROM orders;
DELETE FROM stockitems;
DELETE FROM customers;

SET SERVEROUTPUT ON
CALL dbms_java.set_output(10000);
BEGIN
    -- w/o path --> curr. path=$ORACLE_HOME
    --orderman.init('/vol/orderman.log'); --nok
    --orderman.init('/vol/db/oralog/orderman.log'); -- ok
    orderman.init('/home/db/log/ora/orderman.log'); -- ok, but ~db nok
    --# for log in $(ls -1tr ~db/log/ora/orderman.log.[0-9]) ; do echo "### $log"; cat $log; done | view -
    orderman.add_stock_item(2010, 'camshaft', 245.00);
    orderman.add_stock_item(2011, 'connecting rod', 122.50);
    orderman.add_stock_item(2012, 'crankshaft', 388.25);
    orderman.add_stock_item(2013, 'cylinder head', 201.75);
    orderman.add_stock_item(2014, 'cylinder sleeve', 73.50);
    orderman.add_stock_item(2015, 'engine bearning', 43.85);
    orderman.add_stock_item(2016, 'flywheel', 155.00);
    orderman.add_stock_item(2017, 'freeze plug', 17.95);
    orderman.add_stock_item(2018, 'head gasket', 36.75);
    orderman.add_stock_item(2019, 'lifter', 96.25);
    orderman.add_stock_item(2020, 'oil pump', 207.95);
    orderman.add_stock_item(2021, 'piston', 137.75);
    orderman.add_stock_item(2022, 'piston ring', 21.35);
    orderman.add_stock_item(2023, 'pushrod', 110.00);
    orderman.add_stock_item(2024, 'rocker arm', 186.50);
    orderman.add_stock_item(2025, 'valve', 68.50);
    orderman.add_stock_item(2026, 'valve spring', 13.25);
    orderman.add_stock_item(2027, 'water pump', 144.50);
    COMMIT;
END;
/

SET SERVEROUTPUT ON
CALL dbms_java.set_output(10000);
BEGIN
    orderman.add_customer(101, 'A-1 Automotive', '4490 Stevens Blvd', 'San Jose', 'CA', '95129', '408-555-1212');
    orderman.add_customer(102, 'AutoQuest', '2032 America Ave', 'Hayward', 'CA', '94545', '510-555-1212');
    orderman.add_customer(103, 'Bell Auto Supply', '305 Cheyenne Ave', 'Richardson', 'TX', '75080', '972-555-1212');
    orderman.add_customer(104, 'CarTech Auto Parts', '910 LBJ Freeway', 'Dallas', 'TX', '75234', '214-555-1212');
    COMMIT;
END;
/

SET SERVEROUTPUT ON
CALL dbms_java.set_output(10000);
BEGIN
    orderman.add_order(30501, 103, '150201', '150210', '305 Cheyenne Ave', 'Richardson', 'TX', '75080');
    orderman.add_line_item(01, 30501, 2011, 5, 0.02);
    orderman.add_line_item(02, 30501, 2018, 25, 0.10);
    orderman.add_line_item(03, 30501, 2026, 10, 0.05);

    orderman.add_order(30502, 102, '150201', '150207', '2032 America Ave', 'Hayward', 'CA', '94545');
    orderman.add_line_item(01, 30502, 2013, 1, 0.00);
    orderman.add_line_item(02, 30502, 2014, 1, 0.00);

    orderman.add_order(30503, 104, '150201', '150202', '910 LBJ Freeway', 'Dallas', 'TX', '75234');
    orderman.add_line_item(01, 30503, 2020, 5, 0.02);
    orderman.add_line_item(02, 30503, 2027, 5, 0.02);
    orderman.add_line_item(03, 30503, 2021, 15, 0.05);
    orderman.add_line_item(04, 30503, 2022, 15, 0.05);

    orderman.add_order(30504, 101, '150201', '150206', '4490 Stevens Blvd', 'San Jose', 'CA', '95129');
    orderman.add_line_item(01, 30504, 2025, 20, 0.10);
    orderman.add_line_item(02, 30504, 2026, 20, 0.10);
    COMMIT;
END;
/

SET SERVEROUTPUT ON
CALL dbms_java.set_output(10000);
CALL orderman.total_orders();

select * from Orders;
select * from LineItems;
select * from StockItems;
select * from Customers;

QUIT;
