#-*-python-*-
# Main build script for Avaons

############### You should not need to edit below this line ############
import os
import packages

# Get build environment
env = Environment( ENV = os.environ )

packages.AddPackages( env )

# Temp always in debug
env.Append(CXXFLAGS=["-g"])
env.Append(LINKFLAGS=["-g"])

# Creates binary file
env.Program( target = 'avaons', source = [ "avaons.cc" ] )
