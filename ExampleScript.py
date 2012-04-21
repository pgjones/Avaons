#!/usr/bin/env python
# Author P G Jones - 20/04/2012 <p.g.jones@qmul.ac.uk> : First revision
# This is the example Rejection function script

def RejectEvent( event ):
    """ Currently the event list has one entry, the nhits."""
    print "Got Event with Nhit:", event.NHits
    if event.NHits < 1000:
        return True
    else:
        return False
