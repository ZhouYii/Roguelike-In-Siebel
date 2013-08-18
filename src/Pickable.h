class Pickable {
    public:
        bool pick(Actor *item, Actor *owner);

        //Generally returning true from this function means the item is
        //consumed!
        virtual bool use(Actor *item, Actor *owner);
};

class HealingEntity : public Pickable {
    public:
        float amount;
        HealingEntity(float amount);
        bool use(Actor *item, Actor *owner);
};
