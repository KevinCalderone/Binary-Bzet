#ifndef __BBZETWRAPPER_H__
#define __BBZETWRAPPER_H__

#include "Python.h"
#include "BinaryBzet.h"
#include "bitR.h"
#include <stdio.h>
#define DLLEXPORT extern "C" __declspec(dllexport)

using namespace std;

//bitR
DLLEXPORT bitR* bitR_new(){
	return new bitR(); 
}
DLLEXPORT void bitR_add(bitR* bitRObj, uint start, uint end=0, uint step=0 ){ 
	bitRObj->add(start, end, step); 
}
DLLEXPORT bool bitR_at(bitR* bitRObj, uint index){
	return bitRObj->at(index); 
}
DLLEXPORT uint bitR_size(bitR* bitRObj){
	return bitRObj->size(); 
}

/*------------BinaryBzet--------------*/
DLLEXPORT BinaryBzet* BinaryBzet_new(){
	return new BinaryBzet(); 
}
DLLEXPORT BinaryBzet* BinaryBzet_new_index(uint indexi, uint indexe=0, uint step=0){
	return new BinaryBzet(indexi, indexe=0, step=0);
}
DLLEXPORT BinaryBzet* BinaryBzet_new_string(char* bitstring){
	string input = string(bitstring);
	return new BinaryBzet(input);
}
DLLEXPORT BinaryBzet* BinaryBzet_new_vector(vector<bool>* bzetvector, uint depth){
	return new BinaryBzet(bzetvector, depth);
}

//Miscellaneous Bzet Property Functions
DLLEXPORT uint BinaryBzet_getDepth(BinaryBzet* BBObj){return BBObj->getDepth();}
DLLEXPORT uint BinaryBzet_size(BinaryBzet* BBObj){return BBObj->size();}
DLLEXPORT void BinaryBzet_set(BinaryBzet* BBObj,uint index){return BBObj->set(index);}
DLLEXPORT void BinaryBzet_unset(BinaryBzet* BBObj, uint index){return BBObj->unset(index);}
DLLEXPORT void BinaryBzet_flip(BinaryBzet* BBObj, uint index){return BBObj->flip(index);}
DLLEXPORT void BinaryBzet_clean(BinaryBzet* BBObj){return BBObj->clean();}
DLLEXPORT uint BinaryBzet_countBits (BinaryBzet* BBObj){return BBObj->countBits();}
DLLEXPORT uint BinaryBzet_getFirstBit (BinaryBzet* BBObj){return BBObj->getFirstBit();}
DLLEXPORT uint BinaryBzet_getLastBit (BinaryBzet* BBObj){return BBObj->getLastBit();}
DLLEXPORT vector<uint> BinaryBzet_bitList (BinaryBzet* BBObj){return BBObj->bitList();}
  
//Print Bzet
DLLEXPORT uint BinaryBzet_getBzetString(BinaryBzet* BBObj, char* output){
	string temp = BBObj->getBzetString();
	strcpy(output, temp.c_str());
	return strlen(output); 
}
DLLEXPORT uint BinaryBzet_getBzetPretty(BinaryBzet* BBObj,char* output){
	string temp = BBObj->getBzetPretty();
	strcpy(output, temp.c_str());
	return strlen(output);
}

//Binary Boolean Operations
DLLEXPORT BinaryBzet BinaryBzet_FALSE (BinaryBzet* BBObj, const BinaryBzet& rhs){	return BBObj->FALSE(rhs);	}
DLLEXPORT BinaryBzet BinaryBzet_AND (BinaryBzet* BBObj, const BinaryBzet& rhs){	return BBObj->AND(rhs);	}
DLLEXPORT BinaryBzet BinaryBzet_NonImplication (BinaryBzet* BBObj, const BinaryBzet& rhs){	return BBObj->NonImplication(rhs);	}
DLLEXPORT BinaryBzet BinaryBzet_A(BinaryBzet* BBObj, const BinaryBzet& rhs){	return BBObj->A(rhs);	}
DLLEXPORT BinaryBzet BinaryBzet_ConverseNonImplication(BinaryBzet* BBObj, const BinaryBzet& rhs){	return BBObj->ConverseNonImplication(rhs);	}
DLLEXPORT BinaryBzet BinaryBzet_B(BinaryBzet* BBObj, const BinaryBzet& rhs){	return BBObj->B(rhs);	}
DLLEXPORT BinaryBzet BinaryBzet_XOR(BinaryBzet* BBObj, const BinaryBzet& rhs){	return BBObj->XOR(rhs);	}
DLLEXPORT BinaryBzet BinaryBzet_OR(BinaryBzet* BBObj, const BinaryBzet& rhs){	return BBObj->OR(rhs);	}
DLLEXPORT BinaryBzet BinaryBzet_NOR(BinaryBzet* BBObj, const BinaryBzet& rhs){	return BBObj->NOR(rhs);	}
DLLEXPORT BinaryBzet BinaryBzet_EQ(BinaryBzet* BBObj, const BinaryBzet& rhs){	return BBObj->EQ(rhs);	}
DLLEXPORT BinaryBzet BinaryBzet_NotB(BinaryBzet* BBObj, const BinaryBzet& rhs){	return BBObj->NotB(rhs);	}	
DLLEXPORT BinaryBzet BinaryBzet_ConverseImplication(BinaryBzet* BBObj, const BinaryBzet& rhs){	return BBObj->ConverseImplication(rhs);	}
DLLEXPORT BinaryBzet BinaryBzet_NotA(BinaryBzet* BBObj, const BinaryBzet& rhs){	return BBObj->NotA(rhs);	}
DLLEXPORT BinaryBzet BinaryBzet_Implication(BinaryBzet* BBObj, const BinaryBzet& rhs){	return BBObj->Implication(rhs);	}
DLLEXPORT BinaryBzet BinaryBzet_NAND(BinaryBzet* BBObj, const BinaryBzet& rhs){	return BBObj->NAND(rhs);	}
DLLEXPORT BinaryBzet BinaryBzet_TRUE(BinaryBzet* BBObj, const BinaryBzet& rhs){	return BBObj->TRUE(rhs);	}

//Comparison Operators
DLLEXPORT int BinaryBzet_compare(BinaryBzet& bzet);

//Shifing Operators
DLLEXPORT void BinaryBzet_leftShift(uint distance);
DLLEXPORT void BinaryBzet_rightShift(uint distance);

//Slice/Substring
DLLEXPORT BinaryBzet BinaryBzet_slice(uint startIndex, uint endIndex);


#endif