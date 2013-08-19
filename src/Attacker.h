class Attacker : public Persistent {
    public :
        float attack_points;

        Attacker(float power);
        void attack(Actor *attacker, Actor* target);
        void load(TCODZip &zip);
        void save(TCODZip &save);
};
