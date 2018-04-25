/*
 * Usage of CDK Matrix
 * NetId: axh161330
 * File:  cdkexample.cc
 * Author: Adil Hamid
 * Email:  adilhamid98@gmail.com 
 */
#include<fstream>
#include <iostream>
#include "cdk.h"
#include <string>
#include <stdint.h>
#include <sstream>

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 25
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;
class BinaryFileHeader
{
public:
  uint32_t magicNumber; /* Should be 0xFEEDFACE */
  uint32_t versionNumber;
  uint64_t numRecords;
};
/*
 * Records in the file have a fixed length buffer
 * that will hold a C-Style string. This is the
 * size of the fixed length buffer.
 */
const int maxRecordStringLength = 25;
class BinaryFileRecord
{
public:
  uint8_t strLength;
  char stringBuffer[maxRecordStringLength];
};

int main()
{

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  // Remember that matrix starts out at 1,1.
  // Since arrays start out at 0, the first entries
  // below ("R0", and "C0") are just placeholders
  // 
  // Finally... make sure your arrays have enough entries given the
  // values you choose to set for MATRIX_WIDTH and MATRIX_HEIGHT
  // above.

  const char 		*rowTitles[] = {"R0", "a", "b", "c", "d", "e"};
  const char 		*columnTitles[] = {"C0", "a", "b", "c", "C4", "C5"};
  int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  stringstream s;  //for strings from file
  stringstream n0; // for int from file
  stringstream n1;
  stringstream n2;
  stringstream n3;
  stringstream n4;
  drawCDKMatrix(myMatrix, true);

  BinaryFileHeader *myHeader = new BinaryFileHeader(); // create object for header
  ifstream binInfile ("cs3377.bin", ios::in | ios::binary); // read binary file
  binInfile.read((char *) myHeader, sizeof(BinaryFileHeader));
  s << "Magic: 0x" << std::hex << myHeader->magicNumber; // use stringstream to  convert to string
  string temp=s.str();
  setCDKMatrixCell(myMatrix, 1, 1, temp.c_str()); // set matrix cell 
  
  
  n0 <<"Version: "<< myHeader->versionNumber; // get version number
  temp=n0.str();
  setCDKMatrixCell(myMatrix, 1, 2, temp.c_str());

  s.str(""); // to clear streamstring
  s<< "NumRecords: " << hex << myHeader->numRecords;
  temp=s.str();
  setCDKMatrixCell(myMatrix, 1, 3, temp.c_str());

  
  BinaryFileRecord *myRecord=new BinaryFileRecord(); // create object for records
  binInfile.read((char*)myRecord, sizeof(BinaryFileRecord)); // read inary file for records
  int num=strlen(myRecord->stringBuffer); //to convert to int
  n1 <<"strlen: " <<num;
  temp=n1.str();
  
  setCDKMatrixCell(myMatrix, 2, 1, temp.c_str()); // to output to matrix cell
  setCDKMatrixCell(myMatrix, 2, 2,myRecord->stringBuffer );

  binInfile.read((char*)myRecord, sizeof(BinaryFileRecord)); // read binary file for next line
   num=strlen(myRecord->stringBuffer);
   n2<<"strlen: " <<num;
  temp=n2.str();
  setCDKMatrixCell(myMatrix, 3, 1, temp.c_str()); // set matrix 3,1 and 3,2
  setCDKMatrixCell(myMatrix, 3, 2,myRecord->stringBuffer );    

  binInfile.read((char*)myRecord, sizeof(BinaryFileRecord)); // read binary file for next line
   num=strlen(myRecord->stringBuffer);
   n3<<"strlen: " <<num;
  temp=n3.str();
  setCDKMatrixCell(myMatrix, 4, 1, temp.c_str());
  setCDKMatrixCell(myMatrix, 4, 2,myRecord->stringBuffer );

  binInfile.read((char*)myRecord, sizeof(BinaryFileRecord)); // read binary file for next line
   num=strlen(myRecord->stringBuffer);
   n4<< "strlen: " <<num;
  temp=n4.str();
  setCDKMatrixCell(myMatrix, 5, 1, temp.c_str());    // to set last 2 matrix spots in matrix
  setCDKMatrixCell(myMatrix, 5, 2,myRecord->stringBuffer );
  /*
   * Dipslay a message
   */
  // setCDKMatrixCell(myMatrix, 2, 2, "Test Message");
  drawCDKMatrix(myMatrix, true);    /* required  */

  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;

  // Cleanup screen
  endCDK();
}
