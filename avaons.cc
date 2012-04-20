#include <avalanche.hpp>

#include <Python.h>

#include <iostream>
using namespace std;

#include <RAT/DS/PackedEvent.hh>

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// This function calls the python script, returns true if event should not be rebroadcast
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool 
RejectEvent( PyObject* pFunction,
	     RAT::DS::PackedEvent* event )
{
  bool result = false;
  // Currently only send the Nhits
  PyObject* pScriptArgs = PyTuple_New( 1 );
  PyObject* pNhit = PyInt_FromLong( event->NHits );
  PyTuple_SetItem( pScriptArgs, 0, pNhit );

  // Call the function
  PyObject* pResult = PyObject_CallObject( pFunction, pScriptArgs );
  if( pResult == Py_True )
    result = true;
  Py_DECREF( pNhit ); pNhit = NULL; // 'delete' pNhit
  Py_DECREF( pScriptArgs );
  Py_XDECREF( pResult );
  return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// This function runs the event loop
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void
EventLoop( PyObject* pFunction,
	   avalanche::client& client,
	   avalanche::server& server )
{
  while( true )
    {
      RAT::DS::PackedRec* rec = reinterpret_cast<RAT::DS::PackedRec*>( client.recv() );
      if( rec && rec->RecordType == 1 ) // Only deals with PackedEvents, ignores the rest 
	{
	  RAT::DS::PackedEvent* event = dynamic_cast<RAT::DS::PackedEvent*>( rec->Rec );
	  if( RejectEvent( pFunction, event ) == false )
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
  // Setup python
  Py_Initialize();
  PyObject* pScriptName = PyString_FromString( argv[1] );
  PyObject* pScript = PyImport_Import( pScriptName ); // Load script
  Py_DECREF( pScriptName ); pScriptName = NULL; // 'Delete' pScriptName variable
  PyObject* pFunction = PyObject_GetAttrString( pScript, "RejectEvent" );
  if( !pFunction || !PyCallable_Check( pFunction ) )
    {
      cout << "Error: Function RejectEvent in " << argv[1] << " is not callable." << endl;
      return 1;
    }
  // Setup avalanche
  avalanche::client client;
  client.addDispatcher( argv[2] );
  avalanche::server server( argv[3] );
  // All setup, start repeating...
  EventLoop( pFunction, client, server );
  // Cleanup 
  Py_XDECREF( pFunction ); pFunction = NULL; // 'delete' pFunction
  Py_DECREF( pScript ); pScript = NULL; // 'delete' pScript
  Py_Finalize();
}
