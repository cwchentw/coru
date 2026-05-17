import tempfile

from cffi import FFI


# Create a FFI object.
ffi = FFI()

# Load the declaration of coru.
# Due to the limitation introduced by cffi,
#  we use vanilla data type instead of our original macro.
ffi.cdef("""typedef struct coru_doc_t coru_doc_t;

coru_doc_t * coru_doc_load_all_fs(FILE *stream, unsigned char lang);
coru_doc_t * coru_doc_load_non_empty_fs(FILE *stream, unsigned char lang);
const char * coru_doc_string(const coru_doc_t *self);
void coru_doc_delete(coru_doc_t *self);
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

# Due to the limitation introduced by cffi,
#  we hardcode the language code here, which is Python.
lang = ffi.cast("unsigned char", 9)

# Run coru from Python.
doc = coru.coru_doc_load_all_fs(fs, lang)

# Retrieve the raw C string.
s = coru.coru_doc_string(doc)

# Print out the result within Python.
print(ffi.string(s).decode("utf-8"), end="")

# Close the temp file.
tmp.close()

# Release the coru_document_t object.
coru.coru_doc_delete(doc)

# Close coru.
ffi.dlclose(coru)
