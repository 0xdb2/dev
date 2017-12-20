#include "lambdaf.H"
#include <algorithm>

int main(void)
{
    cout << "##### main::exrc_entity ###### "<< endl;
    EntityContainer container;
    container.addEntity(new Entity(1, "Stone golem", "Monster", 500));
    container.addEntity(new Entity(2, "Player", "Human", 50));
    container.addEntity(new Entity(3, "Spider", "Monster", 5));
    container.addEntity(new Entity(4, "Bat", "Monster", 10));

    // insert code her e
    for (auto ent: container.get())
    {
        cout << *ent << endl;
    }

    auto search = [] (const Entity& e) -> bool { return e.gettype().compare(string("Human")) == 0; };
    vector<Entity*> humans = container.findMatchingEntities( search );

    for (auto human: humans)
    {
        cout << *human << endl;
    }

    cout << "##### main::exrc_mail ###### "<< endl;
    EmailProcessor processor;
    MessageSizeStore size_store;
    //processor.setHandlerFunc( &size_store.checkMessage ); // this won't work
    //function<bool (const string&)> checkfunc(&size_store.checkMessage);
    //processor.setHandlerFunc(checkfunc);
    processor.setHandlerFunc( [&size_store] (const string& message) { size_store.checkMessage( message ); } ); // ok
    processor.receiveMessage("aaa");
    processor.receiveMessage("bbbb");
    cout << "##### main::exrc_mail max_size=" << size_store.getMaxSize() << endl; // ##### main::exrc_mail max_size=4

    cout << "##### main::exrc_for_each ###### "<< endl;
    vector<int> v;
    v.push_back( 1 );
    v.push_back( 2 );
    //...
    for_each( v.begin(), v.end(), [] (int val)
    {
        cout << val << endl;
    } );

};

