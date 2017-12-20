static char BStore_C [] = "@(#) db $Header: http://belov.homenet.org/svn/trunk/base/store/BaseStore.C 232 2013-10-21 14:57:30Z db $";

#include <BaseStore.H>

/*
   $Log: not supported by cvs2svn $
   Revision 1.1  2000/10/16 21:45:48  db
 *** empty log message ***
 */

//============= BaseStorable members ==========
//BaseStorable::BaseStorable() : Bobj()
//{
//}

BaseStorable::~BaseStorable()
{
    cout << "~BaseStorable=" << this << endl;
}

//============= BaseStorable end ==============
//============= BaseStore members ==========

BaseStore::BaseStore()
{
    number = 0;
    storage = 0;
}

BaseStore::BaseStore(BaseStore const &other)
{
    copy(other);
}


BaseStore::~BaseStore()
{
    destroy();
}


BaseStore const &BaseStore::operator=(BaseStore const &other)
{
    cout << "BaseStore::operator=(" << other << ")" << endl;
    if (this != &other)
    {
        destroy();
        copy(other);
    }
    return (*this);
}

ostream & operator << (ostream& os, const BaseStore& s)
{
    os << s.ClassName() << ".nstored=" << s.number << ' '; 
    s.onPrint(os); 
    return os; 
};

void BaseStore::copy(BaseStore const &other)
{
    number = other.number;
    storage = new BaseStorable* [number];
    for (int i = 0; i < number; i++)
        storage [i] = other.storage [i]->Copy();
}

void BaseStore::destroy()
{
    for (register int i = 0; i < number; i++)
        delete storage [i];
    delete storage;
}

BaseStorable *BaseStore::Copy() const
{
    return (new BaseStore (*this));
}

void BaseStore::add(BaseStorable const &newobj)
{
    // reallocate storage array
    storage = (BaseStorable **) realloc(storage, (number + 1) * sizeof(BaseStorable *));
    // put duplicate of newobj in storage
    storage [number] = newobj.Copy();
    // increase number of obj in storage
    number++;
}

const BaseStorable& BaseStore::back(const BaseStorable& oldobj)
{
    for (register int i = 0; i < number; i++)
    {
        if ( *storage[i] == oldobj)
        {
            return *storage[i];
        }
    }
    cerr << "obj to restore=" << oldobj << ": NOTFOUND on store=" << *this << endl;
    return oldobj;
}

BaseStorable *BaseStore::get(int index)
{
    // check if index within range
    if (index < 0 || index >= number)
        return (0);
    // return address of stored object
    return (storage [index]);
}

int BaseStore::nstored() const
{
    return (number);
}

//============= BaseStore end ==========
