#pragma once

#include <memory>

extern "C" {
	#include <stdio.h>
 	#include "duktape.h"
} 

%HEADERS%

using namespace std;
namespace Polycode {

%BODY%

}
