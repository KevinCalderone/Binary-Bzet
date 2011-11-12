%module mymodule
%include "std_string.i"
%include "std_vector.i"

%{
#include "BinaryBzet.h"
%}

%include "BinaryBzet.h"