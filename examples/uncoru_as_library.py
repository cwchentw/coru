import tempfile

from cffi import FFI


# Create a FFI object.
ffi = FFI()

# Load the declaration of coru.
# Due to the limitation introduced by cffi,
#  we use vanilla data type instead of our original macro.
ffi.cdef("""typedef struct uncoru_doc_t uncoru_doc_t;

uncoru_doc_t * uncoru_doc_load_fs(FILE *stream, unsigned char lang);
const char * uncoru_doc_string(uncoru_doc_t *doc);
void uncoru_doc_delete(uncoru_doc_t *doc);
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

# Due to the limitation introduced by cffi,
#  we hardcode the language code here, which is Python.
lang = ffi.cast("unsigned char", 9)

# Run uncoru from Python.
doc = uncoru.uncoru_doc_load_fs(fs, lang)

# Retrieve the raw C string.
s = uncoru.uncoru_doc_string(doc)

# Print out the result within Python.
print(ffi.string(s).decode("utf-8"), end="")

# Close the temp file.
tmp.close()

# Release the uncoru doc object.
uncoru.uncoru_doc_delete(doc)

# Close coru.
ffi.dlclose(uncoru)
