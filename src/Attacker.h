class Attacker {
    public :
        float attack_points;

        Attacker(float power);
        void attack(Actor *attacker, Actor* target);
};
