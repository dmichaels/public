// --------------------------------------------------------------------------

#include <stdio.h>

// --------------------------------------------------------------------------

#define NullPtr				0
#define True				1
#define False				0

// --------------------------------------------------------------------------

typedef int				Boolean;
typedef unsigned long			ulong;

typedef ulong				RoomId;
typedef class RoomList *		RoomListPtr;
typedef class Room *			RoomPtr;
typedef class Maze *			MazePtr;

// --------------------------------------------------------------------------

const RoomId				NullRoomId = 0;

// --------------------------------------------------------------------------

class Room {

    public:
					 Room (RoomId,
					       Boolean is_implicit = False);
					 Room (RoomId,
					       RoomListPtr doors_to_rooms,
					       Boolean is_implicit = False);
					~Room ();

	RoomId				 id () const;
	Boolean				 implicit () const;
	void				 implicit (Boolean);

	void				 add_door_to (RoomId,
						      Boolean = False);
	void				 add_door_to (RoomPtr);
	Boolean				 contains_door_to (RoomId) const;
	ulong				 number_of_doors () const;

	DoorPtr				 doors () const;

	void				 dump () const;

	static Boolean			 ValidId (RoomId);
	static const RoomId		 NullId;

    private:

	void				 doors (RoomListPtr);
	void				 id (RoomId);

    private:

	RoomId				_id;
	DoorPtr				_doors;
	RoomPtr				_next;
};

// --------------------------------------------------------------------------

class Door {

    public:
				 Door (RoomId from, RoomId to);
				~Door ();

	RoomId			from_room () const;
	RoomId			to_room () const;
	Boolean			implicit () const;

	void			from_room (RoomId);
	void			to_room (RoomId);
	void			implicit (Boolean);

    private:

	RoomId			_roomA;
	RoomId			_roomB;
	Boolean			_implicitA : 1;
	Boolean			_implicitB : 1;

};

class DoorList {

    public:
				 DoorList ();
    				~DoorList ();
    private:

	class DoorItem {

	    public:
				 DoorItem ();

	    private:

		DoorPtr		_door;
		DoorItem *	_next;

	};

    private:
};

// --------------------------------------------------------------------------

DoorList::DoorItem::DoorItem ()
{
	_door = NullPtr;
	_next = NullPtr;
}

DoorList::DoorItem::~DoorItem ()
{
	_door = NullPtr;
	_next = NullPtr;
}

// --------------------------------------------------------------------------

class RoomList {

    public:
					 RoomList ();
					~RoomList ();

	RoomPtr				 add (RoomId, Boolean = False);
	void				 add (RoomPtr);
	Boolean				 contains (RoomId) const;
	RoomPtr				 find (RoomId) const;
	RoomPtr				 find_or_add (RoomId,
						      Boolean = False);
	ulong				 number_of_rooms () const;
	RoomPtr				 first () const;
	void				 dump () const;

    private:

	typedef class RoomListItem *	RoomListItemPtr;

	class RoomListItem {

	    public:
					 RoomListItem (RoomId,
						       Boolean = False);
					 RoomListItem (RoomPtr);
					~RoomListItem ();
		RoomPtr			 room () const;
		RoomId			 id () const;
		ulong			 number_of_rooms () const;
		void			 dump () const;

	    private:

		RoomListItemPtr		 _next_room;
		RoomPtr			 _room;

	    friend class		 RoomList;
	};

    private:

	ulong				_number_of_rooms;
	RoomListItemPtr 		_rooms;
};

// --------------------------------------------------------------------------

class Maze {

    public:
				 Maze (RoomId = 1);
				~Maze ();

	void			 add_door_to (RoomId,
					      Boolean is_implicit = False);
	void			 add_door_to (RoomPtr);
	RoomPtr			 room (RoomId, Boolean is_implicit = False);

	void			 dump () const;

    private:

	RoomPtr			_start_room;
	RoomListPtr		_all_rooms;
};

// --------------------------------------------------------------------------

Room::Room (RoomId room_id, Boolean is_implicit)
{
	if (!Room::ValidId (room_id))
		id (NullRoomId);
	else	id (room_id);
	doors (NullPtr);
	implicit (is_implicit);

}

Room::Room (RoomId room_id, RoomListPtr doors_to_rooms, Boolean is_implicit)
{
	if (!Room::ValidId (room_id))
		id (NullRoomId);
	else	id (room_id);
	doors (doors_to_rooms);
	implicit (is_implicit);

}

Room::~Room ()
{
	if (doors () != NullPtr)
		delete doors ();
}

RoomId
Room::id () const
{
	return (_id);
}

void
Room::id (RoomId room_id)
{
	_id = room_id;
}

Boolean
Room::implicit () const
{
	return (_implicit);
}

void
Room::implicit (Boolean b)
{
	_implicit = b;
}

RoomListPtr
Room::doors () const
{
	return (_doors);
}

void
Room::doors (RoomListPtr d)
{
	_doors = d;
}

Boolean
Room::ValidId (RoomId id)
{
	return (True);
}

Boolean
Room::contains_door_to (RoomId id) const
{
	if (doors () != NullPtr)
		return (doors ()->contains (id));
	else	return (False);
}

void
Room::add_door_to (RoomId id, Boolean is_implicit)
{
	if (id == this->id ())
		return;
	if (doors () == NullPtr)
		doors (new RoomList ());
	doors ()->add (id, is_implicit);
}

void
Room::add_door_to (RoomPtr r)
{
	if ((r == NullPtr) || (r->id () == id ()))
		return;
	if (doors () == NullPtr)
		doors (new RoomList);
	doors ()->add (r);
}

ulong
Room::number_of_doors () const
{
	if (doors () == NullPtr)
		return (0);
	return (doors ()->number_of_rooms ());
}

void
Room::dump () const
{
	if (number_of_doors () == 0)
		return;
	printf ("Room %d: ", id ());
	doors ()->dump ();
}

// --------------------------------------------------------------------------

RoomList::RoomListItem::RoomListItem (RoomId id, Boolean is_implicit)
{
	_room = new Room (id, is_implicit);
	_next_room = NullPtr;
}

RoomList::RoomListItem::RoomListItem (RoomPtr r)
{
	_room = r;
	_next_room = NullPtr;
}

RoomList::RoomListItem::~RoomListItem ()
{
}

RoomId
RoomList::RoomListItem::id () const
{
	if (_room == NullPtr)
		return (NullRoomId);
	else	return (_room->id ());
}

RoomPtr
RoomList::RoomListItem::room () const
{
	return (_room);
}

void
RoomList::RoomListItem::dump () const
{
	_room->dump ();
}

// --------------------------------------------------------------------------

RoomList::RoomList ()
{
	_number_of_rooms = 0;
	_rooms = NullPtr;
}

RoomList::~RoomList ()
{
	RoomListItemPtr rp, next_rp;

	for (rp = _rooms ; rp != NullPtr ; rp = next_rp) {
		next_rp = rp->_next_room;
		delete rp;
	}
}

RoomPtr
RoomList::find (RoomId room_id) const
{
	RoomListItemPtr rp;

	for (rp = _rooms ; rp != NullPtr ; rp = rp->_next_room) {
		if (rp->id () == room_id)
			return (rp->room ());
	}
	return (NullPtr);
}

Boolean
RoomList::contains (RoomId room_id) const
{
	if (find (room_id) != NullPtr)
		return (True);
	else	return (False);
}

RoomPtr
RoomList::find_or_add (RoomId room_id, Boolean is_implicit)
{
	RoomPtr rp;

	if ((rp = find (room_id)) != NullPtr)
		return (rp);
	else	return (add (room_id, is_implicit));
}

RoomPtr
RoomList::add (RoomId room_id, Boolean is_implicit)
{
	RoomPtr rp;

	if (!Room::ValidId (room_id))
		return (NullPtr);
	rp = new Room (room_id, is_implicit);
	add (rp);
	return (rp);
}

void
RoomList::add (RoomPtr r)
{
	RoomListItemPtr new_rp, rp, prev_rp = NullPtr;

	new_rp = new RoomListItem (r);

	for (rp = _rooms ; rp != NullPtr ; rp = rp->_next_room) {
		if (rp->id () == new_rp->id ())
			return;
		else if (rp->id () > new_rp->id ())
			break;
		else	prev_rp = rp;
	}

	if (prev_rp == NullPtr)
		_rooms = new_rp;
	else	prev_rp->_next_room = new_rp;
	new_rp->_next_room = rp;

	_number_of_rooms++;
}

ulong
RoomList::number_of_rooms () const
{
	return (_number_of_rooms);
}

void
RoomList::dump () const
{
	RoomListItemPtr rp;

	for (rp = _rooms ; rp != NullPtr ; rp = rp->_next_room) {
		if (rp != _rooms)
			printf (" ");
		printf ("%d", rp->id ());
	}

	for (rp = _rooms ; rp != NullPtr ; rp = rp->_next_room)
		rp->dump ();
	printf ("\n");
}

// --------------------------------------------------------------------------

Maze::Maze (RoomId start_room_id)
{
	if (start_room_id = NullRoomId)
		_all_rooms = new RoomList;
	else	_all_rooms = new RoomList;
	_start_room = _all_rooms->add (1);
}

Maze::~Maze ()
{
	delete _start_room;
}

void
Maze::add_door_to (RoomId room_id, Boolean is_implicit)
{
	_start_room->add_door_to (room_id, is_implicit);
}

void
Maze::add_door_to (RoomPtr r)
{
	_start_room->add_door_to (r);
}

RoomPtr
Maze::room (RoomId room_id, Boolean is_implicit)
{
	return (_all_rooms->find_or_add (room_id, is_implicit));
}

void
Maze::dump () const
{
	_start_room->dump ();
}

// --------------------------------------------------------------------------

main ()
{
	MazePtr maze = new Maze;
	RoomPtr room;

	// Room 1

	maze->add_door_to (20);
	maze->add_door_to (21);
	maze->add_door_to (26);
	maze->add_door_to (41);

	// Room 2

	room = maze->room (2);
	room->add_door_to (12);
	room->add_door_to (22);
	room->add_door_to (29);

	// Room 3

	room = maze->room (3);
	room->add_door_to (9);
	room->add_door_to (15);
	room->add_door_to (18);
	room->add_door_to (33);

	// Room 4

	room = maze->room (4);
	room->add_door_to (11);
	room->add_door_to (15);
	room->add_door_to (16);
	room->add_door_to (24);
	room->add_door_to (29);
	room->add_door_to (39);
	room->add_door_to (42);
	room->add_door_to (43);
	room->add_door_to (44);

	// Room 5

	room = maze->room (5);
	room->add_door_to (20);
	room->add_door_to (22);
	room->add_door_to (30);
	room->add_door_to (43);

	// Room 6

	room = maze->room (6);
	room->add_door_to (8);
	room->add_door_to (32);
	room->add_door_to (40);

	// Room 7

	room = maze->room (7);
	room->add_door_to (16);
	room->add_door_to (33);
	room->add_door_to (36);

	// Room 8

	room = maze->room (8);
	room->add_door_to (6);
	room->add_door_to (12);
	room->add_door_to (29);
	room->add_door_to (31);

	// Room 9

	room = maze->room (9);
	room->add_door_to (3);
	room->add_door_to (18);

	// Room 10

	room = maze->room (10);
	room->add_door_to (14);
	room->add_door_to (34);
	room->add_door_to (37);
	room->add_door_to (41);

	// Room 11

	room = maze->room (11);
	room->add_door_to (4);
	room->add_door_to (19);
	room->add_door_to (24);
	room->add_door_to (32);
	room->add_door_to (39);
	room->add_door_to (40);

	// Room 12

	room = maze->room (12);
	room->add_door_to (2);
	room->add_door_to (8);
	room->add_door_to (21);
	room->add_door_to (39);

	// Room 20

	room = maze->room (20);
	room->add_door_to (1);
	room->add_door_to (5);
	room->add_door_to (27);
	room->add_door_to (37);

	// Room 21

	room = maze->room (20);
	room->add_door_to (1);
	room->add_door_to (12);
	room->add_door_to (24);
	room->add_door_to (31);
	room->add_door_to (44);

	// Dump the maze

	maze->dump ();
}

