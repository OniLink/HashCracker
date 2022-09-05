import os
import re
import sys

re_line_hash_locator = re.compile('field_([0-9A-F]{8})')
re_file_hash_locator = re.compile('([0-9A-F]{8}).html')

if len(sys.argv) != 2:
  print('Error: Invalid arguments. Please provide directory of bdats.')

bdat_dir = sys.argv[1]
file_list = os.listdir(bdat_dir)

hash_list = set()
file_hash_list = set()
field_hash_list = set()

# Extract hashes from filenames
file_list_str = ' '.join(file_list)
filename_hashes = re_file_hash_locator.findall(file_list_str)
file_hash_list.update(filename_hashes)
hash_list.update(filename_hashes)

# Extract hashes from files
for filename in file_list:
  for line in open(bdat_dir + '/' + filename):
    line_hashes = re_line_hash_locator.findall(line)
    field_hash_list.update(line_hashes)
    hash_list.update(line_hashes)

with open('FileHashList.txt', mode='w') as hash_list_file:
  for hash_str in file_hash_list:
    hash_list_file.write(hash_str + os.linesep)

with open('FieldHashList.txt', mode='w') as hash_list_file:
  for hash_str in field_hash_list:
    hash_list_file.write(hash_str + os.linesep)

with open('HashList.txt', mode='w') as hash_list_file:
  for hash_str in hash_list:
    hash_list_file.write(hash_str + os.linesep)

