class Destructible : public Persistent {
    public:
        float max_hp;
        float hp;
        float defense; //damage = damage_raw - defence. must be pos.
        char* corpse_name; //name once dead
        int xp;

        Destructible(float max_hp, float defense, const char * corpse_name, int xp);
        virtual ~Destructible();
        inline bool isDead() {return hp <= 0; };
        float calcDamage(Actor * target, float damage); //returns damage after defense
        virtual void die(Actor* target);
        float heal(float amount);
        void load(TCODZip &zip);
        void save(TCODZip &zip);
        static Destructible *create(TCODZip &zip);

    protected:
        enum DestructibleType {
            MONSTER, PLAYER
        };
};

class MonsterDestructible : public Destructible 
{
    public:
        MonsterDestructible(float max_hp, float defense, const char *corpse_name, int xp = 5);
        void save(TCODZip &zip);
        void die(Actor * owner);
};

class PlayerDestructible : public Destructible 
{
    public:
        PlayerDestructible(float max_hp, float defense, const char *corpse_name, int xp = 0);
        void save(TCODZip &zip);
        void die(Actor * owner);
};
