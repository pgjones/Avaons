import os

# Appends ROOT
def root(env):
    ROOTSYS = os.path.join(os.environ["ROOTSYS"], 'bin')
    env.ParseConfig( os.path.join(ROOTSYS, 'root-config') + " --cflags --ldflags --libs ")
    env.Append( CPPPATH = [ os.environ["ROOTSYS"] + "/include"])
    env.Append( LIBS = 'PyROOT' )
    
# Appends RAT
def rat(env):
    env.Append( CPPPATH = [ os.environ["RATROOT"] + "/include" ] )
    env.Append( LIBPATH = [ os.environ["RATROOT"] + "/lib" ] )
    env.Append( LIBS = [ 'rat_' + os.environ["G4SYSTEM"] ] )
    Curl(env)

# Appends Curl and Bzip (for RAT)
def Curl(env):
    env.Append( LIBS = [ "bz2", "curl" ] )

# Append Avalanche and zmq
def Avalanche(env):
    avalancheLibPath = os.path.join(os.environ['AVALANCHEROOT'], "lib/cpp")
    env.Append( CPPPATH = [ avalancheLibPath, os.environ['ZEROMQROOT'] + "/include" ] )
    env.Append( LIBPATH = [ avalancheLibPath, os.environ['ZEROMQROOT'] + "/lib" ] )
    env.Append( LIBS = [ "avalanche", "zmq" ] )

# Adds all packages
def AddPackages(env):
    rat(env)
    root(env)
    Avalanche(env)

