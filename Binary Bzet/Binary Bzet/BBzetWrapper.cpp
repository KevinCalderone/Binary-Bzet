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

DLLEXPORT void BinaryBzet_TEST_AND (BinaryBzet* a, BinaryBzet* b, BinaryBzet* result) {	
	*result = a->AND(*b);
}

//Binary Boolean Operations
DLLEXPORT void BinaryBzet_FALSE (BinaryBzet* BBObj, const BinaryBzet* rhs, BinaryBzet* result){	*result = BBObj->FALSE(rhs);	}
DLLEXPORT void BinaryBzet_AND (BinaryBzet* BBObj, const BinaryBzet* rhs, BinaryBzet* result){	*result = BBObj->AND(rhs);	}
DLLEXPORT void BinaryBzet_NonImplication (BinaryBzet* BBObj, const BinaryBzet* rhs, BinaryBzet* result){	*result = BBObj->NonImplication(rhs);	}
DLLEXPORT void BinaryBzet_A(BinaryBzet* BBObj, const BinaryBzet* rhs, BinaryBzet* result){	*result = BBObj->A(rhs);	}
DLLEXPORT void BinaryBzet_ConverseNonImplication(BinaryBzet* BBObj, const BinaryBzet* rhs, BinaryBzet* result){	*result = BBObj->ConverseNonImplication(rhs);	}
DLLEXPORT void BinaryBzet_B(BinaryBzet* BBObj, const BinaryBzet* rhs, BinaryBzet* result){	*result = BBObj->B(rhs);	}
DLLEXPORT void BinaryBzet_XOR(BinaryBzet* BBObj, const BinaryBzet* rhs, BinaryBzet* result){	*result = BBObj->XOR(rhs);	}
DLLEXPORT void BinaryBzet_OR(BinaryBzet* BBObj, const BinaryBzet* rhs, BinaryBzet* result){	*result = BBObj->OR(rhs);	}
DLLEXPORT void BinaryBzet_NOR(BinaryBzet* BBObj, const BinaryBzet* rhs, BinaryBzet* result){	*result = BBObj->NOR(rhs);	}
DLLEXPORT void BinaryBzet_EQ(BinaryBzet* BBObj, const BinaryBzet* rhs, BinaryBzet* result){	*result = BBObj->EQ(rhs);	}
DLLEXPORT void BinaryBzet_NotB(BinaryBzet* BBObj, const BinaryBzet* rhs, BinaryBzet* result){	*result = BBObj->NotB(rhs);	}	
DLLEXPORT void BinaryBzet_ConverseImplication(BinaryBzet* BBObj, const BinaryBzet* rhs, BinaryBzet* result){	*result = BBObj->ConverseImplication(rhs);	}
DLLEXPORT void BinaryBzet_NotA(BinaryBzet* BBObj, const BinaryBzet* rhs, BinaryBzet* result){	*result = BBObj->NotA(rhs);	}
DLLEXPORT void BinaryBzet_Implication(BinaryBzet* BBObj, const BinaryBzet* rhs, BinaryBzet* result){	*result = BBObj->Implication(rhs);	}
DLLEXPORT void BinaryBzet_NAND(BinaryBzet* BBObj, const BinaryBzet* rhs, BinaryBzet* result){	*result = BBObj->NAND(rhs);	}
DLLEXPORT void BinaryBzet_TRUE(BinaryBzet* BBObj, const BinaryBzet* rhs, BinaryBzet* result){	*result = BBObj->TRUE(rhs);	}

#endif
