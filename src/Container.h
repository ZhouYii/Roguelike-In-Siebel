class Container {
    public : 
        int size; //Number of slots, 0 = limitless
        TCODList<Actor *> inventory;

        Container(int size);
        ~Container();
        bool add(Actor *item);
        void remove(Actor *element);
};
