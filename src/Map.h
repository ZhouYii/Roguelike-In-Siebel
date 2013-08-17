struct Tile {
    bool can_walk;
    Tile() : can_walk(true) {}
};

class Map {
    public:
        int width, height;

        Map(int width, int height);
        ~Map();
        bool isWall(int x, int y) const;
        void render() const;

    protected:

        //Dynamically allocated to allow variable map size.
        Tile *tiles;
        void setWall(int x, int y);
};

