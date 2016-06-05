//
// EVENTLIST.CC
// Skeleton code for your EventList collection type.
//

#include <iostream>
#include <cstdlib>
#include <iomanip>   // for dump() function

#include "EventList.h"

using namespace std;

////////////////////////////////////////////////////////////////////
// Here's a suitable geometric random number generator for choosing
// pillar heights.  We treat the middle bit of each random number
// generated as a random coin flip.
////////////////////////////////////////////////////////////////////

int EventList::randomHeight()
{
  int v = 1;
  while ((rand() >> 16) & 0x01) { v++; }
  return v;
}

////////////////////////////////////////////////////////////////////
// Methods for the Lab
////////////////////////////////////////////////////////////////////

//
// EventPillar constructor: using one pillar for all events in a particular year
// the parameter events stored a list of events with each pillar
//
EventPillar::EventPillar(Event* e, int h)
{
	pillarHeight = h;
	events = new vector<Event *>();
	events->push_back(e);

	fwdPillars = new EventPillar* [h+1];// the forward pointers in a pillar are stored in an array in that pillar,
	                                    // and they points to nodes farther down the skip list
	for(int x = 1; x <= pillarHeight; x++){
		fwdPillars[x] = (EventPillar*) nullptr;
	}
}

//
//EventPillar constructor for heads and tails
//
EventPillar::EventPillar(int h)
{
	pillarHeight = h;
	events = nullptr;
	fwdPillars = new EventPillar* [h+1];
	for(int x = 1; x <= pillarHeight; x++){
		fwdPillars[x] = (EventPillar*) nullptr;
	}
}

//
//return the year of events stored in that pillar
//
int EventPillar::getKey() {
	if(events != nullptr){
		return (*(events->begin()))->year;
	}else{
		return 1000000;
	}
}

//
//return the height of the pillar
//
int EventPillar::getHeight(){
	return pillarHeight;
}

//
// EventPillar destructor
//
EventPillar::~EventPillar()
{
	delete events;
	delete [] fwdPillars;
}

//
// EventList constructor
//
EventList::EventList()
{
  srand(8725); // you may seed the PRNG however you like.

  //create head and tail and attach them
  head = new EventPillar(maxHeight);
  tail = new EventPillar(maxHeight);

  for(int x = 1; x <= maxHeight; x++){
	  head->fwdPillars[x] = tail;
  }
}


//
// EventList destructor
//
EventList::~EventList()
{
	//walk 0 level nodes and delete all
	EventPillar* tmp;
	EventPillar* nxt;
	tmp = head;
	while(tmp){
		nxt = tmp->fwdPillars[1];
		delete tmp;
		tmp = nxt;
	}
}

//
//resize the skip list if necessary
//
void EventList::resize(){

	int k = maxHeight;

	// copy the current head into temperary pillars
	EventPillar* tmp = new EventPillar(k);
	for(int i = 1; i <= k; i++){
		tmp->fwdPillars[i] = head->fwdPillars[i];
	}

	maxHeight = maxHeight * 2;
	head = new EventPillar(maxHeight);
	tail = new EventPillar(maxHeight);
	for(int i = 1; i <= maxHeight; i++){
		head->fwdPillars[i] = tail;
	}

	//set heads point to old heads
	for(int i = 1; i <= k; i++ ){
		head->fwdPillars[i] = tmp->fwdPillars[i];
	}

}


//
// Add an Event to the list.
//
void EventList::insert(Event *e)
{
	int h = 0;
	EventPillar** updateVec = new EventPillar* [maxHeight + 1];//updateVec pointers always point to the most previous node
	EventPillar* tmp = head;                                   //prior to the insertion/deletion point at each level.
	int cmpKey;
	int key = (*e).year;

	//Figure out where new node goes
	for( h = maxHeight; h >=1; h--){
		cmpKey = (*(tmp->fwdPillars[h])).getKey();
		while(cmpKey < key){
			tmp = tmp->fwdPillars[h];
			cmpKey = (*(tmp->fwdPillars[h])).getKey();
		}
		updateVec[h] = tmp;
	}
	tmp = tmp->fwdPillars[1];
	cmpKey = tmp->getKey();

	// if the year already exists, push the new event into the right pillar.
	if( cmpKey == key ){
		tmp->events->push_back(e);
	}
	else{//new year
		//Perform an insert
		int level = randomHeight();
		if(level > maxHeight){
			int k = maxHeight;
			while(maxHeight < level){
				resize();
			}
			for(int i = k + 1; i <= maxHeight; i++){
				updateVec[i] = head;
			}
		}

		//insert new element
		tmp = new EventPillar(e, level);
		for(int i = 1; i <= level; i++){
			tmp->fwdPillars[i] = updateVec[i]->fwdPillars[i];
			updateVec[i]->fwdPillars[i] = tmp;
		}
	}
}


//
// Remove all Events in the list with the specified year.
//
void EventList::remove(int year)
{
	int h = 0;
	EventPillar ** updateVec = new EventPillar* [maxHeight + 1];
	EventPillar* tmp = head;
	int cmpKey;

	//Find the key and delete the node
	for(h = maxHeight; h >=1; h--){
		cmpKey = (*(tmp->fwdPillars[h])).getKey();
		while( cmpKey < year ){
			tmp = tmp->fwdPillars[h];
			cmpKey = (*(tmp->fwdPillars[h])).getKey();
		}
		updateVec[h] = tmp;
	}

	tmp = tmp->fwdPillars[1];
	cmpKey = tmp->getKey();

	if( cmpKey == year ){
		for(int i = 1; i < maxHeight; i++){
			if( updateVec[i]->fwdPillars[i] != tmp )
				break;
			updateVec[i]->fwdPillars[i] = tmp->fwdPillars[i];
		}
		delete tmp;
		while((maxHeight > 1) && (head->fwdPillars[maxHeight]->events == nullptr)){
			maxHeight --;
		}
	}
}


//
// Find all events with greatest year <= input year.
//
vector<Event *> *EventList::findMostRecent(int year)
{

	vector<Event *> *allEvents = new vector<Event *>();
	EventPillar ** updateVec = new EventPillar* [maxHeight + 1];
	EventPillar* tmp = head;
	int cmpKey;

	//Find the key - greatest year <= input year and return the node
	for(int h = maxHeight; h >=1; h--){
		cmpKey = (*(tmp->fwdPillars[h])).getKey();
		while( cmpKey < year){
			tmp = tmp->fwdPillars[h];
			cmpKey = (*(tmp->fwdPillars[h])).getKey();
		}
		updateVec[h] = tmp;
	}

	cmpKey =(*( tmp->fwdPillars[1])).getKey();
	EventPillar* tmp2 = tmp->fwdPillars[1];

	if(tmp == head){//no year exist
		return nullptr;
	}else if(cmpKey == year){ // year = input year
		for(int i = 0; i < tmp2->events->size(); i++){
			allEvents->push_back(tmp2->events->at(i));
		}
	}else{// year < input year
		for(int i = 0; i < tmp->events->size(); i++){
			allEvents->push_back(tmp->events->at(i));
		}
	}
	return allEvents;
}


//
// Find all Events within the specific range of years (inclusive).
//
vector<Event *> *EventList::findRange(int first, int last)
{
	vector<Event *> *allEvents = new vector<Event *>();
	EventPillar ** updateVec = new EventPillar* [maxHeight + 1];
	EventPillar* tmp = head;
	int cmpKey1;

	//Find the key before "first" and record the node
	for(int h1 = maxHeight; h1 >=1; h1--){
		cmpKey1 = (*(tmp->fwdPillars[h1])).getKey();
		while( cmpKey1 < first){
			tmp = tmp->fwdPillars[h1];
			cmpKey1 = (*(tmp->fwdPillars[h1])).getKey();
		}
		updateVec[h1] = tmp;
	}

	tmp = tmp->fwdPillars[1];
	cmpKey1 = tmp->getKey();

	//Find the key before "last" and record the node
	EventPillar* tmp2 = head;
	EventPillar ** updateVec2 = new EventPillar* [maxHeight + 1];
	int cmpKey2;
	for(int h2 = maxHeight; h2 >=1; h2--){
		cmpKey2 = (*(tmp2->fwdPillars[h2])).getKey();
		while( cmpKey2 < last){
			tmp2 = tmp2->fwdPillars[h2];
			cmpKey2 = (*(tmp2->fwdPillars[h2])).getKey();
		}
		updateVec2[h2] = tmp2;
	}
	tmp2 = tmp2->fwdPillars[1];
	cmpKey2 = tmp2->getKey();

	while(tmp != tmp2){
		for(int i = 0; i < tmp->events->size(); i++){
			allEvents->push_back(tmp->events->at(i));
		}
		tmp = tmp->fwdPillars[1];
	}

	if ( cmpKey2 == last ){ //include last if it exist
		for(int i = 0; i < tmp2->events->size(); i++){
			allEvents->push_back(tmp2->events->at(i));
		}
	}

	return allEvents;
}

//
// Utility function to print list
// Usage (in Lab3.cc): list->dump(cout);
//
void EventList::dump(ostream& os)
{

  // Your code may look different depending on how you implement
  //   the EventPillar object.  Use below code as an example, 
  //   but feel free to modify it to suit your needs.
/*
  os << "Event list: maxUsedHeight: " << maxUsedHeight << endl;
  EventPillar *p = head;
  while(p)
  {
    int key = (p->e) ? p->e->year : numeric_limits<int>::max();
    os << "Pillar " << p << " key " 
      << key << ": ";
    for(int i=0; i < p->height; ++i)
      os << setw(10) << p->next[i];
    os << endl;
    p = p->next[0];
  }
*/
	 os << "Event list: maxUsedHeight: " << maxHeight << endl;
	 EventPillar * tmp = head;
	 os << "There is the head node!" << endl;
	 tmp = tmp->fwdPillars[1];
	 int i = 1;
	 while(tmp->events != nullptr){
		 os << i << ":" << tmp->getKey() << endl;
		 tmp = tmp->fwdPillars[1];
		 i++;
	 }
	 os << "There is the tail node!" << endl;

}
