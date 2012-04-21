#include <avalanche.hpp>

#include <iostream>
using namespace std;

#include <RAT/DS/PackedEvent.hh>

#include <TPython.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// This function calls the python script, returns true if event should not be rebroadcast
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool 
RejectEvent( char* pyFile,
	     RAT::DS::PackedEvent* event )
{
  TPython pyWrapper;
  pyWrapper.LoadMacro( pyFile );
  pyWrapper.Bind( event, "packedEvent" );
  return static_cast<int>( pyWrapper.Eval( "RejectEvent( packedEvent )" ) ); // Convert to int first, then bool in return
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// This function runs the event loop
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void
EventLoop( char* pyFile,
	   avalanche::client& client,
	   avalanche::server& server )
{
  while( true )
    {
      RAT::DS::PackedRec* rec = reinterpret_cast<RAT::DS::PackedRec*>( client.recv() );
      if( rec && rec->RecordType == 1 ) // Only deals with PackedEvents, ignores the rest 
	{
	  RAT::DS::PackedEvent* event = dynamic_cast<RAT::DS::PackedEvent*>( rec->Rec );
	  if( RejectEvent( pyFile, event ) == false )
	    server.sendObject( rec );
	}
      delete rec;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Entry point to avaons
/////////////////////////////////////////////////////////////////////////////////////////////////////////
int 
main( int argc, 
      char* argv[] )
{
  // argv[1] should be the name of the python script to execute, argv[2] input port, argv[3] output
  if( argc != 4 )
    {
      cout << "Error: Incorrect usage, should be avaons pythonScript inputPort outputPort." << endl;
      return 1;
    }
  // Setup avalanche
  avalanche::client client;
  client.addDispatcher( argv[2] );
  avalanche::server server( argv[3] );
  // All setup, start repeating...
  EventLoop( argv[1], client, server );
}
