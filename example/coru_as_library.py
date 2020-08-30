from ctypes import CDLL
import tempfile

from cffi import FFI


# Create a FFI object.
ffi = FFI()

# Load the declaration of coru.
# Due to the limitation introduced by cffi,
#  we use vanilla data type instead of our original macro.
ffi.cdef("""typedef struct coru_stats_t coru_stats_t;

coru_stats_t * coru_stats_load_fs(FILE *stream);
void coru_stats_delete(void *self);

char ** coru_out_new();
void coru_out_delete(void *self);

unsigned char coru_load_all_fs(FILE *stream, char **out, coru_stats_t *stats, unsigned char lang);
unsigned char coru_load_non_empty_fs(FILE *stream, char **out, coru_stats_t *stats, unsigned char lang);
""")

# Open the compiled dynamic library.
coru = ffi.dlopen("./libcoru.so")

# Write some Python 3 code.
code = """# Python Program to find the area of triangle

a = 5
b = 6
c = 7

# Uncomment below to take inputs from the user
# a = float(input('Enter first side: '))
# b = float(input('Enter second side: '))
# c = float(input('Enter third side: '))

# calculate the semi-perimeter
s = (a + b + c) / 2

# calculate the area
area = (s*(s-a)*(s-b)*(s-c)) ** 0.5
print('The area of the triangle is %0.2f' %area)
"""

# Create a temp file.
tmp = tempfile.TemporaryFile("w+")
# Write our code into the temp file.
tmp.write(code)
# Rewind the tempfile for reading later.
tmp.seek(0)

# Cast the temp file as a C file stream.
fs = ffi.cast("FILE *", tmp)

stats = coru.coru_stats_load_fs(fs)

# Rewind the same file again.
tmp.seek(0)

# Create a coru out object as the output.
out = coru.coru_out_new()

# Due to the limitation introduced by cffi,
#  we hardcode the language code here, which is Python.
lang = ffi.cast("unsigned char", 9)

# Run coru from Python.
st = coru.coru_load_all_fs(fs, out, stats, lang)

# Print out the result within Python.
print(ffi.string(out[0]).decode("utf-8"), end="")

# Close the temp file.
tmp.close()

# Release the coru stats object.
coru.coru_stats_delete(stats)

# Release the coru out object.
coru.coru_out_delete(out)

# Close coru.
ffi.dlclose(coru)
