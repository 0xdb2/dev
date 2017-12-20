

CREATE FUNCTION oscar_quote RETURN VARCHAR2
AS LANGUAGE JAVA
NAME 'oscar.quote() return java.lang.String'
;
/

SELECT * FROM user_objects WHERE object_name = dbms_java.shortname('oscar');

VARIABLE theQuote VARCHAR2(50);
CALL oscar_quote() INTO :theQuote;
PRINT theQuote

VARIABLE theQuote VARCHAR2(50);
execute call oscar_quote() INTO :theQuote;
PRINT theQuote


SELECT * FROM employees;
SELECT * FROM jobs;

