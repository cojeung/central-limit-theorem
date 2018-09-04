/* NOTE:
 *  chi-squared = summation over bins {of (delta / sigma)^2 }
 *
 * Possible input values:
 *    numBins = 10, numRands (n) = 2, numTotalSums = 1000
 */


#include <iostream>
#include <fstream>
#include <math.h>
#include <sstream>
#include <iomanip>

using namespace std;

/*
 *  Class for linked list implentation
 */
class LinkedList
{
  private:
      typedef struct Node
      {
        Node* next; // next pointer
      }* nodePtr;

  public:
    float value;
    float min;
    float max;

    void AddNode(float data)
    {
      nodePtr rN = new Node; // creates new

      nodePtr head = NULL;
      nodePtr curr = NULL;
      //nodePtr temp = NULL;

      rN->next = NULL; // makes end of list
      value = data; // makes that the rootValue

      if (head) {
        curr = head;
        while(curr->next != NULL) {
          curr = curr->next;
        } //end while
        curr->next = rN; //point to new node
      } //end if
      else {
        head = rN;
      } //end else

      if (value > max) {
        max = value;
      }

    } //end AddNode

    float returnValue() {
      return value;
    } //end returnValue()

    /*
     * For graphing purposes
     * Finds the rightmost bin tick
     */
    float findMax()
    {
      return max;
    } //end findMax

}; //end class LinkedList

/*
 * Calculates the sum of random numbers
 * in the range of -1 to 1
 */
float calcSum(int precision, int numRands)
{
  float randNum = 0;
  float sum = 0;

  float LO = -1; float HI = 1;
  for (int i = 0; i < numRands; i++)
  {
    randNum = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
    sum += randNum;
  }
    return sum;
} //end calcSum

/*
 *  Main Function for implentation of a sum of random numbers
 *  Then calculates the chi squared value
 *  Want graph to be
 *      ( Number of points in BIN and Normal curve) vs ( Number bins )
 */
int main()
{
  LinkedList sumsList;

  /* initialize random seed: */
  srand (time(NULL));

  ofstream sumFile;
  sumFile.open("p2range.txt");

  ofstream chiFile;
  chiFile.open("chi.txt");

  ofstream mickFile_Y; //for mickey mouse shaped data
  mickFile_Y.open("mick_Y.txt");

  ofstream mickFile_X; //for mickey mouse shaped data
  mickFile_X.open("mick_X.txt");

  ofstream mickValid_X;
  mickValid_X.open("mick_Valid_X.txt");

  ofstream mickValid_Y;
  mickValid_Y.open("mick_Valid_Y.txt");


  int N = 0;
  int precision = 0;
  int numRands = 0;
  int numTotalSums = 0;
  const double pi = 3.141592653589793;

  cout << endl;

  cout << "How many bins?  N = ";
  cin >> N;
  cout << "Decimal precision?  ";
  cin >> precision;
  cout << "How many numbers per sum? ";
  cin >> numRands;
  cout << "How many sums? ";
  cin >> numTotalSums;

  sumFile << "==========================================" << endl;
  sumFile << "Total Bins: " << N << endl;
  sumFile << "Decimal Precision: " << precision << endl;
  sumFile << "Numbers per Sum: " << numRands << endl;
  sumFile << "Total Sums: " << numTotalSums << endl;
  sumFile << "===========================================" << endl;

  cout << endl;

  /******************** SETTING UP THE BINS ***********************/

  float arrayOfSums[numTotalSums]; // basically array version of linked list

  /*
   * Generates the list of sums
   * Calculates a sum with numRands and adds value to sumsList
   * Repeates until loop ends at numTotalSums
   */

  for (int i = 0; i < numTotalSums; i++)
  {
    sumsList.AddNode( calcSum(precision, numRands) );
    arrayOfSums[i] = sumsList.returnValue();
  } // end for

  cout << endl;

  float minSum = numRands * (-1); // since range goes from -1 to 1
  float maxSum = numRands; // smallest/largest are -nR and +nR
  float valueRange = (2 * numRands);

  float binSize = valueRange /  (float) N; // length per bin
  cout << "Bin Size: " << binSize << endl;
  cout << "Lowest Possible Sum: " << minSum << endl;
  cout << "Largest Possible Sum: " << maxSum << endl;
  cout << "Value Range: " << valueRange << endl << endl;

  // setting up array that holds range of values due to bin number
  int sumsPerBin[N]; // represents amount of sums stored in a bin
      for (int i = 0; i < N; i++) {
        sumsPerBin[i] = 0; //iniitalize this array
      }

  sumFile << endl << "BIN SIZE: " << binSize << endl;

  /*
   * Setting up the array that keeps track of the bin ticks
   * Ticks range from max and min value of sumsList
   * Varies with binSize and numRands
   */
  float binTicks[N+1]; // keeps track of bin ticks

  sumFile << endl << "===============  BIN TICKS  ==============" << endl;

  for (int i = 0; i < N+1; i++) {
    binTicks[i] = minSum + (binSize * (i));
    cout << "Bin " << i << " starts at " << binTicks[i] << endl;
    sumFile << setprecision(precision) << binTicks[i] << endl;
  }
  // keeps track of the number of values that fit within a certain bin
  cout << endl; 

  sumFile << endl << "TICK RANGE PER BIN: " << endl;

  for (int i = 0; i < N; i++)
  {
    cout << "Sums in bin from " << binTicks[i] << " to "
        << binTicks[i+1] << ": " << sumsPerBin[i] << endl;

    sumFile << endl << binTicks[i] << " to " << binTicks[i+1];
  }

  /*
   *  Tallies the number of sums per bin
   *  and updates sumsPerBin
   */
   for (int i = 0; i < N+1; i++) { // i < N+1 ???????
     for (int j = 0; j < numTotalSums; j++)
     {
       if (   (arrayOfSums[j] >= binTicks[i]) // gte left tick
                && ( arrayOfSums[j] <= binTicks[i+1] ) ) // lte right tick
       {
         sumsPerBin[i]++;
       } //end if
     } //end j for
   } //end i for

  cout << endl;
  sumFile << endl << "================  TALLY PER   =============" << endl;
  sumFile << "=============  RESPECTIVE BIN  ============" << endl;


  for (int i = 0; i < N; i++)
  {
    cout << "Sums in bin from " << binTicks[i] << " to "
        << binTicks[i+1] << ": " << sumsPerBin[i] << endl;

    sumFile << sumsPerBin[i] << endl;
  }
  cout << endl;
  sumFile << endl;

  chiFile << "SQRT OF EACH COUNT PER BIN: " << endl << endl;

  for (int i = 0; i < N; i++)
  {
    sumFile << sumsPerBin[i] << ", ";
    chiFile << ( sqrt(sumsPerBin[i]) ) << endl;
  }

  /***************** CALCULATING MEAN AND STD DEV *******************/

  double mean = 0;
  double meanSum = 0;
  double sd = 0;
  double sdSum[N];

  // calc mean
  for (int i = 0; i < N; i++)
  {
    meanSum += sumsPerBin[i];
  }

  mean = meanSum / N;

  chiFile << endl << "MEAN: " << mean << endl;

  // calc standard deviation
  // std of each value is ( VALUE - MEAN )^2

  chiFile << endl << "Calculating STANDARD DEVIATION: " << endl;

  for (int i = 0; i < N; i++)
  {
    sdSum[i] = sumsPerBin[i] - mean;  // difference between sum and mean
    sdSum[i] = pow(sdSum[i], 2); // square that difference
  } //end for

  for (int i = 0; i < N; i++) // sum up values from previous
  {
    sd += sdSum[i];
  } //end for

  sd = sqrt( sd / N ); // divide by number of data points and take square root

  chiFile << endl << "Standard Deviation: " << sd << endl << endl;

  chiFile << "EXPECTED VALUES F(X): " << endl << endl;

  double chiVal = 0;
  for (int i = 0; i < N; i++)
  {
    double part1 = 1 / sqrt(2 * pi * pow(sd,2));
    double part2 = -pow( (binTicks[i] - mean), 2);
    double part3 = 2 * pow(sd, 2);
    double Gauss = part1 * exp( part2 / part3);

    chiFile << Gauss << endl;

    chiVal += (pow( (sumsPerBin[i] - Gauss), 2)) / Gauss;

  } //end for

  chiFile << "CHI SQUARED VALUE: " << chiVal << endl << endl;


  sumFile << endl << endl;

  sumFile.close();

  /******************** POINTS FOR RANDOM SCATTER PLOT ***********************/

  //let range be -1 to 1
  float LO = -1; float HI = 1;
  double prec = 1 / (pow(10,precision));

  int mickCount = 0;
  int mickTotal = 0;

  int while_int = 0;

  while (while_int < 5)
  {
    for (double x = 0; x <= 2; x += prec)  {
      double x_coord = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
      double y_coord = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

      if (x_coord > 1) { x_coord -= 1; }
      mickFile_X << x_coord << endl;

      if (y_coord > 1) { y_coord -= 1; }
      mickFile_Y << y_coord << endl;

      if (y_coord < 0) {
        mickCount++;
        mickValid_X << x_coord << endl;
        mickValid_Y << y_coord << endl;
      }
      mickTotal++;
    }

    while_int++;
  }

  cout << endl << endl << "==================" << endl << "Mick Count: " << mickCount << endl;
  cout << "Mick Total: " << mickTotal << endl;
  mickFile_Y.close();
  mickFile_X.close();
  mickValid_Y.close();
  mickValid_X.close();

  return 0;
} //end main
