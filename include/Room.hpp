
#include <vector>

class Room {
public:
	Room::Room(int type);

	void Room::AddRoom(Room room);
private:
	std::vector<Room> childRoom;
};