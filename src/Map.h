#ifndef MAP_H
#define MAP_H

struct Tile {
    bool explored;
    Tile() : explored(false) {}
};

class Map : public Persistent {
    private:
        void checkCoordinateBounds(int &x1, int &y1);
        void verifyRoomCoords(int &x1, int &y1, int &x2, int &y2);

    public:
        int width, height;

        Map(int width, int height);
        ~Map();

        void init(bool load_actors);
        void addMonster(int x, int y);
        void addItem(int x, int y);
        bool isWall(int x, int y) const;
        bool canWalk(int x, int y) const;
        void render() const;
        bool inFov(int x, int y) const; //Field of View
        bool isExplored(int x, int y) const;
        void computeFov();
        void load(TCODZip &zip);
        void save(TCODZip &zip);


    protected:
        //Dynamically allocated to allow variable map size.
        Tile *tiles;
        TCODMap *map;
        friend class BspListener;
        long seed;
        TCODRandom *rng;
        
        void dig(int x1, int y1, int x2, int y2);
        void createRoom(bool first, int x1, int y1, int x2, int y2, bool load_actors);
};

#endif
