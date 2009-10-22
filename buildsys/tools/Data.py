import os.path

def Data(target, source, env):
	for i in range(len(source)):
		env.Depends(target[i], source[i])
		src = file(str(source[i]), "rb")
		dst = file(str(target[i]), "wb")

		var = os.path.basename(str(source[i])).replace(".", "_").replace("-", "_")
		names = {"define": var.upper(),
			 "var": var}

		dst.write("""#ifndef %(define)s_DATA_C
#define %(define)s_DATA_C

static const unsigned char %(var)s[] = {
  """ % names)

		byte = src.read(1)
		if len(byte) == 1:
			dst.write("0x%02x" % ord(byte))

		linechar = 1
		while True:
			byte = src.read(1)
			if len(byte) == 0:
				break
			dst.write(", 0x%02x" % ord(byte))
			linechar += 1
			if linechar > 12:
				dst.write("\n")
				linechar = 0

		dst.write("""};

#endif
""")

		src.close()
		dst.close()

def generate(env):
	bld = env.Builder(action = Data)
	env.Append(BUILDERS = {"Data": bld})

def exists(env):
	return True
