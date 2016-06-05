//
// CLOSEST-PAIR-DC.CC 
// Implementation of the divide-and-conquer
// closest-pair algorithm

#include <iostream>
#include <limits>
#include <cmath>

#include "ClosestPairDC.h"
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
//  - points sorted in nondecreasing order by X coordinate
//  - points sorted in nondecreasing order by Y coordinate
//  - # of input points
//

Point** closestPair(Point *pointsByX[], Point *pointsByY[],
	     int nPoints);

Point** Combine(Point *pointsByY[], Point *midPoint,
		int nPoints, Point *lrPoints[]);

//////////////////////////////////////////////////////////////////////////
// main function called by lab1.cc
void findClosestPair(Point *pointsByX[], Point *pointsByY[],
		     int nPoints)
{
	Point** minPoints = new Point*[2];
	minPoints = closestPair(pointsByX, pointsByY, nPoints);

	////order the points so the left most point is first
	if(minPoints[0]->x() > minPoints[1]->x() ||
			(minPoints[0]->x()==minPoints[1]->x() && minPoints[0]->y()>minPoints[1]->y())){
		Point *temp = minPoints[0];
		minPoints[0] = minPoints[1];
		minPoints[1] = temp;
	}

	double minDist = (*minPoints[0]).distance(minPoints[1]);

	cout << minPoints[0] << " "<< minPoints[1] << " "<< minDist << endl;

	delete[] minPoints;
}


//////////////////////////////////////////////////////////////////////////////////
// auxiliary function called by findClosestPair()
Point** closestPair(Point *pointsByX[], Point *pointsByY[],
	     int nPoints)
{
	Point** minPoints = new Point*[2];
	Point** minLPoints = new Point*[2];
	Point** minRPoints = new Point*[2];
	Point** lrPoints = new Point*[2];

	if(nPoints == 1){//base case
		minPoints[0] = pointsByX[0];
		minPoints[1] = new Point(INFTY,INFTY);
	}else if(nPoints == 2){//base case
		minPoints[0] = pointsByX[0];
		minPoints[1] = pointsByX[1];
	}else if(nPoints > 2){
	    Point** minLPoints = new Point*[2];
		Point** minRPoints = new Point*[2];
		Point** lrPoints = new Point*[2];
		int midPoint = ceil(nPoints/2) - 1;
		Point** XL = new Point*[midPoint + 1];
		Point** XR = new Point*[nPoints - midPoint - 1];
		Point** YL = new Point*[midPoint + 1];
		Point** YR = new Point*[nPoints - midPoint - 1];

		int YLindx = 0;
		int YRindx = 0;

		// copy pointsByX into XL, XR
		for (int i=0; i< midPoint + 1; i++){
			XL[i] = pointsByX[i];
 		}
		for (int i= 0; i<nPoints - midPoint - 1; i++){
		    XR[i] = pointsByX[midPoint + i + 1];
		}

		//copy pointsByY into YL, YR
		for (int i=0; i<nPoints; i++){
		    if ((*pointsByY[i]).x() < (*pointsByX[midPoint]).x()){
		    	YL[YLindx] = pointsByY[i];
		     	YLindx++;
		    }else if((*pointsByY[i]).x() > (*pointsByX[midPoint]).x()){
		     	YR[YRindx] = pointsByY[i];
		     	YRindx++;
		    }else{
		    	for(int j = 0; j < midPoint + 1; j++){
		    		if(pointsByY[i] == XL[j] ){
		    			YL[YLindx] = XL[j];
		    			YLindx ++ ;
		    	    }
		    	}
		    	for(int k = 0; k < nPoints - midPoint -1 ; k++){
		    		if(pointsByY[i] == XR[k] ){
		    		    YR[YRindx] = XR[k];
		    		    YRindx ++ ;
		    	    }
		    	}

		    }
		 }

		 int LnPoints = YLindx;
		 int RnPoints = YRindx;

		 //Conquer: recursive calls of helper function
		 minLPoints = closestPair(XL, YL, LnPoints);
		 minRPoints = closestPair(XR, YR, RnPoints);
		 double distL = (*minLPoints[0]).distance(minLPoints[1]);
		 double distR = (*minRPoints[0]).distance(minRPoints[1]);


		 //lrPoints is the closer of the 2 pairs of points minLPoints and minRPoints
		 if (distL <= distR){
			 lrPoints = minLPoints;
		 }else{
			 lrPoints = minRPoints;
		 }

		 //combine
		 minPoints = Combine(pointsByY, pointsByX[midPoint],nPoints, lrPoints);

		 //free memory
		 delete[] YL;
		 delete[] YR;
		 delete[] XL;
		 delete[] XR;
	  }

	delete[] lrPoints;
	delete[] minLPoints;
	delete[] minRPoints;
	return minPoints;
}

////////////////////////////////////////////////////////////////////
// combine function
Point** Combine(Point *pointsByY[], Point *midPoint,
		int nPoints, Point *lrPoints[])
{
	double lrDist = (*lrPoints[0]).distance(lrPoints[1]);
	Point **minCombPoints = new Point*[2];
	minCombPoints = lrPoints;

	Point **yStrip = new Point* [nPoints];
	int yStripIndex = 0;
	double dist = INFTY;

	// assign yStrip points
	for(int i = 0; i < nPoints; i++){
		if(abs((*pointsByY[i]).x() - (*midPoint).x()) < lrDist){
			yStrip[yStripIndex] = pointsByY[i];
			yStripIndex++;
		}
	}

	int yStripLength = yStripIndex;
	double minDist = lrDist;

	for (int j=0; j<yStripLength-1; j++){
		int k = j+1;
	    while (k < yStripLength && (yStrip[k]->y() - yStrip[j]->y() <lrDist)){
	    	dist = (*yStrip[j]).distance(yStrip[k]);
	    	if (dist < minDist){
	    		minCombPoints[0] = yStrip[j];
	    		minCombPoints[1] = yStrip[k];
	    		minDist = dist;
	    	}
	    	k++;
	    }
	}

	delete[] yStrip;
	return minCombPoints;

}
