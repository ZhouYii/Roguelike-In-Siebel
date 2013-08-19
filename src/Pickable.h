class Pickable {
    public:
        bool pick(Actor *item, Actor *owner);

        //Generally returning true from this function means the item is
        //consumed!
        virtual bool use(Actor *item, Actor *owner);
        void drop(Actor *item, Actor *owner);
};

class HealingEntity : public Pickable {
    public:
        float amount;
        HealingEntity(float amount);
        bool use(Actor *item, Actor *owner);
};

class LightningBolt : public Pickable {
    public:
        float range, damage;
        LightningBolt(float range, float damage);
        bool use(Actor *item, Actor *user);
};

class Fireball : public LightningBolt {
    public:
        Fireball(float range, float damage);
        bool use(Actor *item, Actor *user);
};

class Confuse : public Pickable {
    public:
        Confuse(int duration, float range);
        bool use(Actor* item, Actor* user);

    private:
        int duration;
        float range;
};

