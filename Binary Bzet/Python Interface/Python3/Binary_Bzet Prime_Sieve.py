#!/usr/bin/python3
# vi: ts=4 sw=4 et ai sm
# (c) Copyright 2011 Robert Uzgalis

# Version of 2011-5-11

#             BZET Primes Example
# Primes are difficult for compression because they stay
# relatively dense. BZETS do remarkably well in the long
# run with only a mild cost in compression when compared to
# a run-length encoded bit-set.  And depending on the size
# of the set about the same as recording the bits without
# compression, or in the case of primes upto 100,001 a yield
# of a 10% reduction in size.  For primes upto 1,000,000
# the reduction would probably be a %30 reduction in size over
# the raw bits.

from BBzetPy3 import BZET
from time import *

def cpssn( bs ):
    return (bs/bmsize)*100.0

maxsize = 100000
report = 1000
notprime = BZET()  # Zero is not a prime
print( "Starting at", ctime(time()))
print( "setting max size to", maxsize )
print( "setting report freq", report )
bmsize = maxsize/8.0
bsize = notprime.size()
print( "Starting bitset size uncompressed", bmsize, "bytes; compressed", \
       bsize, "%4.2f"%cpssn(bsize), "%\n" ) 


# Do Sieve of Eratosthenes to find Primes.

nextp = 1
count = 0
clock()
while nextp < maxsize:    
    if count%report == 1 or count < 25:
        bsize = notprime.size()
        tm = clock()
        print( "%6d"%count, "%7.1f"%tm, "sec; prime:", \
               "%6d;"%nextp, "size", "%6d"%bsize, "%6.2f"%cpssn(bsize) + \
               "%; primes/sec", "%5.2f"%( float(count)/tm) )
    count += 1
    for np in range(nextp+1,min(maxsize,nextp+nextp+1)):
        if not notprime[np]: # Find the next prime
            nextp = np
            break
    else: # we fell through without finding a nextp
        print( "No more primes." )
        break
    # print( "Doing", "%06d"%count, "%06d"%nextp )
    for ix in range( nextp+nextp, maxsize, nextp ):
        notprime.set(ix)

tm = clock()  # Final time.
print( "Ending at", ctime(time()), "%7.1f"%tm, "seconds" )
print( "%5d"%count, "%7.1f"%tm, "sec; last prime:", \
               "%6d"%nextp, "; final size:", bsize, ("%6.2f"%cpssn(bsize)) + \
               "%; primes/sec", "%5.2f"%(float(count)/tm) )

# Print Table of Primes
primes = notprime.NOT()
ix = 0
for p in primes.LIST_T():
    if p > maxsize: break
    if ix%10 == 0: print( "" )
    ix += 1
    print( "%6d"%p, "", end='' )
    
    

