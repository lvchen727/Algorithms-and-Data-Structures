//
// EVENTLIST.H
// Interface for your EventList collection type.
//

#ifndef __EVENTLIST_H
#define __EVENTLIST_H

#include <vector>
#include <string>

#include "Event.h"

//
// Single pillar object
//
class EventPillar  {
public:
  EventPillar(Event* e , int);
  EventPillar(int);
  ~EventPillar();


  int getHeight(void);
  int getKey(void);

  std::vector<Event *>* events;
  EventPillar** fwdPillars;

private:
  int pillarHeight;

};

//
// Skiplist object
//
class EventList {
public:
  EventList();
  ~EventList();
  
  //
  // Add an Event to the list.
  //
  void insert(Event *e);
  
  //
  // Remove all Events in the list with the specified year.
  //
  void remove(int year);
  
  //
  // Find all events with greatest year <= input year
  //
  std::vector<Event *> *findMostRecent(int year);


  //
  // Find all Events within the specific range of years (inclusive).
  //
  std::vector<Event *> *findRange(int first, int last);
  
  //
  //resize the skiplist if necessary
  //
  void resize();

  ///////////////////////////////////////////////////////////////////

  // Print entire list (for testing/debugging)
  void dump(std::ostream& os);
  
  EventPillar* head;
  EventPillar* tail;
  int maxHeight = 1;

private:
  int randomHeight();

};

#endif
