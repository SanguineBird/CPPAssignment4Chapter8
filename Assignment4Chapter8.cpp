/*
Meghan Moore
February 21, 2015
CIT 245 - Data Structures and Programming C++
Assignment 4 - Chapter 8
Purpose:
Source: Based on code from Display 8.5 in Absolute C++ 5th Edition.
*/

#include <cmath> //abs, fabs, round
#include <iostream> //cin, cout, cerr
#include <cstdlib> //system
using namespace std;

//Class for amounts of money in U.S. currency
class Money{
  public:
    Money(): dollars(0), cents(0){}
    Money(double amount): dollars(dollarsPart(amount)), cents(centsPart (amount)){}
    Money(int theDollars, int theCents);
    Money(int theDollars): dollars(theDollars){cents = 0;}
    
    double getAmount() const {return (dollars + cents * 0.01);}
    int getDollars() const {return dollars;}
    int getCents() const {return cents;}
    friend const Money operator +(const Money& amount1, const Money& amount2);
    friend const Money operator -(const Money& amount1, const Money& amount2);
    friend bool operator ==(const Money& amount1, const Money& amount2);
    friend const Money operator -(const Money& amount);
    friend ostream& operator <<(ostream& outputStream, const Money& amount);
    friend istream& operator >>(istream& inputStream, Money& amount);
    
    friend bool operator >(const Money& amount1, const Money& amount2);
    friend bool operator <(const Money& amount1, const Money& amount2);
    friend const Money percentAmount(int percent, const Money& totalAmount);
  private:
    /*A negative amount is represented as negative dollars and negative cents.
    For example, negative $4.50 is represented as -4 and -50.*/
    int dollars, cents;
    
    int dollarsPart(double amount) const {return static_cast<int>(amount);}
    int centsPart(double amount) const;
    int round(double number) const {return static_cast<int>(floor(number + 0.5));}
};

int main(){
  Money yourAmount, myAmount(10, 9);
  cout << "Enter an amount of money: ";
  cin >> yourAmount;
  cout << "Your amount is " << yourAmount << endl;
  cout << "My amount is " << myAmount << endl;
  
  if(yourAmount == myAmount)
    cout << "We have the same amounts.\n";
  else{
    cout << "One of us is richer.\n";
    if(yourAmount > myAmount)
      cout << "You have more money than me.\n";
    else if(yourAmount < myAmount)
      cout << "I have more money than you.\n";
    else{
      cerr << "Error\n";
      exit(1);
    }
  } //end else
  
  cout << "10% of your money is: " << percentAmount(10, yourAmount) << endl;
  
  Money ourAmount = yourAmount + myAmount;
  cout << yourAmount << " + " << myAmount << " equals " << ourAmount << endl;
  
  Money diffAmount = yourAmount - myAmount;
  cout << yourAmount << " - " << myAmount << " equals " << diffAmount << endl;
  
  system("pause");
  return 0;
} //end main

Money::Money(int theDollars, int theCents){
  if((theDollars < 0 && theCents > 0) || (theDollars > 0 && theCents < 0)){
    cout << "Incosistenet money data.\n";
    exit(1);
  } //end if
  dollars = theDollars;
  cents = theCents;
} //end Money constructor w/two arguments

int Money::centsPart(double amount) const{
  double doubleCents = amount * 100;
  int intCents = (round(fabs(doubleCents))) % 100;
  //% can misbehave on negatives
  
  if(amount < 0)
    intCents = -intCents;
  
  return intCents;
} //end Money::centsPart

const Money operator +(const Money& amount1, const Money& amount2){
  int allCents1 = amount1.cents + amount1.dollars * 100;
  int allCents2 = amount2.cents + amount2.dollars * 100;
  int sumAllCents = allCents1 + allCents2;
  int absAllCents = abs(sumAllCents); //Money can be negative
  int finalDollars = absAllCents / 100;
  int finalCents = absAllCents % 100;
  
  if(sumAllCents < 0){
    finalDollars = -finalDollars;
    finalCents = -finalCents;
  } //end if
  
  return Money(finalDollars, finalCents);
} //end + overload

const Money operator -(const Money& amount1, const Money& amount2){
  int allCents1 = amount1.cents + amount1.dollars * 100;
  int allCents2 = amount2.cents + amount2.dollars * 100;
  int diffAllCents = allCents1 - allCents2;
  int absAllCents = abs(diffAllCents);
  
  int finalDollars = absAllCents / 100;
  int finalCents = absAllCents % 100;
  if(diffAllCents < 0){
    finalDollars = -finalDollars;
    finalCents = -finalCents;
  } //end if
  
  return Money(finalDollars, finalCents);
} //end - binary overload

bool operator ==(const Money& amount1, const Money& amount2){
  return ((amount1.dollars == amount2.dollars) && (amount1.cents == amount2.cents));
} //end == overload

const Money operator -(const Money& amount){
  return Money(-amount.dollars, -amount.cents);
} // end - unary overload

ostream& operator <<(ostream& outputStream, const Money& amount){
  int absDollars = abs(amount.dollars);
  int absCents = abs(amount.cents);
  if(amount.dollars < 0 || amount.cents < 0)
    //accounts for dollars == 0 or cents ==0
    outputStream << "$-";
  else
    outputStream << "$";
  outputStream << absDollars;
  
  if(absCents >= 10)
    outputStream << '.' << absCents;
  else
    outputStream << '.' << '0' << absCents;
  
  return outputStream;
} //end << overload

//Uses iostream and cstdlib:
istream& operator >>(istream& inputStream, Money& amount){
  char dollarSign;
  inputStream >> dollarSign; //hopefully
  if(dollarSign != '$'){
    cout << "No dollar sign in Money input.\n";
    exit(1);
  } //end if
  
  double amountAsDouble;
  inputStream >> amountAsDouble;
  amount.dollars = amount.dollarsPart(amountAsDouble);
  amount.cents = amount.centsPart(amountAsDouble);
  
  return inputStream;
} //end >> overload

bool operator >(const Money& amount1, const Money& amount2){
  int allCents1 = amount1.cents + amount1.dollars * 100;
  int allCents2 = amount2.cents + amount2.dollars * 100;
  
  return (allCents1 > allCents2);
} //end > overload

bool operator <(const Money& amount1, const Money& amount2){
  int allCents1 = amount1.cents + amount1.dollars * 100;
  int allCents2 = amount2.cents + amount2.dollars * 100;
  
  return (allCents1 < allCents2);
} //end < overload

const Money percentAmount(int percent, const Money& totalAmount){
  int allCents = totalAmount.cents + totalAmount.dollars * 100;
  int resultDollars = (allCents / percent) / 100;
  int resultCents = (allCents / percent) % 100;
  
  return Money(resultDollars, resultCents);
} //end percentAmount
