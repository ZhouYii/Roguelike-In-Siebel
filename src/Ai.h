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
};

class MonsterAi : public Ai {
    public:
        void update(Actor * target);
    
    protected:
        int move_count;
        void moveOrAttack(Actor *target, int x, int y);
};
