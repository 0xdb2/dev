#ifndef _BaseStore_H
#define _BaseStore_H

static char BaseStore_H [] = "@(#) db $Header: http://belov.homenet.org/svn/trunk/base/store/BaseStore.H 232 2013-10-21 14:57:30Z db $";

/*
   $Log: not supported by cvs2svn $
   Revision 1.1  2000/10/16 21:45:48  db
 *** empty log message ***

*/

// #include <BDef.H>
#include <BaseObj.H>

class BaseStorable: public BaseObj
{
    public:
        //BaseStorable() { index = 0; };
        ~BaseStorable();
        virtual BaseStorable * Copy() const = 0; // pure virtual
        virtual char const * ClassName() const { return("BaseStorable"); }
        //int getIndex() { return index; };
        //bool isStored() { return (index > 0); };
    //private:
        //int index;
}; 
// BaseStorable end

class BaseStore: public BaseStorable
{
    public:
        // destructors, constructor
        BaseStore();
        BaseStore(BaseStore const &other);
        virtual ~BaseStore();
        virtual char const * ClassName() const { return("BaseStore"); }

        // overloaded assignment
        BaseStore const &operator=(BaseStore const &other);

        // functionality to duplicate storages
        BaseStorable *Copy() const;

        // interface
        void add(const BaseStorable& newobj);
        const BaseStorable& back(const BaseStorable& oldobj);
        int nstored() const;
        BaseStorable *get(int index);
        friend ostream & operator << (ostream& os, const BaseStore&);
    private:
        // copy/destroy primitives
        void destroy();
        void copy(BaseStore const &other);

        // private data
        int number;
        BaseStorable **storage;
}; // End  BaseStore

#endif // _BaseStore_H
