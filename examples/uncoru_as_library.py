import tempfile

from cffi import FFI


# Create a FFI object.
ffi = FFI()

# Load the declaration of coru.
# Due to the limitation introduced by cffi,
#  we use vanilla data type instead of our original macro.
ffi.cdef("""typedef struct uncoru_stats_t uncoru_stats_t;

uncoru_stats_t * uncoru_stats_load_fs(FILE *stream);
void uncoru_stats_delete(void *self);

char ** uncoru_out_new();
void uncoru_out_delete(void *self);

unsigned char uncoru_load_fs(FILE *stream, char **out, uncoru_stats_t *stats, unsigned char lang);
""")

# Open the compiled dynamic library.
uncoru = ffi.dlopen("./libuncoru.so")

# Write some Python 3 code.
code = """# Python Program to find the area of triangle     #  1

a = 5                                             #  2
b = 6                                             #  3
c = 7                                             #  4

# Uncomment below to take inputs from the user    #  5
# a = float(input('Enter first side: '))          #  6
# b = float(input('Enter second side: '))         #  7
# c = float(input('Enter third side: '))          #  8

# calculate the semi-perimeter                    #  9
s = (a + b + c) / 2                               # 10

# calculate the area                              # 11
area = (s*(s-a)*(s-b)*(s-c)) ** 0.5               # 12
print('The area of the triangle is %0.2f' %area)  # 13
"""

# Create a temp file.
tmp = tempfile.TemporaryFile("w+")
# Write our code into the temp file.
tmp.write(code)
# Rewind the tempfile for reading later.
tmp.seek(0)

# Cast the temp file as a C file stream.
fs = ffi.cast("FILE *", tmp)

stats = uncoru.uncoru_stats_load_fs(fs)

# Rewind the same file again.
tmp.seek(0)

# Create a coru out object as the output.
out = uncoru.uncoru_out_new()

# Due to the limitation introduced by cffi,
#  we hardcode the language code here, which is Python.
lang = ffi.cast("unsigned char", 9)

# Run coru from Python.
st = uncoru.uncoru_load_fs(fs, out, stats, lang)

# Print out the result within Python.
if st != 0:
    print(ffi.string(out[0]).decode("utf-8"), end="")

# Close the temp file.
tmp.close()

# Release the coru out object.
uncoru.uncoru_out_delete(out)

# Release the coru stats object.
uncoru.uncoru_stats_delete(stats)

# Close coru.
ffi.dlclose(uncoru)
