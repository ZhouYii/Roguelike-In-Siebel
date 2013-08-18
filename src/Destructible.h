class Destructible 
{
    public:
        float max_hp;
        float hp;
        float defense; //damage = damage_raw - defence. must be pos.
        const char* corpse_name; //name once dead

        Destructible(float max_hp, float defense, const char * corpse_name);
        inline bool isDead() {return hp <= 0; };
        float calcDamage(Actor * target, float damage); //returns damage after defense
        virtual void die(Actor* target);
};

class MonsterDestructible : public Destructible 
{
    public:
        MonsterDestructible(float max_hp, float defense, const char *corpse_name);
        void die(Actor * owner);
};

class PlayerDestructible : public Destructible 
{
    public:
        PlayerDestructible(float max_hp, float defense, const char *corpse_name);
        void die(Actor * owner);
};
