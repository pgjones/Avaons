import os

# Appends ROOT
def root(env):
    ROOTSYS = os.path.join(os.environ["ROOTSYS"], 'bin')
    env.ParseConfig( os.path.join(ROOTSYS, 'root-config') + " --cflags --ldflags --libs ")
    env.Append( CPPPATH = [ os.environ["ROOTSYS"] + "/include"])
    
# Appends RAT
def rat(env):
    env.Append( CPPPATH = [ os.environ["RATROOT"] + "/include" ] )
    env.Append( LIBPATH = [ os.environ["RATROOT"] + "/lib" ] )
    env.Append( LIBS = [ 'rat_' + os.environ["G4SYSTEM"] ] )
    Curl(env)

# Appends Curl and Bzip (for RAT)
def Curl(env):
    env.Append( LIBS = [ 'bz2' ] )
    if "BZIPROOT" in os.environ:
        env['CPPPATH'].append( os.environ['BZIPROOT'] + "/include" )
        env['LIBPATH'].append( os.environ['BZIPROOT'] + "/lib" )
    env.ParseConfig( "curl-config --cflags --libs" )

# Append Avalanche and zmq
def Avalanche(env):
    avalancheLibPath = os.path.join(os.environ['AVALANCHEROOT'], "lib/cpp")
    env.Append( CPPPATH = [ avalancheLibPath, os.environ['ZEROMQROOT'] + "/include" ] )
    env.Append( LIBPATH = [ avalancheLibPath, os.environ['ZEROMQROOT'] + "/lib" ] )
    env.Append( LIBS = [ "avalanche", "zmq" ] )

# Append Python libraries
def Python(env):
    ldflags = env.backtick( "python-config --includes --libs --ldflags").split() # Split on space, diff flags
    # Put all options after -u in LINKFLAGS, may not have a -u part though
    try:
        idx = ldflags.index('-u')
        env.Append( LINKFLAGS=ldflags[idx:] )
        del ldflags[idx:] # Remove the -u part
    except ValueError:
        idx = -1
    env.MergeFlags( ' '.join( ldflags ) )

# Adds all packages
def AddPackages(env):
    Python(env)
    rat(env)
    root(env)
    Avalanche(env)

