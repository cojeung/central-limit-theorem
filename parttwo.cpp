#include <iostream>
#include <fstream>
#include <math.h>
#include <sstream>
#include <iomanip>
#include <sstream>

using namespace std;

ofstream AMPS;
ofstream SIGS;

int runProg(double choice, ofstream &AMPS, ofstream &SIGS)
{
  double UP_VAL = 0;
  int J_LIM = 0;

  if (choice == 1) {
    UP_VAL = 20.7485;
    J_LIM = 100000;
  }
  else if (choice == 2) {
    UP_VAL = 22.9485;
    J_LIM = 100000;
  }
  else if (choice == 3) {
    UP_VAL = 24.3285;
    J_LIM = 100000;
  }
  else if (choice == 4) {
    UP_VAL = 27.5485;
    J_LIM = 1000000;
  }

  //create files
  ifstream binFile; //file containing bin sums
  binFile.open("bins2.txt");

  ofstream chiFile; //file to write chi values to
  chiFile.open("chi.txt");

  // ofstream AMPS;
  // ofstream SIGS;
  //
  // AMPS.open("AMPS.txt");
  // SIGS.open("SIGS.txt");

  // VARIABLES
  string binValue; //line read from binFile
  int N = 20; //number of bins
  long double std_dev = 0; //standard deviation
   srand (time(NULL));
  long double LO_AMP = 1000; long double HI_AMP = 2000;
  long double LO_SIG = 1; long double HI_SIG = 4;
  long double lowest_chi = 0;
  long double curr_chi = 0;
  long double kept_chi = 0;
  long double best_amp = 0;
  long double best_sig = 0;
  int binValue_int = 0;

  for (int j = 0; j < J_LIM; j++)
  {
    long double rand_amp = LO_AMP + static_cast <long double> (rand()) /( static_cast <long double> (RAND_MAX/(HI_AMP-LO_AMP)));
    long double rand_sig = LO_SIG + static_cast <long double> (rand()) /( static_cast <long double> (RAND_MAX/(HI_SIG-LO_SIG)));

    // f(x) = A * exp( -x^2 / (2 * sigma * sigma) )
    for(int i = 1; i < N+1; i++) //loop thru each bin
    {
      getline(binFile, binValue);
      binValue_int = stoi(binValue);

      int x = i - (N/2); //to adjust value to -p to p
      long double gauss = rand_amp * exp( (-(pow(x,2))) /  (2 * pow(rand_sig,2)) );
      std_dev = sqrt(binValue_int);

      long double numerator = gauss - ( binValue_int + std_dev);
      curr_chi += ( pow(numerator,2) / gauss);
    }
    curr_chi = curr_chi / N; //average out chi square values
    //curr_chi = curr_chi / 100;

    if (curr_chi <= UP_VAL) {
      AMPS << rand_amp << endl;
      SIGS << rand_sig << endl;
    }

    if (j == 0) {
      lowest_chi = curr_chi;
    }

    //compare and keep lowest value
    if (lowest_chi <= curr_chi) {
      kept_chi = lowest_chi;
      best_amp = rand_amp;
      best_sig = rand_sig;
    } else { //if curr_chi < lowest_chi
      lowest_chi = curr_chi;
      kept_chi = curr_chi;
      best_amp = rand_amp;
      best_sig = rand_sig;
    }

    //"resetting" file
    binFile.clear();
    binFile.seekg(0, ios::beg);
  } //end j for------done getting best possible chi square

  chiFile << "Best Chi Value: " << kept_chi << endl;
  chiFile << "Used: " << endl;
  chiFile << "          Amplitude: " << best_amp << endl;
  chiFile << "          Sigma: " << best_sig << endl;

  cout << "Best Chi Value: " << kept_chi << endl;
  cout << "Used: " << endl;
  cout << "          Amplitude: " << best_amp << endl;
  cout << "          Sigma: " << best_sig << endl;
  cout << endl;

  binFile.close();
  chiFile.close();

  return 0;
} //end runProg

int main()
{
  AMPS.open("AMPS.txt");
  SIGS.open("SIGS.txt");

  cout << "Choose up value: " << endl;
  cout << "1) 70\t2) 90\t3) 95\t4)99" << endl << endl;
  int choice = 0;
  cout << "   Choice: ";
  cin >> choice;

  for(int i = 0; i < 1; i++) {
    runProg(choice, AMPS, SIGS);
  }

  cout << "DONE" << endl;

  AMPS.close();
  SIGS.close();

  return 0;
}
