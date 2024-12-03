#pragma once

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN_SWAP(small_dest,big_dest,temp) if (small_dest > big_dest) {\
                        temp = small_dest;\
                        small_dest = big_dest;\
                        big_dest = temp;\
                    }
