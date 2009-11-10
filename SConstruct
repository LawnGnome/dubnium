# wxWidgets base directory. You'll almost certainly need to change this if
# you're on Windows; on other platforms, we use wx-config to get this.
wxWidgetsBase = "C:/wxWidgets-2.8.4"

# CPPUnit base directory. Again, this should only need to be changed on
# Windows, as we can use pkg-config on other platforms. You can ignore this if
# you don't plan to build the unit tests.
cppUnitBase = "#/../cppunit-1.12.0"

# You shouldn't need to change anything below this line if you're not modifying
# the source.

from buildsys.checkers import CheckLongLong, CheckWX, CheckWXConfig
import os
import platform

env = Environment(tools = ["default", "Data"], toolpath = ["buildsys/tools"])
env.Append(CPPPATH=["#/src", "#/images"])

if ARGUMENTS.get("DEBUG", 0):
	debug = True
	buildDir = "#/build/debug"
else:
	debug = False
	buildDir = "#/build/release"

if os.name == "nt":
	# Windows compilation. At present, only MSVC++ is supported.

	# These are the libraries we need to link in when using Visual Studio
	# 2005. This may differ on older versions. The standard parameters are
	# also defined here.
	env.Append(LIBS=[
			"wsock32",
			"uuid",
			"rpcrt4",
			"comctl32",
			"kernel32",
			"user32",
			"gdi32",
			"winspool",
			"comdlg32",
			"advapi32",
			"shell32",
			"ole32",
			"oleaut32",
			"odbc32",
			"odbccp32"
		],
		LIBPATH=["%s/lib/vc_lib" % wxWidgetsBase],
		CPPPATH=[
			"#",
			"%s/include" % wxWidgetsBase,
			"%s/include/msvc" % wxWidgetsBase
			],
		CPPFLAGS=" /EHsc /TP /D_UNICODE /DUNICODE "
	)

	if debug:
		env.Append(LIBS=[
				"wxmsw28ud_adv",
				"wxmsw28ud_aui",
				"wxmsw28ud_core",
				"wxmsw28ud_richtext",
				"wxmsw28ud_stc",
				"wxbase28ud",
				"wxbase28ud_net",
				"wxbase28ud_xml",
				"wxexpatd"
			],
			LINKFLAGS=" /DEBUG ",
			CPPPATH=["C:/wxWidgets-2.8.4/lib/vc_lib/mswud"],
			CPPFLAGS=" /DDEBUG /Zi /MDd /W2 "
		)
	else:
		env.Append(LIBS=[
				"wxmsw28u_adv",
				"wxmsw28u_aui",
				"wxmsw28u_core",
				"wxmsw28u_richtext",
				"wxmsw28u_stc",
				"wxbase28u",
				"wxbase28u_net",
				"wxbase28u_xml",
				"wxexpat"
			],
			CPPPATH=["C:/wxWidgets-2.8.4/lib/vc_lib/mswud"],
			CPPFLAGS=" /DNDEBUG /MD /w /O2 "
		)
else:
	# Sensible platforms with wx-config go here.
	conf = Configure(env, custom_tests={"CheckWXConfig": CheckWXConfig})

	if not conf.CheckWXConfig(2, 8):
		print "You must have wxWidgets 2.8 installed."
		Exit(1)

	conf.Finish()

	if debug:
		env.Append(CXXFLAGS=" -g -Wall ")
		env.ParseConfig("wx-config --debug --cxxflags --libs std,aui,richtext,stc")
	else:
		env.Append(CXXFLAGS=" -O3 ")
		env.ParseConfig('wx-config --cxxflags --libs std,aui,richtext,stc')

# Test our environment settings.
conf = Configure(env, custom_tests={"CheckLongLong": CheckLongLong, "CheckWX": CheckWX})

if not conf.CheckCXXHeader("wx/wx.h"):
	print "wx.h is not in the include path."
	Exit(1)

if not conf.CheckWX():
	print "Cannot link a wxWidgets application."
	Exit(1)

conf.Finish()

# First compile the library, then the binaries in this directory that depend on
# it.
images = SConscript("#/images/SConscript", duplicate=0, exports=["env"])
libDBGp = SConscript("#/src/DBGp/SConscript", build_dir=buildDir, src_dir="#/src", duplicate=0, exports=["env"])
SConscript("#/src/SConscript", build_dir=buildDir, src_dir="#/src", duplicate=0, exports=["env", "libDBGp"])
SConscript("#/src/TestApp/SConscript", build_dir=buildDir, src_dir="#/src", duplicate=0, exports=["env", "libDBGp"])
Dubnium = SConscript("#/src/Dubnium/SConscript", build_dir=buildDir, src_dir="#/src", duplicate=0, exports=["env", "images", "libDBGp"])
SConscript("#/tests/SConscript", duplicate=0, exports=["env", "libDBGp", "cppUnitBase", "debug"])

# Awful, awful hack to support generating an OSX application bundle. There's
# modules on the SCons Wiki that are supposed to be able to do this, but
# they're pretty nasty as well and break ParseConfig.
if platform.system() == "Darwin":
	bundle = env.Command("%s/Dubnium.app" % buildDir, Dubnium, "buildsys/bundle.sh '%s' 'build/Dubnium.app'" % Dubnium[0])
	env.Depends(bundle, Dubnium)
	env.Default(bundle)

# vim:set ts=8 sw=8 noet nocin ai ft=python:
