class Ai {
    public:
        //Abstract class.
        virtual void update(Actor *owner) = 0;
};

class PlayerAi : public Ai {
    public :
        void update(Actor *target);

    protected:
        bool moveOrAttack(Actor *target, int x, int y);
        void handleActionKey(Actor *player, int ascii);
        Actor *choseFromInventory(Actor *player);
};

class MonsterAi : public Ai {
    public:
        void update(Actor * target);
    
    protected:
        int move_count;
        void moveOrAttack(Actor *target, int x, int y);
};

class ConfusedMonsterAi : public Ai {
    public:
        ConfusedMonsterAi(int duration, Ai *old_ai);
        void update(Actor *target);

    protected:
        int duration;
        Ai *old_ai;
};
