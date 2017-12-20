-- publish
-- :!sqlplus db/myoradb\$db @orderman

--################################################################### create tables

DROP TABLE LineItems;
DROP TABLE StockItems;
DROP TABLE Orders;
DROP TABLE Customers CASCADE CONSTRAINTS;

CREATE TABLE Customers (
    CustNo NUMBER(3) NOT NULL,
    CustName VARCHAR2(30) NOT NULL,
    Street VARCHAR2(20) NOT NULL,
    City VARCHAR2(20) NOT NULL,
    State CHAR(2) NOT NULL,
    Zip VARCHAR2(10) NOT NULL,
    Phone VARCHAR2(12),
    PRIMARY KEY (CustNo)
);

CREATE TABLE Orders (
    OrderNo NUMBER(5),
    Custno NUMBER(3) REFERENCES Customers,
    OrderDate DATE,
    ShipDate DATE,
    ToStreet VARCHAR2(20),
    ToCity VARCHAR2(20),
    ToState CHAR(2),
    ToZip VARCHAR2(10),
    PRIMARY KEY (OrderNo)
);

CREATE TABLE StockItems (
    StockNo NUMBER(4) PRIMARY KEY,
    Description VARCHAR2(20),
    Price NUMBER(6,2)
);

CREATE TABLE LineItems (
    LineNo NUMBER(2),
    OrderNo NUMBER(5) REFERENCES Orders,
    StockNo NUMBER(4) REFERENCES StockItems,
    Quantity NUMBER(2),
    Discount NUMBER(4,2),
    PRIMARY KEY (LineNo, OrderNo)
);

--################################################################### publishing

/*
:DB db/myoradb\\$db

VARIABLE CompiledCnt NUMBER;
CALL DBMS_JAVA.compile_class('orderman') INTO :CompiledCnt;
PRINT CompiledCnt;

    SELECT * FROM user_objects WHERE object_name = dbms_java.shortname('orderman') OR object_name = UPPER('orderman');

    SELECT cast(object_name as varchar2(20)), object_type FROM user_objects;

    COL object_name format a30
    COL object_type format a15
    SELECT OBJECT_NAME,	OBJECT_ID, OBJECT_TYPE,	CREATED, LAST_DDL_TIME, TIMESTAMP, STATUS, NAMESPACE
    FROM user_objects WHERE object_type LIKE 'JAVA%' ORDER BY object_type, object_name;

    select * from javasnm;

*/

CREATE OR REPLACE PACKAGE orderman AS
PROCEDURE init(logfile VARCHAR2);
PROCEDURE add_customer (cust_no NUMBER, cust_name VARCHAR2,
    street VARCHAR2, city VARCHAR2, state CHAR, zip_code VARCHAR2,
    phone_no VARCHAR2);
PROCEDURE add_stock_item (stock_no NUMBER, description VARCHAR2,
    price NUMBER);
PROCEDURE add_order (order_no NUMBER, cust_no NUMBER,
    order_date VARCHAR2, ship_date VARCHAR2, to_street VARCHAR2,
    to_city VARCHAR2, to_state CHAR, to_zip_code VARCHAR2);
PROCEDURE add_line_item (line_no NUMBER, order_no NUMBER,
    stock_no NUMBER, quantity NUMBER, discount NUMBER);
PROCEDURE total_orders;
PROCEDURE check_stock_item (stock_no NUMBER);
PROCEDURE change_quantity (new_qty NUMBER, order_no NUMBER,
stock_no NUMBER);
PROCEDURE delete_order (order_no NUMBER);
END orderman;
/
SHOW ERRORS;

CREATE OR REPLACE PACKAGE BODY orderman AS
PROCEDURE init(logfile VARCHAR2) AS LANGUAGE JAVA
--NAME 'orderman.init(java.lang.String)';
--NAME 'core.log.myLogger.init(java.lang.String)';
NAME 'orderman.init(java.lang.String)';
PROCEDURE add_customer (cust_no NUMBER, cust_name VARCHAR2,
    street VARCHAR2, city VARCHAR2, state CHAR, zip_code VARCHAR2,
    phone_no VARCHAR2) AS LANGUAGE JAVA
NAME 'orderman.addCustomer(int, java.lang.String,
    java.lang.String, java.lang.String, java.lang.String,
    java.lang.String, java.lang.String)';

PROCEDURE add_stock_item (stock_no NUMBER, description VARCHAR2,
price NUMBER) AS LANGUAGE JAVA
NAME 'orderman.addStockItem(int, java.lang.String, float)';

PROCEDURE add_order (order_no NUMBER, cust_no NUMBER,
order_date VARCHAR2, ship_date VARCHAR2, to_street VARCHAR2,
to_city VARCHAR2, to_state CHAR, to_zip_code VARCHAR2)
AS LANGUAGE JAVA
NAME 'orderman.addOrder(int, int, java.lang.String,
java.lang.String, java.lang.String, java.lang.String,
java.lang.String, java.lang.String)';

PROCEDURE add_line_item (line_no NUMBER, order_no NUMBER,
stock_no NUMBER, quantity NUMBER, discount NUMBER)
AS LANGUAGE JAVA
NAME 'orderman.addLineItem(int, int, int, int, float)';

PROCEDURE total_orders
AS LANGUAGE JAVA
NAME 'orderman.totalOrders()';

PROCEDURE check_stock_item (stock_no NUMBER)
AS LANGUAGE JAVA
NAME 'orderman.checkStockItem(int)';

PROCEDURE change_quantity (new_qty NUMBER, order_no NUMBER,
    stock_no NUMBER) AS LANGUAGE JAVA
NAME 'orderman.changeQuantity(int, int, int)';

PROCEDURE delete_order (order_no NUMBER)
AS LANGUAGE JAVA
NAME 'orderman.deleteOrder(int)';
END orderman;
/
SHOW ERRORS;

QUIT;
