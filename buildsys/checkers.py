import subprocess

def CheckLongLong(context):
	context.Message("Checking for C++ compiler support for long long... ")
	ret = context.TryLink("""
int main() {
	long long ll = 42LL;
	return 0;
}
	""", ".cpp")
	context.Result(ret)
	return ret

def CheckWXConfig(context, major, minor):
	context.Message("Checking for wx-config >= %d.%d... " % (major, minor))
	wxConfig = subprocess.Popen(["wx-config", "--version-full"], stdout=subprocess.PIPE)
	version = wxConfig.communicate()[0]
	if wxConfig.returncode == 0:
		wxVer = version.split(".")
		ret = (int(wxVer[0]) > major) or (int(wxVer[0]) == major and int(wxVer[1]) >= minor)
	else:
		ret = 0
	context.Result(ret)
	return ret

def CheckWX(context):
	context.Message("Checking wxWidgets compilation... ")
	ret = context.TryLink("""
#include <wx/app.h>

class TestApp : public wxApp {
	public:
		virtual bool OnInit() { return true; }
};

IMPLEMENT_APP(TestApp)
	""", ".cpp")
	context.Result(ret)
	return ret
