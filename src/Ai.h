class Ai : public Persistent {
    public:
        //Abstract class.
        virtual void update(Actor *owner) = 0;
        static Ai *create (TCODZip &zip);

    protected :
        enum AiType {
            MONSTER,CONFUSED_MONSTER,PLAYER
        };
};

class PlayerAi : public Ai {
    public :
        int xp_level;
        PlayerAi();
        int getNextLevelXp();
        void update(Actor *target);
        void load(TCODZip &zip);
        void save(TCODZip &zip);

    protected:
        bool moveOrAttack(Actor *target, int x, int y);
        void handleActionKey(Actor *player, int ascii);
        Actor *choseFromInventory(Actor *player);
};

class MonsterAi : public Ai {
    public:
        void update(Actor * target);
        void load(TCODZip &zip);
        void save(TCODZip &zip);
    
    protected:
        int move_count;
        void moveOrAttack(Actor *target, int x, int y);
};

class ConfusedMonsterAi : public Ai {
    public:
        ConfusedMonsterAi(int duration, Ai *old_ai);
        void update(Actor *target);
        void load(TCODZip &zip);
        void save(TCODZip &zip);

    protected:
        int duration;
        Ai *old_ai;
};
