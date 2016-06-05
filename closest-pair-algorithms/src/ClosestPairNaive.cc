//
// CLOSEST-PAIR-NAIVE.CC
// Naive implementation of the closest-pair algorithm

#include <iostream>
#include <limits>

#include "ClosestPairNaive.h"

using namespace std;

// bigger than any possible interpoint distance
const double INFTY = numeric_limits<double>::max(); 

//
// findClosestPair()
//
// Given a collection of nPoints points, find and ***print***
//  * the closest pair of points
//  * the distance between them
// in the form "(x1, y1) (x2, y2) distance"
//

// INPUTS:
//  - points (not necessarily sorted)
//  - # of input points
//
void findClosestPairNaive(Point *points[], int nPoints)
{
  //
  // Your code here!
  //
  if(nPoints == 1){
	  cout << INFTY << endl;
  }else{

	  double min = (*points[0]).distance(points[1]);
	  Point *point1 = points[0];
	  Point *point2 = points[1];

	  for(int i = 0; i < nPoints-1; i++){
		  for(int j = i + 1; j < nPoints; j++){
			  double dis = (*points[i]).distance(points[j]);
			  if(dis < min){
				  min = dis;
				  if(((*points[i]).x() < (points[j])->x())||
				     (((*points[i]).x() == (points[j])->x())&&((*points[i]).y() < (points[j])->y()))){
					  point1 = points[i];
					  point2 = points[j];
				  }else{
					  point1 = points[j];
					  point2 = points[i];
				  }
			  }
		  }
	  }

	  cout << point1 << " "<<point2 << " " << min << endl;
  }
}
