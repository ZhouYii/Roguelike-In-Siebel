class Pickable : public Persistent {
    public:
        bool pick(Actor *item, Actor *owner);

        //Generally returning true from this function means the item is
        //consumed!
        virtual bool use(Actor *item, Actor *owner);
        void drop(Actor *item, Actor *owner);
        static Pickable *create(TCODZip &zip);

    protected:
        enum PickableType {
            HEALING_ENTITY, LIGHTNING_BOLT, FIREBALL, CONFUSE
        };
};

class HealingEntity : public Pickable {
    public:
        float amount;
        HealingEntity(float amount);
        bool use(Actor *item, Actor *owner);
        void load(TCODZip &zip);
        void save(TCODZip &zip);
};

class LightningBolt : public Pickable {
    public:
        float range, damage;
        LightningBolt(float range, float damage);
        bool use(Actor *item, Actor *user);    
        void load(TCODZip &zip);
        void save(TCODZip &zip);
};

class Fireball : public LightningBolt {
    public:
        Fireball(float range, float damage);
        bool use(Actor *item, Actor *user);
        void save(TCODZip &zip);
};

class Confuse : public Pickable {
    public:
        Confuse(int duration, float range);
        bool use(Actor* item, Actor* user);
        void load(TCODZip &zip);
        void save(TCODZip &zip);

    private:
        int duration;
        float range;
};

