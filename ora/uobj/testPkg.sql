create or replace package testPkg as
    procedure termout (context varchar2, message varchar2); 
    function myfunc (par varchar2) return varchar2;
end testPkg;
/
list
show errors
create or replace package body testPkg as 
    procedure termout (context varchar2, message varchar2) is 
    begin null; error here; end; 
    function myfunc (par varchar2) return varchar2 is
    begin null; end; 
end testPkg;
/
list
show errors
exit
