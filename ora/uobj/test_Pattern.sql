/*
$Header$
*/

/*
    SET LINESIZE 32000;
    SELECT * FROM ALL_OBJECTS WHERE ROWNUM=1;
    SELECT * FROM ALL_OBJECTS WHERE OWNER = 'HR' AND OBJECT_TYPE LIKE 'TYPE%';

*/

--######################################### CHAIN-OF-RESPONSIBILITY

--DROP TYPE ;

CREATE OR REPLACE TYPE type_HndlBase FORCE AS OBJECT (
    HndlName VARCHAR2(40),
    Msg VARCHAR2(4000),
    NOT INSTANTIABLE MEMBER PROCEDURE push (Hndl IN type_HndlBase),
    NOT INSTANTIABLE MEMBER FUNCTION pop RETURN type_HndlBase,
    NOT INSTANTIABLE MEMBER PROCEDURE setNext (Hndl IN type_HndlBase),
    NOT INSTANTIABLE MEMBER PROCEDURE Handle(Str IN VARCHAR2),
    MAP MEMBER FUNCTION get RETURN VARCHAR2
) NOT FINAL NOT INSTANTIABLE;
/

CREATE OR REPLACE TYPE BODY type_HndlBase AS
    MAP MEMBER FUNCTION get RETURN VARCHAR2 IS
    BEGIN
        RETURN HndlName;
    END get;
END;
/

CREATE OR REPLACE TYPE type_Hndl FORCE UNDER type_HndlBase(
    NextHndl type_HndlBase,
    OVERRIDING MEMBER PROCEDURE push (Hndl IN type_HndlBase),
    OVERRIDING MEMBER FUNCTION pop RETURN type_HndlBase,
    OVERRIDING MEMBER PROCEDURE setNext (Hndl IN type_HndlBase),
    OVERRIDING MEMBER PROCEDURE Handle(Str IN VARCHAR2)
) NOT FINAL NOT INSTANTIABLE;
/

CREATE OR REPLACE TYPE BODY type_Hndl AS

    OVERRIDING MEMBER PROCEDURE push (Hndl IN type_HndlBase) IS
    BEGIN
        IF NextHndl IS NULL THEN
            NextHndl := Hndl;
        ELSE
            NextHndl.push(Hndl);
        END IF;
    END push;

    OVERRIDING MEMBER FUNCTION pop RETURN type_HndlBase IS
        NextHndl type_HndlBase;
    BEGIN
        IF NextHndl IS NULL THEN
            RETURN SELF;
        ELSE
            pop.NextHndl := NextHndl.pop();
            IF pop.NextHndl = NextHndl THEN
                NextHndl := NULL;
                RETURN pop.NextHndl;
            ELSE
                RETURN NextHndl;
            END IF;
        END IF;
    END pop;

    OVERRIDING MEMBER PROCEDURE setNext (Hndl IN type_HndlBase) IS
    BEGIN
        NextHndl := Hndl;
    END setNext;

    OVERRIDING MEMBER PROCEDURE Handle(Str IN VARCHAR2) IS
    BEGIN
        IF NextHndl IS NOT NULL THEN
            NextHndl.Handle(Str);
        ELSE
            RAISE_APPLICATION_ERROR(-20000, 'last handler ' || HndlName || ': unmatched Str=' || Str);
        END IF;
    END Handle;
END;
/

CREATE OR REPLACE TYPE type_HndlExpr FORCE UNDER type_Hndl (
    Pattern VARCHAR2(40),
    CONSTRUCTOR FUNCTION type_HndlExpr(Name IN VARCHAR2, Pattern IN VARCHAR2) RETURN SELF AS RESULT,
    OVERRIDING MEMBER PROCEDURE Handle(Str IN VARCHAR2)
)
/

CREATE OR REPLACE TYPE BODY type_HndlExpr AS

    CONSTRUCTOR FUNCTION type_HndlExpr(Name IN VARCHAR2, Pattern IN VARCHAR2) RETURN SELF AS RESULT IS
    BEGIN
        SELF.HndlName := Name;
        SELF.Pattern := Pattern;
        SELF.Msg  := 'undefined';
        RETURN;
    END type_HndlExpr;

    OVERRIDING MEMBER PROCEDURE Handle (Str IN VARCHAR2) IS
    BEGIN
        IF REGEXP_COUNT(Str, Pattern) > 0 THEN
            Msg := HndlName || ' matched Str=' || Str || ' Pattern=' || Pattern || ': SubStr=' || REGEXP_SUBSTR(Str, Pattern);
        ELSE
            Msg := HndlName || ' unmatch Str=' || Str || ' Pattern=' || Pattern;
            (SELF AS type_Hndl).Handle(Str);
            Msg := Msg || '|' || NextHndl.Msg;
        END IF;
    END Handle;
END;
/

DECLARE
    root type_HndlExpr;
    chain type_HndlExpr;
    trc_hndlr type_TraceHndlr := type_TraceHndlr('PAT_ChOfResp');
    trc type_Trace := type_Trace('');
BEGIN
    root := type_HndlExpr('root', 'root');
    root.Handle('root');
    trc.Msg('BEG Msg=' || root.msg);
    chain := type_HndlExpr('start chain', 'chain');
    chain.push(type_HndlExpr('pattern1', '^[[:alpha:]]+'));
    chain.push(type_HndlExpr('pattern2', '^[[:digit:]]+'));
    chain.push(type_HndlExpr('pattern3', '.*'));
    root.setNext(chain);
    root.Handle('abc');
    trc.Msg('END Msg=' || root.msg);
    root.Handle('123');
    trc.Msg('END Msg=' || root.msg);
    root.Handle('zzz');
    trc.Msg('END Msg=' || root.msg);
    root.Handle('123 abc ');
    trc.Msg('END Msg=' || root.msg);
    root.Handle('+++');
    trc.Msg('END Msg=' || root.msg);
END;
/

QUIT;
--######################################### COMMAND

CREATE OR REPLACE TYPE type_Reciever FORCE AS OBJECT (
    Msg VARCHAR2(1999),
    Result VARCHAR2(40),
    --NOT INSTANTIABLE MEMBER FUNCTION Action(Msg IN OUT VARCHAR2) RETURN VARCHAR2
    NOT INSTANTIABLE MEMBER FUNCTION Action(SELF IN OUT type_Reciever) RETURN VARCHAR2
) NOT FINAL NOT INSTANTIABLE;
/

CREATE OR REPLACE TYPE type_Command FORCE AS OBJECT (
    Reciever type_Reciever,
    NOT INSTANTIABLE MEMBER FUNCTION Execute(SELF IN OUT type_Command) RETURN VARCHAR2,
    --NOT INSTANTIABLE MEMBER FUNCTION Execute RETURN VARCHAR2,
    MEMBER FUNCTION getMsg RETURN VARCHAR2
    --MEMBER PROCEDURE setReciever(type_Reciever)
) NOT FINAL NOT INSTANTIABLE;
/

CREATE OR REPLACE TYPE BODY type_Command AS
    MEMBER FUNCTION getMsg RETURN VARCHAR2 IS
    BEGIN
        RETURN Reciever.Msg;
    END getMsg;
END;
/

CREATE OR REPLACE TYPE type_MyCmd UNDER type_Command (
    --OVERRIDING MEMBER FUNCTION Execute RETURN VARCHAR2
    OVERRIDING MEMBER FUNCTION Execute(SELF IN OUT type_MyCmd) RETURN VARCHAR2
    --MEMBER PROCEDURE setReciever(type_Reciever)
);
/

CREATE OR REPLACE TYPE BODY type_MyCmd AS
    --OVERRIDING MEMBER FUNCTION Execute RETURN VARCHAR2 IS
    OVERRIDING MEMBER FUNCTION Execute(SELF IN OUT type_MyCmd) RETURN VARCHAR2 IS
        Ret VARCHAR2(40);
    BEGIN
        Ret := Reciever.Action();
        --RETURN Ret || '|MyCmd.Msg=' || Msg;
        RETURN Ret;
    END Execute;
END;
/

CREATE OR REPLACE TYPE type_MyRec UNDER type_Reciever (
    --OVERRIDING MEMBER FUNCTION Action(Msg IN OUT VARCHAR2) RETURN VARCHAR2
    OVERRIDING MEMBER FUNCTION Action(SELF IN OUT type_MyRec) RETURN VARCHAR2
);
/

CREATE OR REPLACE TYPE BODY type_MyRec AS
    OVERRIDING MEMBER FUNCTION Action(SELF IN OUT type_MyRec) RETURN VARCHAR2 IS
    BEGIN
        Msg := 'do something - implement';
        --RETURN 'OK' || '|' || Msg;
        RETURN 'OK';
        --DoIt.Invoke()=OK|doit|do something - implement: Msg=doit
    END Action;
END;
/

--CREATE OR REPLACE TYPE type_CommandTab AS TABLE OF type_Command;
--/

CREATE OR REPLACE TYPE type_Invoker AS OBJECT (
    Command type_Command,
    CONSTRUCTOR FUNCTION type_Invoker(Command IN type_Command) RETURN SELF AS RESULT,
    MEMBER PROCEDURE setCommand(Cmd type_Command),
    MEMBER FUNCTION getMsg RETURN VARCHAR2,
    MEMBER FUNCTION Invoke(SELF IN OUT type_Invoker) RETURN VARCHAR2
) NOT FINAL;
/

CREATE OR REPLACE TYPE BODY type_Invoker AS
    CONSTRUCTOR FUNCTION type_Invoker(Command IN type_Command) RETURN SELF AS RESULT IS
    BEGIN
        SELF.Command := Command;
        RETURN;
    END type_Invoker;

    MEMBER PROCEDURE setCommand(Cmd type_Command) IS
    BEGIN
        Command := Cmd;
    END setCommand;

    MEMBER FUNCTION getMsg RETURN VARCHAR2 IS
    BEGIN
        RETURN Command.getMsg();
    END getMsg;

    MEMBER FUNCTION Invoke(SELF IN OUT type_Invoker) RETURN VARCHAR2 IS
    BEGIN
        RETURN Command.Execute();
    END Invoke;
END;
/

DECLARE
    Ret VARCHAR2(40);
    Msg VARCHAR2(1999);
    --MyRec type_MyRec := type_MyRec('doit', NULL);
    --MyCmd type_MyCmd := type_MyCmd(MyRec);
    --DoIt type_Invoker := type_Invoker(MyCmd);
    DoIt type_Invoker :=
        type_Invoker(
            type_MyCmd(
                type_MyRec('doit', NULL)
            )
        );
    trc type_Trace := type_Trace('');
BEGIN
    Ret := DoIt.Invoke();
    trc.Msg('DoIt.Invoke()=' || Ret || '|Msg=' || DoIt.getMsg());
    --       DoIt.Invoke()=OK|Msg=do something - implement
END;
/
LIST

QUIT;
--######################################### DECORATOR

DROP TYPE type_Decorator;
DROP TYPE type_Core;
DROP TYPE type_Interface;

CREATE OR REPLACE TYPE type_Interface AS OBJECT (
    Name VARCHAR2(40),
    Rate NUMBER,

    NOT INSTANTIABLE MEMBER FUNCTION getName RETURN VARCHAR2,
    NOT INSTANTIABLE MEMBER FUNCTION getRate RETURN NUMBER,
    CONSTRUCTOR FUNCTION type_Interface RETURN SELF AS RESULT
) NOT FINAL NOT INSTANTIABLE;
/

CREATE OR REPLACE TYPE BODY type_Interface AS

    CONSTRUCTOR FUNCTION type_Interface RETURN SELF AS RESULT IS
    BEGIN
        SELF.Name := 'UNDEF';
        SELF.Rate := 0;
        RETURN;
    END type_Interface;
END;
/

CREATE OR REPLACE TYPE type_Core UNDER type_Interface (
    CONSTRUCTOR FUNCTION type_Core (Name IN VARCHAR2, Rate IN NUMBER) RETURN SELF AS RESULT,
    OVERRIDING MEMBER FUNCTION getName RETURN VARCHAR2,
    OVERRIDING MEMBER FUNCTION getRate RETURN NUMBER
) FINAL;
/

CREATE OR REPLACE TYPE BODY type_Core IS
    CONSTRUCTOR FUNCTION type_Core(Name IN VARCHAR2, Rate IN NUMBER) RETURN SELF AS RESULT IS
    BEGIN
        SELF.Name := Name;
        SELF.Rate := Rate;
        RETURN;
    END type_Core;

    OVERRIDING MEMBER FUNCTION getName RETURN VARCHAR2 IS
    BEGIN
        RETURN Name;
    END getName;

    OVERRIDING MEMBER FUNCTION getRate RETURN NUMBER IS
    BEGIN
        RETURN Rate;
    END getRate;
END;
/

CREATE OR REPLACE TYPE type_Decorator FORCE UNDER type_Interface (
    Wrappee type_Interface,
    OVERRIDING MEMBER FUNCTION getName RETURN VARCHAR2,
    OVERRIDING MEMBER FUNCTION getRate RETURN NUMBER
) NOT FINAL NOT INSTANTIABLE;
/

CREATE OR REPLACE TYPE BODY type_Decorator AS

    OVERRIDING MEMBER FUNCTION getName RETURN VARCHAR2 IS
    BEGIN
        RETURN Wrappee.getName() || '*';
    END getName;

    OVERRIDING MEMBER FUNCTION getRate RETURN NUMBER IS
    BEGIN
        RETURN Wrappee.getRate();
    END getRate;
END;
/

CREATE OR REPLACE TYPE type_Wrapper UNDER type_Decorator (
    OVERRIDING MEMBER FUNCTION getName RETURN VARCHAR2,
    OVERRIDING MEMBER FUNCTION getRate RETURN NUMBER,
    CONSTRUCTOR FUNCTION type_Wrapper(Wrappee IN type_Interface, Name IN VARCHAR2, Rate IN NUMBER) RETURN SELF AS RESULT
) NOT FINAL;
/

CREATE OR REPLACE TYPE BODY type_Wrapper AS
    OVERRIDING MEMBER FUNCTION getName RETURN VARCHAR2 IS
        myName VARCHAR2(40) := Name || TO_CHAR(Rate);
    BEGIN
        --RETURN myName || '|' || Wrappee.getName || '|' || myName;
        RETURN myName || '|' || (SELF AS type_Decorator).getName || '|' || myName;
    END getName;

    OVERRIDING MEMBER FUNCTION getRate RETURN NUMBER IS
    BEGIN
        --RETURN Wrappee.getRate + Rate;
        RETURN (SELF AS type_Decorator).getRate + Rate;
    END getRate;

    CONSTRUCTOR FUNCTION type_Wrapper(Wrappee IN type_Interface, Name IN VARCHAR2, Rate IN NUMBER) RETURN SELF AS RESULT IS
    BEGIN
        SELF.Wrappee := Wrappee;
        SELF.Name := Name;
        SELF.Rate := Rate;
        RETURN;
    END ;

END;
/

DECLARE
    trc type_Trace := type_Trace('');
    Inst type_Interface;
BEGIN
    Inst := type_Core('core', 100);
    trc.Msg('Name=' || Inst.getName || ',Rate=' || Inst.getRate);
    Inst := type_Wrapper(Inst , 'Wrap', 20);
    trc.Msg('Name=' || Inst.getName || ',Rate=' || Inst.getRate);
    Inst := type_Wrapper(Inst , 'Wrap', 30);
    trc.Msg('Name=' || Inst.getName || ',Rate=' || Inst.getRate);
END;
/
LIST

QUIT;
/*
    http://blog.serpland.com/oracle/decorator-design-pattern-in-oracle-plsql-using-object-type-with-constructor
*/

drop type sugar_o;
drop type cream_o;
drop type additions_decorator_o;
drop type espresso_o;
drop type tea_o;
drop type drink_o;

------------------------------------------------------
-- drink_o [abstract]
------------------------------------------------------
create or replace type drink_o as object (
    description varchar2(200)
    , member function getDescription return varchar2
    , member function price return number
    , constructor function drink_o return self as result
) not final not instantiable;
/

create or replace type body drink_o as
    -- Constructor
    constructor function drink_o return self as result
    is
    begin
        self.description := '--undefined--';
        return;
    end;

    -- Setters / Getters
    member function price return number is
    begin
        --return self.price();
        -- recursive call of the member function price()
        -- es gibt ja kein price Attribut !!!
        return 0;
    end;

    member function getDescription return varchar2 is
    begin
        return self.description;
    end;

end;
/

------------------------------------------------------
-- espresso_o [instantiable] concrete drinking object
------------------------------------------------------
create or replace type espresso_o under drink_o (
    OVERRIDING member function price return number
    , constructor function espresso_o return self as result
) final instantiable;
/

create or replace type body espresso_o as
    -- Constructor
    constructor function espresso_o return self as result is
    begin
        self.description := 'Espresso';
        return;
        -- dont forget to RETURN !
        -- Otherwise:
        -- Error at line 34
        -- ORA-06503: PL/SQL: Function returned without value
        -- ORA-06512: at "ESPRESSO_O", line 6
        -- ORA-06512: at line 4
    end;

    -- specific price for Espresso! Costs $ 1.-
    OVERRIDING member function price return number is
    begin
        return 1;
    end;
end;
/

------------------------------------------------------
-- tea_o [instantiable] concrete drinking object
------------------------------------------------------
create or replace type tea_o under drink_o (
    OVERRIDING member function price return number,
    constructor function tea_o return self as result
) final instantiable;
/

create or replace type body tea_o as
    -- Constructor
    constructor function tea_o return self as result is
    begin
        self.description := 'Tea';
        return;
    end;

    -- specific price for Tea! Costs $ 3.- (expensive!)
    OVERRIDING member function price return number
    is
    begin
        return 3;
    end;
end;
/

------------------------------------------------------
-- additions_decorator_o [not instantiable]
------------------------------------------------------
create or replace type additions_decorator_o under drink_o (
    drink drink_o,
    OVERRIDING member function getDescription return varchar2
) not final not instantiable;
/

create or replace type body additions_decorator_o as
    -- private methods (overriding the drink_o class!)
    OVERRIDING member function getDescription return varchar2 is
    begin
        return '---decorated---';
    end;
end;
/

------------------------------------------------------
-- sugar_o [instantiable] concrete additions object
------------------------------------------------------
create or replace type sugar_o under additions_decorator_o (
    --drink drink_o,
    OVERRIDING member function getDescription return varchar2,
    OVERRIDING member function price return number,
    constructor function sugar_o(pdrink drink_o) return self as result
) final instantiable;
/

create or replace type body sugar_o as
    -- Constructor
    constructor function sugar_o(pdrink drink_o) return self as result is
    begin
        self.drink := pdrink;
        return;
    end;

    -- private methods
    OVERRIDING member function getDescription return varchar2 is
    begin
        return drink.getDescription ||', Sugar';
    end;

    -- specific price for sugar! Costs $ 0.20
    OVERRIDING member function price return number is
    begin
        return drink.price + 0.20;
    end;
end;
/

------------------------------------------------------
-- cream_o [instantiable] concrete additions object
------------------------------------------------------
create or replace type cream_o under additions_decorator_o (
    --drink drink_o,
    OVERRIDING member function getDescription return varchar2,
    OVERRIDING member function price return number,
    constructor function cream_o(pdrink drink_o) return self as result
) final instantiable;
/

create or replace type body cream_o as
    -- Constructor
    constructor function cream_o(pdrink drink_o) return self as result is
    begin
        self.drink := pdrink;
        return;
    end;

    -- private methods
    OVERRIDING member function getDescription return varchar2 is
    begin
        return drink.getDescription ||', Cream';
    end;

    -- specific price for cream! Costs $ 0.75
    OVERRIDING member function price return number is
    begin
        return drink.price + 0.75;
    end;
end;
/

------------------------------------------------------
-- Testcase on drink
------------------------------------------------------
declare
-- declaring two kind of drinks
    aCoffee drink_o;
    aTea drink_o;
    trc type_Trace := type_Trace();
begin
    trc.msg(chr(10) ||'-- start with a Espresso --');
    --
    -- lets create a warm Espresso
    aCoffee := espresso_o;
    trc.Msg('descr= ' || aCoffee.getDescription);
    trc.Msg('price= ' || aCoffee.price);
    --
    -- lets add sugar to our Espresso
    aCoffee := sugar_o(aCoffee);
    trc.Msg('descr= ' || aCoffee.getDescription);
    trc.Msg('price= ' || aCoffee.price);
    --
    -- also lets add some cream to our Espresso, too
    aCoffee := cream_o(aCoffee);
    trc.Msg('descr= ' || aCoffee.getDescription);
    trc.Msg('price= ' || aCoffee.price);

    trc.Msg(chr(10) ||'-- start with a Tea --');
    --
    -- lets create a warm Tea on a swiss mountain lodge
    aTea := tea_o;
    trc.Msg('descr= ' || aTea.getDescription);
    trc.Msg('price= ' || aTea.price);
    --
    -- lets add sugar to our Tea
    aTea := sugar_o(aTea);
    trc.Msg('descr= ' || aTea.getDescription);
    trc.Msg('price= ' || aTea.price);
    --
    -- also lets add some cream to our Tea, too
    aTea := cream_o(aTea);
    trc.Msg('descr= ' || aTea.getDescription);
    trc.Msg('price= ' || aTea.price);

end;
/

/*
-- start with a Espresso --
descr= Espresso
price= 1
descr= Espresso, Sugar
price= 1,2
descr= Espresso, Sugar, Cream
price= 1,95

-- start with a Tea --
descr= Tea
price= 3
descr= Tea, Sugar
price= 3,2
descr= Tea, Sugar, Cream
price= 3,95

*/
