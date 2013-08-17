#ifndef MAP_H
#define MAP_H

struct Tile {
    bool can_walk;
    Tile() : can_walk(false) {}
};

class Map {
    private:
        void checkCoordinateBounds(int &x1, int &y1);
        void verifyRoomCoords(int &x1, int &y1, int &x2, int &y2);

    public:
        int width, height;

        Map(int width, int height);
        ~Map();
        bool isWall(int x, int y) const;
        void render() const;

    protected:
        //Dynamically allocated to allow variable map size.
        Tile *tiles;
        friend class BspListener;
        
        void dig(int x1, int y1, int x2, int y2);
        void createRoom(bool first, int x1, int y1, int x2, int y2);
};

#endif
