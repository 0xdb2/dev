// $Id: makefile 241 2013-11-08 14:18:08Z db $

#include <signal.h>

#include <BaseMap.H>

static EXCDEF(bad_logic);
static EXCDEF(unrealised_map);

typedef unsigned short flags_t;
enum flags {
    undefined  = 0x0000,
    created    = 0x0001,
    bounded    = 0x0002,
    listened   = 0x0004,
    connected  = 0x0008,
    faulty     = 0x8000
};

void signal_handler (int sig)
{
    cerr << TS << "signal_handler: sig=" << sig << endl;
    EXCRAISE("signal_handler", "unexpected signal");
}

int main (int argc, char *argv[])
{
    static const BaseStr _myName = argv[0] + BaseStr("::main"); 
    EXCINIT;

    EXCFUNC(myName);

    signal(SIGINT, signal_handler);
    signal(SIGALRM, signal_handler);

    BaseMap map;

    bool Action = true;
    while (Action)
    {

        EXCBLK(Action);
        LOG << "############## START ..." << endl;
        
        //map["key1"] = "val1";
        //map["key2"] = "val2";
        //const char* str = "key1=val1|key2=val2";
        BaseMap map = "key1=val1|key2=val2";
        map = "key3=val3|key4=val4";

        //for (BaseMapIter mapiter = map.begin(); mapiter != map.end(); mapiter++)
        //for (auto& pair : map)
        //{
        //    LOG << "pair=" << pair.first << "=" << pair.second << endl; 
        //} 
        LOG << map << endl; 

        //BaseStr mapstr = map;
        BaseStr mapstr(map);
        LOG << "mapstr=" << mapstr  << endl;

        map = "Key3=Val3|Key4=Val4|KeyNum=-009999abc";
        int num = stoi(map.at("KeyNum"));
        //LOG << "map.size=" << map.size() << ",mappedstr=" << BaseStr(map) << "num=" << num << endl;
        LOG << map << " num=" << num << endl;

        Action = false;
        //EXCRAISE(bad_logic, "endless loop");

        EXCEPTION;
        WHEN bad_logic THEN
            ERR << "catched exc=" << BaseExc::ActExc() << '/' << (void*)BaseExc::ActExc() << '(' << exc.what() << ')' << endl;
            Action = false;
            //reraise
            BaseExc::ActExc() = unrealised_map;
            EXCRERAISE;
        END;
        EXCEND;
    }

    EXCFUNCEND;

    EXCTERM;
    LOG << "ENDOK" << endl;
    exit (0);
} // END -- :!maptst; echo "$_=$?"

