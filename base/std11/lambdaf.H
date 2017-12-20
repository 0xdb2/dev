// $Header$
//

#include <iostream>
#include <string>
#include <vector>
#include <functional>

using namespace std;

class Entity
{
public:
    Entity(int i, string n, string t, int hp)
        : id(i)
        , name(n)
        , type(t)
        , alive(true)
        , hitpoints(hp)
    {
    }
    virtual ostream& onPrint(ostream& os) const { os << "ent=" << name << "/" << type << "/" << id; return os; };
    friend ostream& operator<<( ostream&, const Entity&);
    virtual ~Entity() { };

    const string& gettype(void) const { return type; }

private:
    int id;
    string name;
    string type;
    bool alive;
    int hitpoints;
};

ostream& operator<<( ostream& os, const Entity& ent) { return ent.onPrint(os); };

class EntityContainer
{
public:
    virtual ~EntityContainer()
    {
        for (auto e : entities)
        {
            delete e;
        }
    }

    void addEntity(Entity* entity)
    {
        entities.push_back(entity);
    }

    vector<Entity*> get(void) const { return entities; }

    template<typename Func>
    vector<Entity*> findMatchingEntities(Func func)
    {
        vector<Entity*> results;
        for ( auto ent: entities)
        {
            // rufe "func" auf um zu pruefen, ob die Suchkriterien passen
            if ( func( *ent ) )
            {
                results.push_back( ent );
            }
        }

        return results;
    }


private:
    vector<Entity*> entities;
};

class AddressBook
{
    public:
    // using a template allows us to ignore the differences between functors, function pointers
    // and lambda
//    template<typename Func>
//    std::vector<std::string> findMatchingAddresses (Func func)
//    {
//        std::vector<std::string> results;
//        for ( auto itr = _addresses.begin(), end = _addresses.end(); itr != end; ++itr )
//        {
//            // call the function passed into findMatchingAddresses and see if it matches
//            if ( func( *itr ) )
//            {
//                results.push_back( *itr );
//            }
//        }
//        return results;
//    }

    std::vector<string> findMatchingAddresses (function<bool (const string&)> func)
    {
        std::vector<string> results;
        for ( auto itr = _addresses.begin(), end = _addresses.end(); itr != end; ++itr )
        {
            // call the function passed into findMatchingAddresses and see if it matches
            if ( func( *itr ) )
            {
                results.push_back( *itr );
            }
        }
        return results;
    }

    private:
    std::vector<std::string> _addresses;
};

class EmailProcessor
{
public:
    void receiveMessage (const string& message)
    {
        if ( _handler_func )
        {
            _handler_func( message );
        }
        // other processing
    }

    void setHandlerFunc (function<void (const string&)> handler_func)
    {
        _handler_func = handler_func;
    }

private:
        std::function<void (const std::string&)> _handler_func;
};

class MessageSizeStore
{
    public:
    MessageSizeStore () : _max_size( 0 ) {}

    void checkMessage (const std::string& message )
    {
        const int size = message.length();
        if ( size > _max_size )
        {
            _max_size = size;
        }
    }
    int getMaxSize ()
    {
        return _max_size;
    }

private:
    int _max_size;
};


